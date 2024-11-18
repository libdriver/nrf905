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
 * @file      driver_nrf905_basic.c
 * @brief     driver nrf905 basic source file
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

#include "driver_nrf905_basic.h"

static nrf905_handle_t gs_handle;        /**< nrf905 handle */

/**
 * @brief  nrf905 irq
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t nrf905_interrupt_irq_handler(void)
{
    if (nrf905_irq_handler(&gs_handle) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief     basic example init
 * @param[in] mode is the chip working mode
 * @param[in] *callback points to a callback function
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t nrf905_basic_init(nrf905_mode_t mode, void (*callback)(uint8_t type, uint8_t *buf, uint8_t len))
{
    uint8_t res;
    uint16_t reg;
    uint8_t addr[4] = NRF905_BASIC_DEFAULT_RX_ADDR;
    
    /* link function */
    DRIVER_NRF905_LINK_INIT(&gs_handle, nrf905_handle_t);
    DRIVER_NRF905_LINK_SPI_INIT(&gs_handle, nrf905_interface_spi_init);
    DRIVER_NRF905_LINK_SPI_DEINIT(&gs_handle, nrf905_interface_spi_deinit);
    DRIVER_NRF905_LINK_SPI_READ(&gs_handle, nrf905_interface_spi_read);
    DRIVER_NRF905_LINK_SPI_WRITE(&gs_handle, nrf905_interface_spi_write);
    DRIVER_NRF905_LINK_SPI_TRANSMIT(&gs_handle, nrf905_interface_spi_transmit);
    DRIVER_NRF905_LINK_CE_GPIO_INIT(&gs_handle, nrf905_interface_ce_gpio_init);
    DRIVER_NRF905_LINK_CE_GPIO_DEINIT(&gs_handle, nrf905_interface_ce_gpio_deinit);
    DRIVER_NRF905_LINK_CE_GPIO_WRITE(&gs_handle, nrf905_interface_ce_gpio_write);
    DRIVER_NRF905_LINK_TX_EN_GPIO_INIT(&gs_handle, nrf905_interface_tx_en_gpio_init);
    DRIVER_NRF905_LINK_TX_EN_GPIO_DEINIT(&gs_handle, nrf905_interface_tx_en_gpio_deinit);
    DRIVER_NRF905_LINK_TX_EN_GPIO_WRITE(&gs_handle, nrf905_interface_tx_en_gpio_write);
    DRIVER_NRF905_LINK_PWR_UP_GPIO_INIT(&gs_handle, nrf905_interface_pwr_up_gpio_init);
    DRIVER_NRF905_LINK_PWR_UP_GPIO_DEINIT(&gs_handle, nrf905_interface_pwr_up_gpio_deinit);
    DRIVER_NRF905_LINK_PWR_UP_GPIO_WRITE(&gs_handle, nrf905_interface_pwr_up_gpio_write);
    DRIVER_NRF905_LINK_DELAY_MS(&gs_handle, nrf905_interface_delay_ms);
    DRIVER_NRF905_LINK_DEBUG_PEINT(&gs_handle, nrf905_interface_debug_print);
    DRIVER_NRF905_LINK_RECEIVE_CALLBACK(&gs_handle, callback);
    
    /* init */
    res = nrf905_init(&gs_handle);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: init failed.\n");
        
        return 1;
    }
    
    /* power up the chip */
    res = nrf905_set_power_up(&gs_handle, NRF905_BOOL_TRUE);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: power up failed.\n");
        (void)nrf905_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable the chip */
    res = nrf905_set_enable(&gs_handle, NRF905_BOOL_FALSE);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set enable failed.\n");
        (void)nrf905_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set the pll mode */
    res = nrf905_set_pll_mode(&gs_handle, NRF905_BASIC_DEFAULT_PLL_MODE);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set pll mode failed.\n");
        (void)nrf905_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set the output power */
    res = nrf905_set_output_power(&gs_handle, NRF905_BASIC_DEFAULT_OUTPUT_POWER);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set output power failed.\n");
        (void)nrf905_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set the rx mode */
    res = nrf905_set_rx_mode(&gs_handle, NRF905_BASIC_DEFAULT_RX_MODE);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set rx mode failed.\n");
        (void)nrf905_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set the auto retransmit */
    res = nrf905_set_auto_retransmit(&gs_handle, NRF905_BASIC_DEFAULT_AUTO_RETRANSMIT);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set auto retransmit failed.\n");
        (void)nrf905_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set the rx address width */
    res = nrf905_set_rx_address_width(&gs_handle, NRF905_BASIC_DEFAULT_RX_ADDRESS_WIDTH);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set rx address width failed.\n");
        (void)nrf905_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set the tx address width */
    res = nrf905_set_tx_address_width(&gs_handle, NRF905_BASIC_DEFAULT_TX_ADDRESS_WIDTH);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set tx address width failed.\n");
        (void)nrf905_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set the rx payload width */
    res = nrf905_set_rx_payload_width(&gs_handle, NRF905_BASIC_DEFAULT_RX_PAYLOAD_WIDTH);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set rx payload width failed.\n");
        (void)nrf905_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set the tx payload width  */
    res = nrf905_set_tx_payload_width(&gs_handle, NRF905_BASIC_DEFAULT_TX_PAYLOAD_WIDTH);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set tx payload width failed.\n");
        (void)nrf905_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set the rx address */
    res = nrf905_set_rx_address(&gs_handle, (uint8_t *)addr);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set rx address failed.\n");
        (void)nrf905_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set output clock frequency */
    res = nrf905_set_output_clock_frequency(&gs_handle, NRF905_BASIC_DEFAULT_OUTPUT_CLOCK_FREQUENCY);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set output clock frequency failed.\n");
        (void)nrf905_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set the output clock */
    res = nrf905_set_output_clock(&gs_handle, NRF905_BASIC_DEFAULT_OUTPUT_CLOCK);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set output clock failed.\n");
        (void)nrf905_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set the crystal oscillator frequency */
    res = nrf905_set_crystal_oscillator_frequency(&gs_handle, NRF905_BASIC_DEFAULT_CRYSTAL_OSCILLATOR_FREQUENCY);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set crystal oscillator frequency failed.\n");
        (void)nrf905_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set the crc */
    res = nrf905_set_crc(&gs_handle, NRF905_BASIC_DEFAULT_CRC);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set crc failed.\n");
        (void)nrf905_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set the crc mode */
    res = nrf905_set_crc_mode(&gs_handle, NRF905_BASIC_DEFAULT_CRC_MODE);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set crc mode failed.\n");
        (void)nrf905_deinit(&gs_handle);
        
        return 1;
    }
    
    /* 433 MHz */
    res = nrf905_frequency_convert_to_register(&gs_handle, NRF905_BASIC_DEFAULT_FREQUENCY, &reg);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: frequency convert to register failed.\n");
        (void)nrf905_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set the frequency */
    res = nrf905_set_frequency(&gs_handle, reg);  
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set frequency failed.\n");
        (void)nrf905_deinit(&gs_handle);
        
        return 1;
    }
    
    /* write the conf */
    res = nrf905_write_conf(&gs_handle);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: write conf failed.\n");
        (void)nrf905_deinit(&gs_handle);
        
        return 1;
    }
    
    if (mode == NRF905_MODE_TX)
    {
        return 0;
    }
    else
    {
        /* set rx mode */
        res = nrf905_set_mode(&gs_handle, NRF905_MODE_RX);
        if (res != 0)
        {
            nrf905_interface_debug_print("nrf905: set mode failed.\n");
            (void)nrf905_deinit(&gs_handle);
            
            return 1;
        }
        
        /* enable the chip */
        res = nrf905_set_enable(&gs_handle, NRF905_BOOL_TRUE);
        if (res != 0)
        {
            nrf905_interface_debug_print("nrf905: set enable failed.\n");
            (void)nrf905_deinit(&gs_handle);
            
            return 1;
        }
        
        return 0;
    }
}

/**
 * @brief  basic example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t nrf905_basic_deinit(void)
{
    if (nrf905_deinit(&gs_handle) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief     basic example send
 * @param[in] *addr points to a address buffer
 * @param[in] *buf points to a data buffer
 * @param[in] len is the buffer length
 * @return    status code
 *            - 0 success
 *            - 1 send failed
 * @note      none
 */
uint8_t nrf905_basic_send(uint8_t *addr, uint8_t *buf, uint8_t len)
{
    uint8_t res;
    
    /* set tx address */
    res = nrf905_set_tx_address(&gs_handle, addr, 4);
    if (res != 0)
    {
        return 1;
    }
    
    /* send data */
    res = nrf905_send(&gs_handle, (uint8_t *)buf, len);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}
