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
#include "driver_nrf905_send_receive_test.h"
#include "driver_nrf905_basic.h"
#include "gpio.h"
#include <getopt.h>
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
 *            - 0 success
 *            - 1 run failed
 *            - 5 param is invalid
 * @note      none
 */
uint8_t nrf905(uint8_t argc, char **argv)
{
    int c;
    int longindex = 0;
    const char short_options[] = "hipe:t:";
    const struct option long_options[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"information", no_argument, NULL, 'i'},
        {"port", no_argument, NULL, 'p'},
        {"example", required_argument, NULL, 'e'},
        {"test", required_argument, NULL, 't'},
        {"data", required_argument, NULL, 1},
        {"timeout", required_argument, NULL, 2},
        {NULL, 0, NULL, 0},
    };
    char type[33] = "unknown";
    char data[33] = "LibDriver";
    uint32_t timeout = 1000;
    
    /* if no params */
    if (argc == 1)
    {
        /* goto the help */
        goto help;
    }
    
    /* init 0 */
    optind = 0;
    
    /* parse */
    do
    {
        /* parse the args */
        c = getopt_long(argc, argv, short_options, long_options, &longindex);
        
        /* judge the result */
        switch (c)
        {
            /* help */
            case 'h' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "h");
                
                break;
            }
            
            /* information */
            case 'i' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "i");
                
                break;
            }
            
            /* port */
            case 'p' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "p");
                
                break;
            }
            
            /* example */
            case 'e' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "e_%s", optarg);
                
                break;
            }
            
            /* test */
            case 't' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "t_%s", optarg);
                
                break;
            }
            
            /* data */
            case 1 :
            {
                /* copy data */
                memset(data, 0, sizeof(char) * 33);
                strncpy(data, optarg, 32);
                
                break;
            }
            
            /* timeout */
            case 2 :
            {
                /* set the timeout */
                timeout = atol(optarg);
                
                break;
            } 
            
            /* the end */
            case -1 :
            {
                break;
            }
            
            /* others */
            default :
            {
                return 5;
            }
        }
    } while (c != -1);

    /* run the function */
    if (strcmp("t_reg", type) == 0)
    {
        uint8_t res;
        
        /* run the reg test */
        res = nrf905_register_test();
        if (res != 0)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if (strcmp("t_send", type) == 0)
    {
        uint8_t res;
        
        /* gpio init */
        res = gpio_interrupt_init();
        if (res != 0)
        {
            return 1;
        }
        
        /* set the gpio irq */
        g_gpio_irq = nrf905_interrupt_test_irq_handler;
        
        /* run send test */
        res = nrf905_send_test();
        if (res != 0)
        {
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;
            
            return 1;
        }
        
        /* gpio deinit */
        (void)gpio_interrupt_deinit();
        g_gpio_irq = NULL;
        
        return 0;
    }
    else if (strcmp("t_receive", type) == 0)
    {
        uint8_t res;
        
        /* gpio init */
        res = gpio_interrupt_init();
        if (res != 0)
        {
            return 1;
        }
        
        /* set the gpio irq */
        g_gpio_irq = nrf905_interrupt_test_irq_handler;
        
        /* run receive test */
        res = nrf905_receive_test();
        if (res != 0)
        {
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;
            
            return 1;
        }
        
        /* gpio deinit */
        (void)gpio_interrupt_deinit();
        g_gpio_irq = NULL;
        
        return 0;
    }
    else if (strcmp("e_send", type) == 0)
    {
        uint8_t res;
        uint8_t addr[4] = NRF905_BASIC_DEFAULT_RX_ADDR;
        
        /* gpio init */
        res = gpio_interrupt_init();
        if (res != 0)
        {
            return 1;
        }
        
        /* set the gpio irq */
        g_gpio_irq = nrf905_interrupt_irq_handler;
        
        /* init */
        res = nrf905_basic_init(NRF905_MODE_TX, a_callback);
        if (res != 0)
        {
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;
            
            return 1;
        }
        
        /* output */
        nrf905_interface_debug_print("nrf905: send %s.\n", data);
        
        /* send */
        if (nrf905_basic_send((uint8_t *)addr, (uint8_t *)data, (uint8_t)strlen(data)) != 0)
        {
            (void)nrf905_basic_deinit();
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;
        }
        
        /* deinit */
        if (nrf905_basic_deinit() != 0)
        {
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;
        }
        
        /* gpio deinit */
        (void)gpio_interrupt_deinit();
        g_gpio_irq = NULL;
        
        return 0;
    }
    else if (strcmp("e_receive", type) == 0)
    {
        uint8_t res;
        
        /* output */
        nrf905_interface_debug_print("nrf905: receiving with timeout %d ms.\n", timeout);
        
        /* gpio init */
        res = gpio_interrupt_init();
        if (res != 0)
        {
            return 1;
        }
        
        /* set the gpio irq */
        g_gpio_irq = nrf905_interrupt_irq_handler;
        
        /* init */
        res = nrf905_basic_init(NRF905_MODE_RX, a_callback);
        if (res != 0)
        {
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;
            
            return 1;
        }
        
        /* delay timeout */
        nrf905_interface_delay_ms(timeout);
        
        /* deinit */
        if (nrf905_basic_deinit() != 0)
        {
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;
        }
        
        /* gpio deinit */
        (void)gpio_interrupt_deinit();
        g_gpio_irq = NULL;
        
        /* output */
        nrf905_interface_debug_print("nrf905: finish receiving.\n");
        
        return 0;
    }
    else if (strcmp("h", type) == 0)
    {
        help:
        nrf905_interface_debug_print("Usage:\n");
        nrf905_interface_debug_print("  nrf905 (-i | --information)\n");
        nrf905_interface_debug_print("  nrf905 (-h | --help)\n");
        nrf905_interface_debug_print("  nrf905 (-p | --port)\n");
        nrf905_interface_debug_print("  nrf905 (-t reg | --test=reg)\n");
        nrf905_interface_debug_print("  nrf905 (-t send | --test=send)\n");
        nrf905_interface_debug_print("  nrf905 (-t receive | --test=receive)\n");
        nrf905_interface_debug_print("  nrf905 (-e send | --example=send) [--data=<str>]\n");
        nrf905_interface_debug_print("  nrf905 (-e receive | --example=receive) [--timeout=<time>]\n");
        nrf905_interface_debug_print("\n");
        nrf905_interface_debug_print("Options:\n");
        nrf905_interface_debug_print("      --data=<str>                   Set the send data.([default: LibDriver])\n");
        nrf905_interface_debug_print("  -e <send | receive>, --example=<send | receive>\n");
        nrf905_interface_debug_print("                                     Run the driver example.\n");
        nrf905_interface_debug_print("  -h, --help                         Show the help.\n");
        nrf905_interface_debug_print("  -i, --information                  Show the chip information.\n");
        nrf905_interface_debug_print("  -p, --port                         Display the pin connections of the current board.\n");
        nrf905_interface_debug_print("  -t <reg | send | receive>, --test=<reg | send | receive>\n");
        nrf905_interface_debug_print("                                     Run the driver test.\n");
        nrf905_interface_debug_print("      --timeout=<time>               Set the timeout in ms.([default: 1000])\n");

        return 0;
    }
    else if (strcmp("i", type) == 0)
    {
        nrf905_info_t info;
        
        /* print nrf905 info */
        nrf905_info(&info);
        nrf905_interface_debug_print("nrf905: chip is %s.\n", info.chip_name);
        nrf905_interface_debug_print("nrf905: manufacturer is %s.\n", info.manufacturer_name);
        nrf905_interface_debug_print("nrf905: interface is %s.\n", info.interface);
        nrf905_interface_debug_print("nrf905: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        nrf905_interface_debug_print("nrf905: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        nrf905_interface_debug_print("nrf905: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        nrf905_interface_debug_print("nrf905: max current is %0.2fmA.\n", info.max_current_ma);
        nrf905_interface_debug_print("nrf905: max temperature is %0.1fC.\n", info.temperature_max);
        nrf905_interface_debug_print("nrf905: min temperature is %0.1fC.\n", info.temperature_min);
        
        return 0;
    }
    else if (strcmp("p", type) == 0)
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
        nrf905_interface_debug_print("nrf905: unknown status code.\n");
    }

    return 0;
}
