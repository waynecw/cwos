#include <irq.h>
#include <uart.h>
#include <memory.h>

//#define UART0_BASE		0x101F1000
#define UART0_BASE  ( 0x101F1000 + VIRT_OFFSET)

#define UART0_DR    ( UART0_BASE + 0x0 )
#define UART0_RSR   ( UART0_BASE + 0x4 )
#define UART0_FR    ( UART0_BASE + 0x18 )
#define UART0_IBRD  ( UART0_BASE + 0x24 )
#define UART0_FBRD  ( UART0_BASE + 0x28 )
#define UART0_LCR_H ( UART0_BASE + 0x2c )
#define UART0_IMSC  ( UART0_BASE + 0x38 )
#define UART0_RIS   ( UART0_BASE + 0x3c )
#define UART0_TMIS  ( UART0_BASE + 0x40 )
#define UART0_ICR   ( UART0_BASE + 0x44 )

#define UART0_IMSC_RX_BIT	1 << 4

#define UART_IRQ 12

#define BUFFER_SIZE 128
static char recv_buffer[BUFFER_SIZE] = {0};
static uint32_t ri;
static uint32_t wi;

void uart_init()
{
	volatile uint32_t *reg = 0;
	request_irq(UART_IRQ, uart0_irq_handler, NULL);

	ri = 0;
	wi = 0;

	/* set baud rate to 115200 */
	reg = (volatile uint32_t *) UART0_IBRD;
	*reg = 0x11;

	reg = (volatile uint32_t *) UART0_FBRD;
	*reg = 0x17;

	/* set 8n1: data bit:8, no parity bit, 1 stop bit */
	reg = (volatile uint32_t *) UART0_LCR_H;
	*reg = 0x60;

	/* enable UART0 interrupt on VIC  */
	reg = (volatile uint32_t *) PIC_INT_ENABLE;
	*reg |= PIC_UART0_BIT;

	/* enable UART0 RX interrupt on PL011 */
	reg = (volatile uint32_t *) UART0_IMSC;
	*reg |= UART0_IMSC_RX_BIT;

	/* check if interrupts are enabled */
	//reg = (volatile uint32_t *) UART0_TMIS;
}

static void uart0_recv()
{
	volatile char *c = (volatile char *) UART0_DR;
//	char echo[2];

	recv_buffer[wi++] = *c;
	if (wi > BUFFER_SIZE)
		wi = 0;

//	echo[0] = *c;
//	uart0_send(echo);
}

void uart0_irq_handler()
{
	volatile uint32_t *reg_ris = (uint32_t *) UART0_RIS;

	/* RX */
	if (*reg_ris & 0x10) {
		/* clear IRQ bit */
		*((volatile uint32_t *) UART0_ICR) = 0x10;
		uart0_recv();
	}
}

void uart0_send(const char *str)
{
	volatile uint8_t *reg_dr = (volatile uint8_t *) UART0_DR;
	volatile uint8_t *reg_fr = (volatile uint8_t *) UART0_FR;

	while (*str != '\0') {
		while (*reg_fr & 0x2);
		*reg_dr = (uint8_t)(*str);
		str++;
	}
}

void uart0_putchar(const char c)
{
	volatile uint8_t *reg_fr = (volatile uint8_t *) UART0_FR;

	while (*reg_fr & 0x2);
	*((volatile char *) UART0_DR) = c;
	while (*reg_fr & 0x2);
}

void uart0_getchar(char *c)
{
	if (ri == wi) {
		*c = '\0';
		return;
	}

	*c = recv_buffer[ri++];

	if (ri > BUFFER_SIZE) 
		ri = 0;
}
