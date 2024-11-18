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
 * @file      driver_nrf905_send_receive_test.c
 * @brief     driver nrf905 send receive test source file
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

#include "driver_nrf905_send_receive_test.h"

static nrf905_handle_t gs_handle;        /**< nrf905 handle */
static volatile uint8_t gs_rx_done;      /**< rx done */

/**
 * @brief  nrf905 interrupt test irq
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t nrf905_interrupt_test_irq_handler(void)
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
                nrf905_interface_debug_print("0x%02X ", buf[i]);
            }
            nrf905_interface_debug_print(".\n");
            gs_rx_done = 1;
            
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
 * @brief  send test
 * @return status code
 *         - 0 success
 *         - 1 test failed
 * @note   none
 */
uint8_t nrf905_send_test(void)
{
    uint8_t i;
    uint8_t res;
    uint16_t reg;
    uint8_t addr[4] = {0xE7, 0xE7, 0xE7, 0xE7};
    uint8_t buffer[32];
    nrf905_info_t info;
    
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
    DRIVER_NRF905_LINK_RECEIVE_CALLBACK(&gs_handle, nrf905_interface_receive_callback);
    
    /* get information */
    res = nrf905_info(&info);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: get info failed.\n");
        
        return 1;
    }
    else
    {
        /* print chip info */
        nrf905_interface_debug_print("nrf905: chip is %s.\n", info.chip_name);
        nrf905_interface_debug_print("nrf905: manufacturer is %s.\n", info.manufacturer_name);
        nrf905_interface_debug_print("nrf905: interface is %s.\n", info.interface);
        nrf905_interface_debug_print("nrf905: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        nrf905_interface_debug_print("nrf905: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        nrf905_interface_debug_print("nrf905: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        nrf905_interface_debug_print("nrf905: max current is %0.2fmA.\n", info.max_current_ma);
        nrf905_interface_debug_print("nrf905: max temperature is %0.1fC.\n", info.temperature_max);
        nrf905_interface_debug_print("nrf905: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* start send test */
    nrf905_interface_debug_print("nrf905: start send test.\n");
    
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
    
    /* set pll mode 433MHz */
    res = nrf905_set_pll_mode(&gs_handle, NRF905_PLL_MODE_433_MHZ);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set pll mode failed.\n");
        (void)nrf905_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set +10dBm */
    res = nrf905_set_output_power(&gs_handle, NRF905_OUTPUT_POWER_10_DBM);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set output power failed.\n");
        (void)nrf905_deinit(&gs_handle);
        
        return 1;
    }
    
    /* normal rx mode */
    res = nrf905_set_rx_mode(&gs_handle, NRF905_RX_MODE_NORMAL);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set rx mode failed.\n");
        (void)nrf905_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable the auto retransmit */
    res = nrf905_set_auto_retransmit(&gs_handle, NRF905_BOOL_FALSE);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set auto retransmit failed.\n");
        (void)nrf905_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set 4 byte */
    res = nrf905_set_rx_address_width(&gs_handle, NRF905_ADDRESS_WIDTH_4_BYTE);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set rx address width failed.\n");
        (void)nrf905_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set 4 byte */
    res = nrf905_set_tx_address_width(&gs_handle, NRF905_ADDRESS_WIDTH_4_BYTE);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set tx address width failed.\n");
        (void)nrf905_deinit(&gs_handle);
        
        return 1;
    }
    
    /* 32 bytes */
    res = nrf905_set_rx_payload_width(&gs_handle, 32);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set rx payload width failed.\n");
        (void)nrf905_deinit(&gs_handle);
        
        return 1;
    }
    
    /* 32 bytes */
    res = nrf905_set_tx_payload_width(&gs_handle, 32);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set tx payload width failed.\n");
        (void)nrf905_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set the rx address */
    res = nrf905_set_rx_address(&gs_handle, addr);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set rx address failed.\n");
        (void)nrf905_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set 500KHz */
    res = nrf905_set_output_clock_frequency(&gs_handle, NRF905_OUTPUT_CLOCK_FREQUENCY_500KHZ);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set output clock frequency failed.\n");
        (void)nrf905_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable the output clock */
    res = nrf905_set_output_clock(&gs_handle, NRF905_BOOL_FALSE);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set output clock failed.\n");
        (void)nrf905_deinit(&gs_handle);
        
        return 1;
    }
    
    /* 16MHz */
    res = nrf905_set_crystal_oscillator_frequency(&gs_handle, NRF905_CRYSTAL_OSCILLATOR_FREQUENCY_16MHZ);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set crystal oscillator frequency failed.\n");
        (void)nrf905_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable the crc */
    res = nrf905_set_crc(&gs_handle, NRF905_BOOL_TRUE);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set crc failed.\n");
        (void)nrf905_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set crc mode 8 */
    res = nrf905_set_crc_mode(&gs_handle, NRF905_CRC_MODE_8);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set crc mode failed.\n");
        (void)nrf905_deinit(&gs_handle);
        
        return 1;
    }
    
    /* 433 MHz */
    res = nrf905_frequency_convert_to_register(&gs_handle, 433.2f, &reg);
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
    
    /* set tx address */
    res = nrf905_set_tx_address(&gs_handle, addr, 4);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set tx address failed.\n");
        (void)nrf905_deinit(&gs_handle);
        
        return 1;
    }
    
    /* send data */
    for (i = 0; i < 32; i++)
    {
        buffer[i] = i;
    }
    res = nrf905_send(&gs_handle, buffer, 32);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: send failed.\n");
        (void)nrf905_deinit(&gs_handle);
        
        return 1;
    }
    else
    {
        nrf905_interface_debug_print("nrf905: send successful.\n");
    }
    
    /* finish send test */
    nrf905_interface_debug_print("nrf905: finish send test.\n");
    (void)nrf905_deinit(&gs_handle);
    
    return 0;
}

/**
 * @brief  receive test
 * @return status code
 *         - 0 success
 *         - 1 test failed
 * @note   none
 */
uint8_t nrf905_receive_test(void)
{
    uint8_t res;
    uint16_t reg;
    uint32_t timeout;
    uint8_t addr[4] = {0xE7, 0xE7, 0xE7, 0xE7};
    nrf905_info_t info;
    
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
    DRIVER_NRF905_LINK_RECEIVE_CALLBACK(&gs_handle, a_callback);
    
    /* get information */
    res = nrf905_info(&info);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: get info failed.\n");
        
        return 1;
    }
    else
    {
        /* print chip info */
        nrf905_interface_debug_print("nrf905: chip is %s.\n", info.chip_name);
        nrf905_interface_debug_print("nrf905: manufacturer is %s.\n", info.manufacturer_name);
        nrf905_interface_debug_print("nrf905: interface is %s.\n", info.interface);
        nrf905_interface_debug_print("nrf905: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        nrf905_interface_debug_print("nrf905: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        nrf905_interface_debug_print("nrf905: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        nrf905_interface_debug_print("nrf905: max current is %0.2fmA.\n", info.max_current_ma);
        nrf905_interface_debug_print("nrf905: max temperature is %0.1fC.\n", info.temperature_max);
        nrf905_interface_debug_print("nrf905: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* start receive test */
    nrf905_interface_debug_print("nrf905: start receive test.\n");
    
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
    
    /* set pll mode 433MHz */
    res = nrf905_set_pll_mode(&gs_handle, NRF905_PLL_MODE_433_MHZ);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set pll mode failed.\n");
        (void)nrf905_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set +10dBm */
    res = nrf905_set_output_power(&gs_handle, NRF905_OUTPUT_POWER_10_DBM);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set output power failed.\n");
        (void)nrf905_deinit(&gs_handle);
        
        return 1;
    }
    
    /* normal rx mode */
    res = nrf905_set_rx_mode(&gs_handle, NRF905_RX_MODE_NORMAL);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set rx mode failed.\n");
        (void)nrf905_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable the auto retransmit */
    res = nrf905_set_auto_retransmit(&gs_handle, NRF905_BOOL_FALSE);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set auto retransmit failed.\n");
        (void)nrf905_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set 4 byte */
    res = nrf905_set_rx_address_width(&gs_handle, NRF905_ADDRESS_WIDTH_4_BYTE);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set rx address width failed.\n");
        (void)nrf905_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set 4 byte */
    res = nrf905_set_tx_address_width(&gs_handle, NRF905_ADDRESS_WIDTH_4_BYTE);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set tx address width failed.\n");
        (void)nrf905_deinit(&gs_handle);
        
        return 1;
    }
    
    /* 32 bytes */
    res = nrf905_set_rx_payload_width(&gs_handle, 32);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set rx payload width failed.\n");
        (void)nrf905_deinit(&gs_handle);
        
        return 1;
    }
    
    /* 32 bytes */
    res = nrf905_set_tx_payload_width(&gs_handle, 32);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set tx payload width failed.\n");
        (void)nrf905_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set the rx address */
    res = nrf905_set_rx_address(&gs_handle, addr);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set rx address failed.\n");
        (void)nrf905_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set 500KHz */
    res = nrf905_set_output_clock_frequency(&gs_handle, NRF905_OUTPUT_CLOCK_FREQUENCY_500KHZ);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set output clock frequency failed.\n");
        (void)nrf905_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable the output clock */
    res = nrf905_set_output_clock(&gs_handle, NRF905_BOOL_FALSE);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set output clock failed.\n");
        (void)nrf905_deinit(&gs_handle);
        
        return 1;
    }
    
    /* 16MHz */
    res = nrf905_set_crystal_oscillator_frequency(&gs_handle, NRF905_CRYSTAL_OSCILLATOR_FREQUENCY_16MHZ);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set crystal oscillator frequency failed.\n");
        (void)nrf905_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable the crc */
    res = nrf905_set_crc(&gs_handle, NRF905_BOOL_TRUE);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set crc failed.\n");
        (void)nrf905_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set crc mode 8 */
    res = nrf905_set_crc_mode(&gs_handle, NRF905_CRC_MODE_8);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set crc mode failed.\n");
        (void)nrf905_deinit(&gs_handle);
        
        return 1;
    }
    
    /* 433 MHz */
    res = nrf905_frequency_convert_to_register(&gs_handle, 433.2f, &reg);
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
    
    /* set tx address */
    res = nrf905_set_tx_address(&gs_handle, addr, 4);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set tx address failed.\n");
        (void)nrf905_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set rx mode */
    res = nrf905_set_mode(&gs_handle, NRF905_MODE_RX);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set mode failed.\n");
        (void)nrf905_deinit(&gs_handle);
        
        return 1;
    }
    
    /* clear rx done */
    timeout = 10 * 1000;
    gs_rx_done = 0;
    res = nrf905_set_enable(&gs_handle, NRF905_BOOL_TRUE);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set enable failed.\n");
        (void)nrf905_deinit(&gs_handle);
        
        return 1;
    }
    
    /* waiting */
    while ((timeout != 0) && (gs_rx_done == 0))
    {
        nrf905_interface_delay_ms(1);
        timeout--;
    }
    
    /* check timeout */
    if ((timeout == 0) && (gs_rx_done == 0))
    {
        nrf905_interface_debug_print("nrf905: receive timeout.\n");
        (void)nrf905_deinit(&gs_handle);
        
        return 1;
    }
    
    /* finish receive test */
    nrf905_interface_debug_print("nrf905: finish receive test.\n");
    (void)nrf905_deinit(&gs_handle);
    
    return 0;
}
