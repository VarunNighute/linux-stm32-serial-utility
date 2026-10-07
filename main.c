#include <stdint.h>

/* RCC */
#define RCC_BASE        0x40021000UL
#define RCC_APB2ENR     (*(volatile uint32_t *)(RCC_BASE + 0x18))

/* GPIOA */
#define GPIOA_BASE      0x40010800UL
#define GPIOA_CRH       (*(volatile uint32_t *)(GPIOA_BASE + 0x04))

/* GPIOC */
#define GPIOC_BASE      0x40011000UL
#define GPIOC_CRH       (*(volatile uint32_t *)(GPIOC_BASE + 0x04))
#define GPIOC_ODR       (*(volatile uint32_t *)(GPIOC_BASE + 0x0C))

/* USART1 */
#define USART1_BASE     0x40013800UL
#define USART1_SR       (*(volatile uint32_t *)(USART1_BASE + 0x00))
#define USART1_DR       (*(volatile uint32_t *)(USART1_BASE + 0x04))
#define USART1_BRR      (*(volatile uint32_t *)(USART1_BASE + 0x08))
#define USART1_CR1      (*(volatile uint32_t *)(USART1_BASE + 0x0C))

#define USART_SR_RXNE   (1U << 5)
#define USART_SR_TXE    (1U << 7)

#define USART_CR1_RE    (1U << 2)
#define USART_CR1_TE    (1U << 3)
#define USART_CR1_UE    (1U << 13)


static void usart1_init(void)
{
    /* Enable GPIOA, GPIOC and USART1 clocks */
    RCC_APB2ENR |= (1U << 2);
    RCC_APB2ENR |= (1U << 4);
    RCC_APB2ENR |= (1U << 14);

    /* PA9 = USART1 TX */
    GPIOA_CRH &= ~(0xFU << 4);
    GPIOA_CRH |=  (0xBU << 4);

    /* PA10 = USART1 RX */
    GPIOA_CRH &= ~(0xFU << 8);
    GPIOA_CRH |=  (0x4U << 8);

    /* PC13 = onboard LED */
    GPIOC_CRH &= ~(0xFU << 20);
    GPIOC_CRH |=  (0x2U << 20);

    /* 115200 baud, assuming 8 MHz APB2 */
    USART1_BRR = 69;

    /* Enable USART TX/RX */
    USART1_CR1 = USART_CR1_TE |
                 USART_CR1_RE |
                 USART_CR1_UE;
}


static void usart1_send_char(char c)
{
    while (!(USART1_SR & USART_SR_TXE))
    {
    }

    USART1_DR = c;
}


static char usart1_receive_char(void)
{
    while (!(USART1_SR & USART_SR_RXNE))
    {
    }

    return (char)USART1_DR;
}


static void usart1_send_string(const char *str)
{
    while (*str)
    {
        usart1_send_char(*str++);
    }
}


static int string_equal(const char *a, const char *b)
{
    while (*a && *b)
    {
        if (*a != *b)
            return 0;

        a++;
        b++;
    }

    return (*a == '\0' && *b == '\0');
}


int main(void)
{
    usart1_init();

    int led_state = 0;

    usart1_send_string("STM32 READY\r\n");

    char command[32];
    int index = 0;

    while (1)
    {
        char c = usart1_receive_char();

        /* End of command */
        if (c == '\r' || c == '\n')
        {
            command[index] = '\0';

            if (string_equal(command, "LED ON"))
            {
                /*
                 * Blue Pill onboard LED is active-low.
                 * LOW = LED ON
                 */
                GPIOC_ODR &= ~(1U << 13);

                led_state = 1;

                usart1_send_string("OK LED ON\r\n");
            }
            else if (string_equal(command, "LED OFF"))
            {
                GPIOC_ODR |= (1U << 13);

                led_state = 0;

                usart1_send_string("OK LED OFF\r\n");
            }
            else if (string_equal(command, "STATUS"))
            {
                if (led_state)
                    usart1_send_string("LED=ON\r\n");
                else
                    usart1_send_string("LED=OFF\r\n");
            }
            else if (index > 0)
            {
                usart1_send_string("ERROR INVALID_CMD\r\n");
            }

            index = 0;
        }
        else
        {
            if (index < sizeof(command) - 1)
            {
                command[index++] = c;
            }
            else
            {
                index = 0;
                usart1_send_string("ERROR CMD_TOO_LONG\r\n");
            }
        }
    }
}
