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
 * @file      driver_nrf905_register_test.c
 * @brief     driver nrf905 register test source file
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
#include <stdlib.h>

static nrf905_handle_t gs_handle;        /**< nrf905 handle */

/**
 * @brief  register test
 * @return status code
 *         - 0 success
 *         - 1 test failed
 * @note   none
 */
uint8_t nrf905_register_test(void)
{
    uint8_t res;
    uint8_t i;
    uint16_t reg;
    uint8_t w;
    uint8_t w_check;
    uint8_t status;
    uint16_t freq;
    uint16_t freq_check;
    float mhz;
    float mhz_check;
    uint8_t buf[32];
    uint8_t buf_check[32];
    nrf905_bool_t enable;
    nrf905_info_t info;
    nrf905_pll_mode_t mode;
    nrf905_rx_mode_t rx_mode;
    nrf905_crc_mode_t crc_mode;
    nrf905_address_width_t width;
    nrf905_output_power_t output_power;
    nrf905_output_clock_frequency_t clock_freq;
    nrf905_crystal_oscillator_frequency_t osc_freq;

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

    /* start register test */
    nrf905_interface_debug_print("nrf905: start register test.\n");

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
        (void)(void)nrf905_deinit(&gs_handle);

        return 1;
    }

    /* disable the tx or rx */
    res = nrf905_set_enable(&gs_handle, NRF905_BOOL_FALSE);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: enable failed.\n");
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

    /* nrf905_set_tx_payload/nrf905_get_tx_payload test */
    nrf905_interface_debug_print("nrf905: nrf905_set_tx_payload/nrf905_get_tx_payload test.\n");

    for (i = 0; i < 32; i++)
    {
        buf[i] = rand() % 256;
    }
    res = nrf905_set_tx_payload(&gs_handle, buf, 32);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set tx payload failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    res = nrf905_get_tx_payload(&gs_handle, buf_check, 32);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: get tx payload failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: check tx_payload %s.\n", (memcmp(buf, buf_check, 32) == 0) ? "ok" : "error");

    /* nrf905_get_rx_payload test */
    nrf905_interface_debug_print("nrf905: nrf905_get_rx_payload test.\n");

    res = nrf905_get_rx_payload(&gs_handle, buf_check, 32);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: get rx payload failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: check rx payload %s.\n", (res == 0) ? "ok" : "error");

    /* nrf905_set_tx_address/nrf905_get_tx_address test */
    nrf905_interface_debug_print("nrf905: nrf905_set_tx_address/nrf905_get_tx_address test.\n");

    for (i = 0; i < 4; i++)
    {
        buf[i] = rand() % 256;
    }
    res = nrf905_set_tx_address(&gs_handle, buf, 4);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set tx address failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    res = nrf905_get_tx_address(&gs_handle, buf_check, 4);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: get tx address failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: check tx address %s.\n", (memcmp(buf, buf_check, 4) == 0) ? "ok" : "error");

    /* nrf905_set_frequency/nrf905_get_frequency test */
    nrf905_interface_debug_print("nrf905: nrf905_set_frequency/nrf905_get_frequency test.\n");

    freq = rand() % 0x1FF;
    res = nrf905_set_frequency(&gs_handle, freq);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set frequency failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    res = nrf905_get_frequency(&gs_handle, (uint16_t *)&freq_check);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: get frequency failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: check frequency %s.\n", (freq_check == freq) ? "ok" : "error");

    /* nrf905_frequency_convert_to_register/nrf905_frequency_convert_to_data test */
    nrf905_interface_debug_print("nrf905: nrf905_frequency_convert_to_register/nrf905_frequency_convert_to_data test.\n");

    mhz = 433.0f + (float)(rand() % 100) / 10.0f;
    res= nrf905_frequency_convert_to_register(&gs_handle, mhz, &reg);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: frequency convert to register failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: set frequency %0.2f MHz.\n", mhz);
    res = nrf905_frequency_convert_to_data(&gs_handle, reg, (float *)&mhz_check);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: frequency convert to data failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: check frequency %0.2f MHz.\n", mhz_check);

    /* nrf905_set_pll_mode/nrf905_get_pll_mode test */
    nrf905_interface_debug_print("nrf905: nrf905_set_pll_mode/nrf905_get_pll_mode test.\n");

    /* set pll mode 868_915_MHZ */
    res = nrf905_set_pll_mode(&gs_handle, NRF905_PLL_MODE_868_915_MHZ);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set pll mode failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: set pll mode 868_915_MHZ.\n");
    res = nrf905_get_pll_mode(&gs_handle, &mode);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: get pll mode failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: check pll mode %s.\n", (mode == NRF905_PLL_MODE_868_915_MHZ) ? "ok" : "error");

    /* set pll mode 433_MHZ */
    res = nrf905_set_pll_mode(&gs_handle, NRF905_PLL_MODE_433_MHZ);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set pll mode failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: set pll mode 433_MHZ.\n");
    res = nrf905_get_pll_mode(&gs_handle, &mode);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: get pll mode failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: check pll mode %s.\n", (mode == NRF905_PLL_MODE_433_MHZ) ? "ok" : "error");

    /* nrf905_set_output_power/nrf905_get_output_power test */
    nrf905_interface_debug_print("nrf905: nrf905_set_output_power/nrf905_get_output_power test.\n");

    /* set output power -10dBm */
    res = nrf905_set_output_power(&gs_handle, NRF905_OUTPUT_POWER_NEGATIVE_10_DBM);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set output power failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: set output power -10dBm.\n");
    res = nrf905_get_output_power(&gs_handle, &output_power);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: get output power failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: check output power %s.\n", (output_power == NRF905_OUTPUT_POWER_NEGATIVE_10_DBM) ? "ok" : "error");

    /* set output power -2dBm */
    res = nrf905_set_output_power(&gs_handle, NRF905_OUTPUT_POWER_NEGATIVE_2_DBM);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set output power failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: set output power -2dBm.\n");
    res = nrf905_get_output_power(&gs_handle, &output_power);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: get output power failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: check output power %s.\n", (output_power == NRF905_OUTPUT_POWER_NEGATIVE_2_DBM) ? "ok" : "error");

    /* set output power +6dBm */
    res = nrf905_set_output_power(&gs_handle, NRF905_OUTPUT_POWER_6_DBM);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set output power failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: set output power +6dBm.\n");
    res = nrf905_get_output_power(&gs_handle, &output_power);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: get output power failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: check output power %s.\n", (output_power == NRF905_OUTPUT_POWER_6_DBM) ? "ok" : "error");

    /* set output power +10dBm */
    res = nrf905_set_output_power(&gs_handle, NRF905_OUTPUT_POWER_10_DBM);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set output power failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: set output power +10dBm.\n");
    res = nrf905_get_output_power(&gs_handle, &output_power);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: get output power failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: check output power %s.\n", (output_power == NRF905_OUTPUT_POWER_10_DBM) ? "ok" : "error");

    /* nrf905_set_rx_mode/nrf905_get_rx_mode test */
    nrf905_interface_debug_print("nrf905: nrf905_set_rx_mode/nrf905_get_rx_mode test.\n");

    /* set rx mode reduced power */
    res = nrf905_set_rx_mode(&gs_handle, NRF905_RX_MODE_REDUCED_POWER);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set rx mode failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: set rx mode reduced power.\n");
    res = nrf905_get_rx_mode(&gs_handle, &rx_mode);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: get rx mode failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: check rx mode %s.\n", (rx_mode == NRF905_RX_MODE_REDUCED_POWER) ? "ok" : "error");

    /* set rx mode normal */
    res = nrf905_set_rx_mode(&gs_handle, NRF905_RX_MODE_NORMAL);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set rx mode failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: set rx mode normal.\n");
    res = nrf905_get_rx_mode(&gs_handle, &rx_mode);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: get rx mode failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: check rx mode %s.\n", (rx_mode == NRF905_RX_MODE_NORMAL) ? "ok" : "error");

    /* nrf905_set_auto_retransmit/nrf905_get_auto_retransmit test */
    nrf905_interface_debug_print("nrf905: nrf905_set_auto_retransmit/nrf905_get_auto_retransmit test.\n");

    /* enable the auto retransmit */
    res = nrf905_set_auto_retransmit(&gs_handle, NRF905_BOOL_TRUE);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set auto retransmit failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: enable the auto retransmit.\n");
    res = nrf905_get_auto_retransmit(&gs_handle, &enable);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: get auto retransmit failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: check auto retransmit %s.\n", (enable == NRF905_BOOL_TRUE) ? "ok" : "error");

    /* disable the auto retransmit */
    res = nrf905_set_auto_retransmit(&gs_handle, NRF905_BOOL_FALSE);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set auto retransmit failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: disable the auto retransmit.\n");
    res = nrf905_get_auto_retransmit(&gs_handle, &enable);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: get auto retransmit failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: check auto retransmit %s.\n", (enable == NRF905_BOOL_FALSE) ? "ok" : "error");

    /* nrf905_set_rx_address_width/nrf905_get_rx_address_width test */
    nrf905_interface_debug_print("nrf905: nrf905_set_rx_address_width/nrf905_get_rx_address_width test.\n");

    /* set rx address width 1 byte */
    res = nrf905_set_rx_address_width(&gs_handle, NRF905_ADDRESS_WIDTH_1_BYTE);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set rx address width failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: set rx address width 1 byte.\n");
    res = nrf905_get_rx_address_width(&gs_handle, &width);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: get rx address width failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: check rx address width %s.\n", (width == NRF905_ADDRESS_WIDTH_1_BYTE) ? "ok" : "error");

    /* set rx address width 4 byte */
    res = nrf905_set_rx_address_width(&gs_handle, NRF905_ADDRESS_WIDTH_4_BYTE);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set rx address width failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: set rx address width 4 byte.\n");
    res = nrf905_get_rx_address_width(&gs_handle, &width);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: get rx address width failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: check rx address width %s.\n", (width == NRF905_ADDRESS_WIDTH_4_BYTE) ? "ok" : "error");

    /* nrf905_set_tx_address_width/nrf905_get_tx_address_width test */
    nrf905_interface_debug_print("nrf905: nrf905_set_tx_address_width/nrf905_get_tx_address_width test.\n");

    /* set tx address width 1 byte */
    res = nrf905_set_tx_address_width(&gs_handle, NRF905_ADDRESS_WIDTH_1_BYTE);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set tx address width failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: set tx address width 1 byte.\n");
    res = nrf905_get_tx_address_width(&gs_handle, &width);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: get tx address width failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: check tx address width %s.\n", (width == NRF905_ADDRESS_WIDTH_1_BYTE) ? "ok" : "error");

    /* set tx address width 4 byte */
    res = nrf905_set_tx_address_width(&gs_handle, NRF905_ADDRESS_WIDTH_4_BYTE);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set tx address width failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: set tx address width 4 byte.\n");
    res = nrf905_get_tx_address_width(&gs_handle, &width);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: get tx address width failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: check tx address width %s.\n", (width == NRF905_ADDRESS_WIDTH_4_BYTE) ? "ok" : "error");

    /* nrf905_set_rx_payload_width/nrf905_get_rx_payload_width test */
    nrf905_interface_debug_print("nrf905: nrf905_set_rx_payload_width/nrf905_get_rx_payload_width test.\n");

    w = rand() % 32;
    res = nrf905_set_rx_payload_width(&gs_handle, w);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set rx payload width failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: set rx payload width %d.\n", w);
    res = nrf905_get_rx_payload_width(&gs_handle, (uint8_t *)&w_check);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: get rx payload width failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: check rx payload width %s.\n", (w_check == w) ? "ok" : "error");

    /* nrf905_set_tx_payload_width/nrf905_get_tx_payload_width test */
    nrf905_interface_debug_print("nrf905: nrf905_set_tx_payload_width/nrf905_get_tx_payload_width test.\n");

    /* set tx payload width */
    res = nrf905_set_tx_payload_width(&gs_handle, w);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set tx payload width failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: set tx payload width %d.\n", w);
    res = nrf905_get_tx_payload_width(&gs_handle, &w_check);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: get tx payload width failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: check tx payload width %s.\n", (w_check == w) ? "ok" : "error");

    /* nrf905_set_rx_address/nrf905_get_rx_address test */
    nrf905_interface_debug_print("nrf905: nrf905_set_rx_address/nrf905_get_rx_address test.\n");

    buf[0] = rand() % 256;
    buf[1] = rand() % 256;
    buf[2] = rand() % 256;
    buf[3] = rand() % 256;
    res = nrf905_set_rx_address(&gs_handle, buf);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set rx address failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: set rx address 0x%02X 0x%02X 0x%02X 0x%02X.\n",
                                 buf[0], buf[1], buf[2], buf[3]);
    res = nrf905_get_rx_address(&gs_handle, buf_check);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: get rx address failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: check rx address %s.\n", (memcmp(buf, buf_check, 4) == 0) ? "ok" : "error");

    /* nrf905_set_output_clock_frequency/nrf905_get_output_clock_frequency test */
    nrf905_interface_debug_print("nrf905: nrf905_set_output_clock_frequency/nrf905_get_output_clock_frequency test.\n");

    /* set output clock frequency 4MHz */
    res = nrf905_set_output_clock_frequency(&gs_handle, NRF905_OUTPUT_CLOCK_FREQUENCY_4MHZ);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set output clock frequency failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: set output clock frequency 4MHz.\n");
    res = nrf905_get_output_clock_frequency(&gs_handle, &clock_freq);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: get output clock frequency failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: check output clock frequency %s.\n", (clock_freq == NRF905_OUTPUT_CLOCK_FREQUENCY_4MHZ) ? "ok" : "error");

    /* set output clock frequency 2MHz */
    res = nrf905_set_output_clock_frequency(&gs_handle, NRF905_OUTPUT_CLOCK_FREQUENCY_2MHZ);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set output clock frequency failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: set output clock frequency 2MHz.\n");
    res = nrf905_get_output_clock_frequency(&gs_handle, &clock_freq);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: get output clock frequency failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: check output clock frequency %s.\n", (clock_freq == NRF905_OUTPUT_CLOCK_FREQUENCY_2MHZ) ? "ok" : "error");

    /* set output clock frequency 1MHz */
    res = nrf905_set_output_clock_frequency(&gs_handle, NRF905_OUTPUT_CLOCK_FREQUENCY_1MHZ);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set output clock frequency failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: set output clock frequency 1MHz.\n");
    res = nrf905_get_output_clock_frequency(&gs_handle, &clock_freq);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: get output clock frequency failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: check output clock frequency %s.\n", (clock_freq == NRF905_OUTPUT_CLOCK_FREQUENCY_1MHZ) ? "ok" : "error");

    /* set output clock frequency 500KHz */
    res = nrf905_set_output_clock_frequency(&gs_handle, NRF905_OUTPUT_CLOCK_FREQUENCY_500KHZ);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set output clock frequency failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: set output clock frequency 500KHz.\n");
    res = nrf905_get_output_clock_frequency(&gs_handle, &clock_freq);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: get output clock frequency failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: check output clock frequency %s.\n", (clock_freq == NRF905_OUTPUT_CLOCK_FREQUENCY_500KHZ) ? "ok" : "error");

    /* nrf905_set_output_clock/nrf905_get_output_clock test */
    nrf905_interface_debug_print("nrf905: nrf905_set_output_clock/nrf905_get_output_clock test.\n");

    /* disable the output clock */
    res = nrf905_set_output_clock(&gs_handle, NRF905_BOOL_FALSE);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set output clock failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: disable the output clock.\n");
    res = nrf905_get_output_clock(&gs_handle, &enable);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: get output clock failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: check output clock %s.\n", (enable == NRF905_BOOL_FALSE) ? "ok" : "error");

    /* enable the output clock */
    res = nrf905_set_output_clock(&gs_handle, NRF905_BOOL_TRUE);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set output clock failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: enable the output clock.\n");
    res = nrf905_get_output_clock(&gs_handle, &enable);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: get output clock failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: check output clock %s.\n", (enable == NRF905_BOOL_TRUE) ? "ok" : "error");

    /* nrf905_set_crystal_oscillator_frequency/nrf905_get_crystal_oscillator_frequency test */
    nrf905_interface_debug_print("nrf905: nrf905_set_crystal_oscillator_frequency/nrf905_get_crystal_oscillator_frequency test.\n");

    /* set crystal oscillator frequency 4MHz */
    res = nrf905_set_crystal_oscillator_frequency(&gs_handle, NRF905_CRYSTAL_OSCILLATOR_FREQUENCY_4MHZ);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set crystal oscillator frequency failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: set crystal oscillator frequency 4MHz.\n");
    res = nrf905_get_crystal_oscillator_frequency(&gs_handle, &osc_freq);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: get crystal oscillator frequency failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: check crystal oscillator frequency %s.\n", (osc_freq == NRF905_CRYSTAL_OSCILLATOR_FREQUENCY_4MHZ) ? "ok" : "error");

    /* set crystal oscillator frequency 8MHz */
    res = nrf905_set_crystal_oscillator_frequency(&gs_handle, NRF905_CRYSTAL_OSCILLATOR_FREQUENCY_8MHZ);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set crystal oscillator frequency failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: set crystal oscillator frequency 8MHz.\n");
    res = nrf905_get_crystal_oscillator_frequency(&gs_handle, &osc_freq);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: get crystal oscillator frequency failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: check crystal oscillator frequency %s.\n", (osc_freq == NRF905_CRYSTAL_OSCILLATOR_FREQUENCY_8MHZ) ? "ok" : "error");

    /* set crystal oscillator frequency 12MHz */
    res = nrf905_set_crystal_oscillator_frequency(&gs_handle, NRF905_CRYSTAL_OSCILLATOR_FREQUENCY_12MHZ);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set crystal oscillator frequency failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: set crystal oscillator frequency 12MHz.\n");
    res = nrf905_get_crystal_oscillator_frequency(&gs_handle, &osc_freq);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: get crystal oscillator frequency failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: check crystal oscillator frequency %s.\n", (osc_freq == NRF905_CRYSTAL_OSCILLATOR_FREQUENCY_12MHZ) ? "ok" : "error");

    /* set crystal oscillator frequency 20MHz */
    res = nrf905_set_crystal_oscillator_frequency(&gs_handle, NRF905_CRYSTAL_OSCILLATOR_FREQUENCY_20MHZ);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set crystal oscillator frequency failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: set crystal oscillator frequency 20MHz.\n");
    res = nrf905_get_crystal_oscillator_frequency(&gs_handle, &osc_freq);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: get crystal oscillator frequency failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: check crystal oscillator frequency %s.\n", (osc_freq == NRF905_CRYSTAL_OSCILLATOR_FREQUENCY_20MHZ) ? "ok" : "error");

    /* set crystal oscillator frequency 16MHz */
    res = nrf905_set_crystal_oscillator_frequency(&gs_handle, NRF905_CRYSTAL_OSCILLATOR_FREQUENCY_16MHZ);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set crystal oscillator frequency failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: set crystal oscillator frequency 16MHz.\n");
    res = nrf905_get_crystal_oscillator_frequency(&gs_handle, &osc_freq);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: get crystal oscillator frequency failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: check crystal oscillator frequency %s.\n", (osc_freq == NRF905_CRYSTAL_OSCILLATOR_FREQUENCY_16MHZ) ? "ok" : "error");

    /* nrf905_set_crc/nrf905_get_crc test */
    nrf905_interface_debug_print("nrf905: nrf905_set_crc/nrf905_get_crc test.\n");

    /* disable the crc */
    res = nrf905_set_crc(&gs_handle, NRF905_BOOL_FALSE);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set crc failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: disable the crc.\n");
    res = nrf905_get_crc(&gs_handle, &enable);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: get crc failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: check crc %s.\n", (enable == NRF905_BOOL_FALSE) ? "ok" : "error");

    /* enable the crc */
    res = nrf905_set_crc(&gs_handle, NRF905_BOOL_TRUE);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set crc failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: enable the crc.\n");
    res = nrf905_get_crc(&gs_handle, &enable);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: get crc failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: check crc %s.\n", (enable == NRF905_BOOL_TRUE) ? "ok" : "error");

    /* nrf905_set_crc_mode/nrf905_get_crc_mode test */
    nrf905_interface_debug_print("nrf905: nrf905_set_crc_mode/nrf905_get_crc_mode test.\n");

    /* set crc mode 8 */
    res = nrf905_set_crc_mode(&gs_handle, NRF905_CRC_MODE_8);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set crc mode failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: set crc mode 8.\n");
    res = nrf905_get_crc_mode(&gs_handle, &crc_mode);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: get crc mode failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: check crc mode %s.\n", (crc_mode == NRF905_CRC_MODE_8) ? "ok" : "error");

    /* set crc mode 16 */
    res = nrf905_set_crc_mode(&gs_handle, NRF905_CRC_MODE_16);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: set crc mode failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: set crc mode 16.\n");
    res = nrf905_get_crc_mode(&gs_handle, &crc_mode);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: get crc mode failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: check crc mode %s.\n", (crc_mode == NRF905_CRC_MODE_16) ? "ok" : "error");

    /* nrf905_read_conf/nrf905_write_conf test */
    nrf905_interface_debug_print("nrf905: nrf905_read_conf/nrf905_write_conf test.\n");

    /* write conf */
    res = nrf905_write_conf(&gs_handle);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: write conf failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    memcpy((uint8_t *)buf, gs_handle.conf, 10);
    res = nrf905_read_conf(&gs_handle, (uint8_t *)buf_check);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: read conf failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: check conf %s.\n", (memcmp(buf, buf_check, 10) == 0) ? "ok" : "error");

    /* nrf905_read_conf/nrf905_update_conf test */
    nrf905_interface_debug_print("nrf905: nrf905_read_conf/nrf905_update_conf test.\n");

    res = nrf905_update_conf(&gs_handle, (uint8_t *)buf);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: update conf failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    res = nrf905_read_conf(&gs_handle, (uint8_t *)buf_check);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: read conf failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: check conf %s.\n", (memcmp(buf, buf_check, 10) == 0) ? "ok" : "error");

    /* nrf905_channel_config/nrf905_read_conf test */
    nrf905_interface_debug_print("nrf905: nrf905_channel_config/nrf905_read_conf test.\n");

    res = nrf905_channel_config(&gs_handle, 0x0000, NRF905_PLL_MODE_868_915_MHZ, NRF905_OUTPUT_POWER_6_DBM);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: channel conf failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    res = nrf905_read_conf(&gs_handle, (uint8_t *)buf);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: read conf failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    res = nrf905_update_conf(&gs_handle, (uint8_t *)buf);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: update conf failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    res = nrf905_get_frequency(&gs_handle, &freq);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: get frequency failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    res = nrf905_get_pll_mode(&gs_handle, &mode);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: get pll mode failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    res = nrf905_get_output_power(&gs_handle, &output_power);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: get output power failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: check channel config %s.\n",
                                ((freq == 0x0000) && (mode == NRF905_PLL_MODE_868_915_MHZ)&&
                                 (output_power == NRF905_OUTPUT_POWER_6_DBM)) ? "ok" : "error");

    /* nrf905_get_status test */
    nrf905_interface_debug_print("nrf905: nrf905_get_status test.\n");

    res = nrf905_get_status(&gs_handle, &status);
    if (res != 0)
    {
        nrf905_interface_debug_print("nrf905: get status failed.\n");
        (void)nrf905_deinit(&gs_handle);

        return 1;
    }
    nrf905_interface_debug_print("nrf905: status is 0x%02X.\n", status);

    /* finish register test */
    nrf905_interface_debug_print("nrf905: finish register test.\n");
    (void)nrf905_deinit(&gs_handle);

    return 0;
}
