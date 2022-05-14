/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      main.c
 * @brief     main source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2022-03-31
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2022/03/31  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_nrf905_register_test.h"
#include "driver_nrf905_sent_receive_test.h"
#include "driver_nrf905_basic.h"
#include "gpio.h"
#include <stdlib.h>

uint8_t (*g_gpio_irq)(void) = NULL;        /**< gpio irq function address */

/**
 * @brief     interface receive callback
 * @param[in] type is the receive callback type
 * @param[in] *buf points to a data buffer
 * @param[in] len is the buffer length
 * @note      none
 */
static void a_callback(uint8_t type, uint8_t *buf, uint8_t len)
{
    switch (type)
    {
        case NRF905_STATUS_AM :
        {
            nrf905_interface_debug_print("nrf905: address match.\n");
            
            break;
        }
        case NRF905_STATUS_TX_DONE :
        {
            nrf905_interface_debug_print("nrf905: tx done.\n");
            
            break;
        }
        case NRF905_STATUS_RX_DONE :
        {
            uint8_t i;
            
            nrf905_interface_debug_print("nrf905: irq receive with length %d.\n", len);
            for (i = 0; i < len; i++)
            {
                nrf905_interface_debug_print("%c", buf[i]);
            }
            nrf905_interface_debug_print(".\n");
            
            break;
        }
        default :
        {
            nrf905_interface_debug_print("nrf905: unknown code.\n");
            
            break;
        }
    }
}

/**
 * @brief     nrf905 full function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *             - 0 success
 *             - 1 run failed
 *             - 5 param is invalid
 * @note      none
 */
uint8_t nrf905(uint8_t argc, char **argv)
{
    if (argc == 1)
    {
        goto help;
    }
    else if (argc == 2)
    {
        if (strcmp("-i", argv[1]) == 0)
        {
            nrf905_info_t info;
            
            /* print nrf905 info */
            nrf905_info(&info);
            nrf905_interface_debug_print("nrf905: chip is %s.\n", info.chip_name);
            nrf905_interface_debug_print("nrf905: manufacturer is %s.\n", info.manufacturer_name);
            nrf905_interface_debug_print("nrf905: interface is %s.\n", info.interface);
            nrf905_interface_debug_print("nrf905: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000)/100);
            nrf905_interface_debug_print("nrf905: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
            nrf905_interface_debug_print("nrf905: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
            nrf905_interface_debug_print("nrf905: max current is %0.2fmA.\n", info.max_current_ma);
            nrf905_interface_debug_print("nrf905: max temperature is %0.1fC.\n", info.temperature_max);
            nrf905_interface_debug_print("nrf905: min temperature is %0.1fC.\n", info.temperature_min);
            
            return 0;
        }
        else if (strcmp("-p", argv[1]) == 0)
        {
            /* print pin connection */
            nrf905_interface_debug_print("nrf905: SCK connected to GPIO11(BCM).\n");
            nrf905_interface_debug_print("nrf905: MISO connected to GPIO9(BCM).\n");
            nrf905_interface_debug_print("nrf905: MOSI connected to GPIO10(BCM).\n");
            nrf905_interface_debug_print("nrf905: CS connected to GPIO8(BCM).\n");
            nrf905_interface_debug_print("nrf905: CE connected to GPIO22(BCM).\n");
            nrf905_interface_debug_print("nrf905: TX_EN connected to GPIO27(BCM).\n");
            nrf905_interface_debug_print("nrf905: PWR_UP connected to GPIO5(BCM).\n");
            nrf905_interface_debug_print("nrf905: INT connected to GPIO17(BCM).\n");
            
            return 0;
        }
        else if (strcmp("-h", argv[1]) == 0)
        {
            /* show nrf905 help */
            
            help:
            
            nrf905_interface_debug_print("nrf905 -i\n\tshow nrf905 chip and driver information.\n");
            nrf905_interface_debug_print("nrf905 -h\n\tshow nrf905 help.\n");
            nrf905_interface_debug_print("nrf905 -p\n\tshow nrf905 pin connections of the current board.\n");
            nrf905_interface_debug_print("nrf905 -t reg\n\trun nrf905 register test.\n");
            nrf905_interface_debug_print("nrf905 -t sent\n\trun nrf905 sent test.\n");
            nrf905_interface_debug_print("nrf905 -t receive\n\trun nrf905 receive test.\n");
            nrf905_interface_debug_print("nrf905 -c sent <data>\n\trun nrf905 sent function."
                                         "data is the send data and it's length must be less 32.\n");
            nrf905_interface_debug_print("nrf905 -c receive <timeout>\n\trun nrf905 receive function.timeout is the timeout time.\n");
            
            return 0;
        }
        else
        {
            return 5;
        }
    }
    else if (argc == 3)
    {
        /* run test */
        if (strcmp("-t", argv[1]) == 0)
        {
            /* reg test */
            if (strcmp("reg", argv[2]) == 0)
            {
                uint8_t res;
                
                res = nrf905_register_test();
                if (res != 0)
                {
                    return 1;
                }
                
                return 0;
            }
            
            /* sent test */
            else if (strcmp("sent", argv[2]) == 0)
            {
                uint8_t res;
                
                res = gpio_interrupt_init();
                if (res != 0)
                {
                    return 1;
                }
                g_gpio_irq = nrf905_interrupt_test_irq_handler;
                res = nrf905_sent_test();
                if (res != 0)
                {
                    (void)gpio_interrupt_deinit();
                    g_gpio_irq = NULL;
                    
                    return 1;
                }
                (void)gpio_interrupt_deinit();
                g_gpio_irq = NULL;
                
                return 0;
            }
            
            /* receive test */
            else if (strcmp("receive", argv[2]) == 0)
            {
                uint8_t res;
                
                res = gpio_interrupt_init();
                if (res != 0)
                {
                    return 1;
                }
                g_gpio_irq = nrf905_interrupt_test_irq_handler;
                res = nrf905_receive_test();
                if (res != 0)
                {
                    (void)gpio_interrupt_deinit();
                    g_gpio_irq = NULL;
                    
                    return 1;
                }
                (void)gpio_interrupt_deinit();
                g_gpio_irq = NULL;
                
                return 0;
            }
            
            /* param is invalid */
            else
            {
                return 5;
            }
        }
        /* param is invalid */
        else
        {
            return 5;
        }
    }
    else if (argc == 4)
    {
        /* run the function */
        if (strcmp("-c", argv[1]) == 0)
        {
            if (strcmp("sent", argv[2]) == 0)
            {
                uint8_t res;
                uint8_t addr[4] = NRF905_BASIC_DEFAULT_RX_ADDR;

                res = gpio_interrupt_init();
                if (res != 0)
                {
                    return 1;
                }
                g_gpio_irq = nrf905_interrupt_irq_handler;
                res = nrf905_basic_init(NRF905_MODE_TX, a_callback);
                if (res != 0)
                {
                    (void)gpio_interrupt_deinit();
                    g_gpio_irq = NULL;
                    
                    return 1;
                }
                nrf905_interface_debug_print("nrf905: sent %s.\n", argv[3]);
                if (nrf905_basic_sent((uint8_t *)addr, (uint8_t *)argv[3], (uint8_t)strlen(argv[3])) != 0)
                {
                    (void)nrf905_basic_deinit();
                    (void)gpio_interrupt_deinit();
                    g_gpio_irq = NULL;
                }
                if (nrf905_basic_deinit() != 0)
                {
                    (void)gpio_interrupt_deinit();
                    g_gpio_irq = NULL;
                }
                
                (void)gpio_interrupt_deinit();
                g_gpio_irq = NULL;
                
                return 0;
            }
            else if (strcmp("receive", argv[2]) == 0)
            {
                uint8_t res;
                uint32_t timeout;
                
                timeout = atoi(argv[3]);
                nrf905_interface_debug_print("nrf905: receiving with timeout %d ms.\n", timeout);
                res = gpio_interrupt_init();
                if (res != 0)
                {
                    return 1;
                }
                g_gpio_irq = nrf905_interrupt_irq_handler;
                res = nrf905_basic_init(NRF905_MODE_RX, a_callback);
                if (res != 0)
                {
                    (void)gpio_interrupt_deinit();
                    g_gpio_irq = NULL;
                    
                    return 1;
                }
                nrf905_interface_delay_ms(timeout);
                if (nrf905_basic_deinit() != 0)
                {
                    (void)gpio_interrupt_deinit();
                    g_gpio_irq = NULL;
                }
                
                (void)gpio_interrupt_deinit();
                g_gpio_irq = NULL;
                
                nrf905_interface_debug_print("nrf905: finish receiving.\n");
                
                return 0;
            }
            /* param is invalid */
            else
            {
                return 5;
            }
        }
        /* param is invalid */
        else
        {
            return 5;
        }
    }
    /* param is invalid */
    else
    {
        return 5;
    }
}

/**
 * @brief     main function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *             - 0 success
 * @note      none
 */
int main(uint8_t argc, char **argv)
{
    uint8_t res;

    res = nrf905(argc, argv);
    if (res == 0)
    {
        /* run success */
    }
    else if (res == 1)
    {
        nrf905_interface_debug_print("nrf905: run failed.\n");
    }
    else if (res == 5)
    {
        nrf905_interface_debug_print("nrf905: param is invalid.\n");
    }
    else
    {
        nrf905_interface_debug_print("nrf905: unknow status code.\n");
    }

    return 0;
}
