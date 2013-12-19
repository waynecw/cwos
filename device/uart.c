#include <irq.h>

#define UART0_BASE		0x101F1000

#define UART0_DR		( UART0_BASE + 0x0 )
#define UART0_FR		( UART0_BASE + 0x18 )
#define UART0_IBRD		( UART0_BASE + 0x24 )
#define UART0_FBRD		( UART0_BASE + 0x28 )
#define UART0_LCR_H		( UART0_BASE + 0x2c )
#define UART0_IMSC		( UART0_BASE + 0x38 )
#define UART0_RIS		( UART0_BASE + 0x3c )
#define UART0_TMIS		( UART0_BASE + 0x40 )

#define UART0_IMSC_RX_BIT	1 << 4

void uart_init()
{
	volatile unsigned int *reg = 0;

	/* set baud rate to 115200 */
	reg = (volatile unsigned int *) UART0_IBRD;
	*reg = 0x11;

	reg = (volatile unsigned int *) UART0_FBRD;
	*reg = 0x17;

	/* set 8n1: data bit:8, no parity bit, 1 stop bit */
	reg = (volatile unsigned int *) UART0_LCR_H;
	*reg = 0x60;

	/* enable UART0 interrupt on VIC  */
	reg = (volatile unsigned int *) PIC_INT_ENABLE;
	*reg |= PIC_UART0_BIT;

	/* enable UART0 RX interrupt on PL011 */
	reg = (volatile unsigned int *) UART0_IMSC;
	*reg |= UART0_IMSC_RX_BIT;

	/* check if interrupts are enabled */
	//reg = (volatile unsigned int *) UART0_TMIS;
}

void uart0_send(const char *str)
{
	volatile unsigned char *reg_dr = (volatile unsigned char *) UART0_DR;
	volatile unsigned char *reg_fr = (volatile unsigned char *) UART0_FR;

	while (*str != '\0') {
		while (*reg_fr & 0x2);
		*reg_dr = (unsigned char)(*str);
		str++;
	}
}
