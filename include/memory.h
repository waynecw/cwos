#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <types.h>


/* memory size */
#define MEM_SIZE 0x8000000

/* translation table */
#define TB_SIZE 0x4000
#define TB_TBI_MASK (~0x3FFF)
#define TB_TBI_OFFSET 20
#define TB_TABLE_INDEX(va) (((uint32_t) va >> TB_TBI_OFFSET) << 2)

#define TB_FPT_ADDR(ttb, va) ({ \
	uint32_t __ttb = (uint32_t) ttb; \
	(__ttb & TB_TBI_MASK) |  TB_TABLE_INDEX(va); \
})

#define MVA_L2_OFFSET 12
#define MVA_L2_MASK 0xFFFFF
#define MVA_L2_INDEX(va) \
	(((((uint32_t) va) & MVA_L2_MASK) >> MVA_L2_OFFSET ) << 2)

#define FPT_SP_ADDR(baseaddr, va) ({ \
	uint32_t __ba = (uint32_t) baseaddr; \
	__ba & FPT_ADDR_MASK | MVA_L2_INDEX(va); \
})


/* first-level descriptor: fine page table descriptor */
#define FPT_BITS 0x13  /* fine page table descriptor */
#define FPT_ADDR_MASK (~0xFFF) 

#define DOMAIN_KERNEL 0 << 5
#define DOMAIN_USER 1 << 5

#define AP_PRIV 0x1
#define AP_USER_RDONLY 0x2
#define AP_USER 0x3

/* second-level descriptor: small page descriptor */
#define SP_INDICATOR 0x2
#define SP_ADDR_MASK (~0xFFF)

#define SP_SET_AP(e, ap) ((e & ~(0x3 << 4)) | (ap << 4))

#define PAGE_VALID(x) (x & 0x3)  


/* 4KB small page */
#define SMALL_PGSIZE 0x1000
#define PAGE_SIZE SMALL_PGSIZE


/* page table pointer definition */
typedef uint32_t ttb_t; /* translation table base */
typedef uint32_t pfe_t; /* first-level descriptor entry */
typedef uint32_t pse_t; /* second-level descriptor entry */

/* kernel maintained structure for each physical page */
struct pginfo_t 
{
	struct pginfo_t *nextfree; 
	int refcount;
};

enum {
	ALLOC_ZERO = 1,
};

/* address translation */
phyaddr_t page2pa(struct pginfo_t *p);
void *page2kva(struct pginfo_t *p);


/* memory layout */
#define KERNEL_BASE 0xC0000000
#define IO_BASE 0xD0000000

#define IO_MEM_SIZE 0x001F5000
#define IO_PHY_BASE 0x10000000

#define __iopa2va(iopa) (iopa + (IO_BASE - IO_PHY_BASE))

#endif
