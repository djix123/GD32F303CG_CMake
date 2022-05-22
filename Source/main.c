#include "main.h"
#include "gd32f30x.h"

#include "cdc_acm_core.h"
#include "usbd_hw.h"

#include <stdio.h>
#include <string.h>

#define LEDPORT     GPIOB
#define LEDPIN      GPIO_PIN_2
#define LED_CLOCK   RCU_GPIOB

void systick_config(void);
void delay_1ms(uint32_t count);

usb_dev usbd_cdc;
int _write(int fd, char *ch, int len)
{
    usb_cdc_handler *cdc = (usb_cdc_handler *)(&usbd_cdc)->class_data[CDC_COM_INTERFACE];

    int offset = 0;
    while(len > 0) {
        int toSend = 0;
        if(len > CDC_ACM_DATA_PACKET_SIZE) {
            toSend = CDC_ACM_DATA_PACKET_SIZE;
        }
        else {
            toSend = len;
        }

        cdc->receive_length = toSend;
        memcpy(cdc->data, (ch+offset), toSend);
        cdc->packet_sent = 1U;
        cdc_acm_data_send(&usbd_cdc);

        offset += toSend;
        len -= toSend;
    }

    return offset;
}

int main(void)
{
    systick_config();

    rcu_periph_clock_enable(LED_CLOCK);

    gpio_pin_remap_config(GPIO_SWJ_SWDPENABLE_REMAP,ENABLE);
    gpio_init(LEDPORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, LEDPIN);

    /* system clocks configuration */
    rcu_config();

    /* GPIO configuration */
    gpio_config();

    /* USB device configuration */
    usbd_init(&usbd_cdc, &cdc_desc, &cdc_class);

    /* NVIC configuration */
    nvic_config();

    /* enabled USB pull-up */
    usbd_connect(&usbd_cdc);

    while (USBD_CONFIGURED != usbd_cdc.cur_status) {
        /* wait for standard USB enumeration is finished */
    }

//    while (1) {
//        if (0U == cdc_acm_check_ready(&usbd_cdc)) {
//            cdc_acm_data_receive(&usbd_cdc);
//        } else {
//            cdc_acm_data_send(&usbd_cdc);
//        }
//    }

    uint32_t count = 0;
    while (1)
    {
        gpio_bit_write(LEDPORT, LEDPIN, SET);
        delay_1ms(500);
        gpio_bit_write(LEDPORT, LEDPIN, RESET);
        delay_1ms(500);

        printf("Loop ... %ld\r\n", count);

        count++;
    }
}

volatile static uint32_t delay;

void systick_config(void)
{
    /* setup systick timer for 1000Hz interrupts */
    if (SysTick_Config(SystemCoreClock / 1000U))
    {
        /* capture error */
        while (1)
        {
        }
    }
    /* configure the systick handler priority */
    NVIC_SetPriority(SysTick_IRQn, 0x00U);
}

void delay_1ms(uint32_t count)
{
    delay = count;

    while (0U != delay)
    {
    }
}

void delay_decrement(void)
{
    if (0U != delay)
    {
        delay--;
    }
}

/*
void NMI_Handler(void) {}

void HardFault_Handler(void)
{
    while (1)
        ;
}

void MemManage_Handler(void)
{
    while (1)
        ;
}

void BusFault_Handler(void)
{
    while (1)
        ;
}

void UsageFault_Handler(void)
{
    while (1)
        ;
}

void SVC_Handler(void)
{
}

void DebugMon_Handler(void)
{
}

void PendSV_Handler(void)
{
}
*/

void SysTick_Handler(void)
{
    delay_decrement();
}
