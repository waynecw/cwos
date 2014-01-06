#include <memory.h>
#include <types.h>
#include <string.h>


struct pginfo_t *free_pages;
struct pginfo_t *pages;

extern void cwos_main();
extern uint32_t kernel_stack_start;

phyaddr_t page2pa(struct pginfo_t *p) 
{
	return (p - pages) * PAGE_SIZE;
}

void inline *page2kva(struct pginfo_t *p)
{
	return (void *)(page2pa(p) + KERNEL_BASE);
}


static void *boot_alloc(size_t n)
{
	static char *nextfree;
	char *result;

	if (!nextfree) {
		extern uint32_t irq_stack_start;
		nextfree = (char *) ((uint32_t) &irq_stack_start - VIRT_OFFSET);
	}
	
	if (!n)
		return nextfree;

	result = nextfree;
	nextfree += n;
	return result;
}

struct pginfo_t *page_alloc(int flag)
{
	struct pginfo_t *p = free_pages;

	if (!free_pages) 
		return NULL;

	free_pages = free_pages->nextfree;

	if (flag & ALLOC_ZERO) {
		phyaddr_t pa = page2pa(p);
		memset((void *)pa, 0, PAGE_SIZE);
	}

	return p;
}

pse_t *pgtable_walk(ttb_t *ttb, const void *va, int create)
{
	pfe_t *pfe;
	pse_t *pse;

	/*
	 * pfe is first-level descriptor entry. We don't need
	 * to allocate its memory because its within TTB 16KB 
	 * (4096 entries * 32bit) region.
	 */
	pfe = (pfe_t *) TB_FPT_ADDR(ttb, va);

	/* 
	 * pfe should provide base address of second-level 
	 * descriptor. If it's empty, allocate a page for the 
	 * second-level descriptor and let pfe point to.
	 */
	if (!*pfe && create) {
		struct pginfo_t *p = page_alloc(ALLOC_ZERO);
		phyaddr_t pa = page2pa(p);

		*pfe = (pa & FPT_ADDR_MASK) | DOMAIN_KERNEL | FPT_BITS;
		p->refcount++;
	}

	if (PAGE_VALID(*pfe)) {
		pse = (pse_t *) FPT_SP_ADDR(*pfe, va);
		return pse;
	}

	return NULL;
}


static void pgtable_map_sections(ttb_t *ttb, void *va, size_t n, phyaddr_t pa, int ap)
{
	int i;

	for (i = 0; i < n; i += PAGE_SIZE) {
		uint32_t mva = (uint32_t) va + i;
		uint32_t *sect_dtr;

		mva = (mva & 0xFFF00000) >> 20;
		mva <<= 2;
		
		sect_dtr = (uint32_t *) ((uint32_t) ttb & 0xFFFFFC00 | mva);
		*sect_dtr = ((pa + i) & 0xFFF00000) | 0xC1E;
	}
}

static void pgtable_map_region(ttb_t *ttb, void *va, size_t n, phyaddr_t pa, int ap) 
{
	int i;

	for (i = 0; i < n; i += PAGE_SIZE) {
		pse_t *pte = pgtable_walk(ttb, va+i, 1);

		*pte = (pse_t) ((pa + i) & SP_ADDR_MASK | SP_INDICATOR | 0xC); // 0xC: C/B 
		*pte = SP_SET_AP(*pte, ap);
	}
}

static void pgtable_init(ttb_t *ttb, uint32_t npages)
{
	int i;
	struct pginfo_t *p;
	pfe_t pfe;

	/* map physical memory 128M to KERNEL_BASE */
	pgtable_map_sections(ttb, (void *) KERNEL_BASE, MEM_SIZE, 0, AP_USER_RDONLY);

	/* map IO memory region to IO_BASE */
	pgtable_map_sections(ttb, (void *) IO_BASE, IO_MEM_SIZE, IO_PHY_BASE, AP_USER);

	/* map 0 to 0 */
	pgtable_map_sections(ttb, (void *) 0x0, PAGE_SIZE * 10, 0, AP_PRIV);
}

static void page_init(int npages)
{
	int i;

	free_pages = NULL;

	for (i = 0; i < npages; i++) {
		pages[i].nextfree = free_pages;

		if (i < (uint32_t) pages / PAGE_SIZE) {
			pages[i].refcount = 1;
			continue;
		}

		pages[i].refcount = 0;
		free_pages = &pages[i];
	}
}

static void mmu_enable(ttb_t *ttb) 
{
	unsigned int dac = 0x1; /* enable domain 0 as Client mode */
	void *new_pc = (void *)(cwos_main + KERNEL_BASE);

	__asm__ __volatile__ (
		"mov sp, %4\n"
		"mov lr, %5\n"
		"mcr p15, 0, %0, c2, c0, 0\n"  /* set TTB */
		"mcr p15, 0, %1, c3, c0, 0\n"  /* set domain AP */
		"mcr p15, 0, %2, c7, c7, 0\n"  /* invalidate D/I cache */
		"mcr p15, 0, %3, c8, c7, 0\n"  /* invalidate TLB */
		"mrc p15, 0, r1, c1, c0, 0\n"  /* ready to enable MMU */
		"orr r1, #0x1\n"
		"mcr p15, 0, r1, c1, c0, 0\n"
		"nop\n"
		"nop\n"
		"mov pc, lr\n"
		: 
		: "r"(ttb), "r"(dac), "r"(0), "r"(0), "r"(&kernel_stack_start), "r"(cwos_main)
		: "r1", "r2"
	);
}

#if 1
void mmu_init()
{
	int i;
	ttb_t* ttb;
	uint32_t npages = MEM_SIZE / PAGE_SIZE; 
	extern uint32_t _ttb_start;
	
	/* ttb contains its own address */
	ttb = (uint32_t *) ((uint32_t) &_ttb_start - VIRT_OFFSET);

	pgtable_init(ttb, npages);
	mmu_enable(ttb);
}
#endif


