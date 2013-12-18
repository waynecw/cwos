
#define UART0_BASE		0x101F1000

#define UART0_DR		UART0_BASE + 0x0

#define UART0_FR		UART0_BASE + 0x18

#define UART0_IBRD		UART0_BASE + 0x24
#define UART0_FBRD		UART0_BASE + 0x28
#define UART0_LCR_H		UART0_BASE + 0x2c

void uart_init()
{
	volatile unsigned char *reg;

	/* set baud rate to 115200 */
	reg = (volatile unsigned char *) UART0_IBRD;
	*reg = 0x11;

	reg = (volatile unsigned char *) UART0_FBRD;
	*reg = 0x17;

	/* set 8n1: data bit:8, no parity bit, 1 stop bit */
	reg = (volatile unsigned char *) UART0_LCR_H;
	*reg = 0x60;


}

void uart0_send(const char *str)
{
	volatile unsigned char *reg_dr = (volatile unsigned char *) UART0_DR;
	volatile unsigned char *reg_fr = (volatile unsigned char *) UART0_FR;

	while (*str != '\0') 
	{
		while (*reg_fr & 0x2);
		*reg_dr = (unsigned char)(*str);
		str++;
	}
}
