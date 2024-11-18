### 1. Chip

#### 1.1 Chip Info

Chip Name: STM32F407ZGT6.

Extern Oscillator: 8MHz.

UART Pin: TX/RX PA9/PA10.

SPI Pin: SCK/MISO/MOSI/CS  PA5/PA6/PA7/PA4.

GPIO Pin: CE/TX_EN/PWR_UP PB1/PB2/PA8.

INT(DR) Pin: INT PB0

### 2. Development and Debugging

#### 2.1 Integrated Development Environment

LibDriver provides both Keil and IAR integrated development environment projects.

MDK is the Keil ARM project and your Keil version must be 5 or higher.Keil ARM project needs STMicroelectronics STM32F4 Series Device Family Pack and you can download from https://www.keil.com/dd2/stmicroelectronics/stm32f407zgtx.

EW is the IAR ARM project and your IAR version must be 9 or higher.

#### 2.2 Serial Port Parameter

Baud Rate: 115200.

Data Bits : 8.

Stop Bits: 1.

Parity: None.

Flow Control: None.

#### 2.3 Serial Port Assistant

We use '\n' to wrap lines.If your serial port assistant displays exceptions (e.g. the displayed content does not divide lines), please modify the configuration of your serial port assistant or replace one that supports '\n' parsing.

### 3. NRF905

#### 3.1 command Instruction

1. Show nrf905 chip and driver information.

   ```shell
   nrf905 (-i | --information)
   ```

2. Show nrf905 help.

   ```shell
   nrf905 (-h | --help)
   ```

3. Show nrf905 pin connections of the current board.

   ```shell
   nrf905 (-p | --port)
   ```

4. Run nrf905 register test.

   ```shell
   nrf905 (-t reg | --test=reg)
   ```

5. Run nrf905 send test.

   ```shell
   nrf905 (-t send | --test=send)
   ```

6. Run nrf905 receive test.

   ```shell
   nrf905 (-t receive | --test=receive)
   ```

7. Run nrf905 send function, data is the send data and it's length must be less 32.

   ```shell
   nrf905 (-e send | --example=send) [--data=<str>]
   ```

8. Run nrf905 receive function, timeout is the timeout time.

   ```shell
   nrf905 (-e receive | --example=receive) [--timeout=<time>]
   ```

#### 3.2 command example

```shell
nrf905 -i

nrf905: chip is Nordic nRF905.
nrf905: manufacturer is Nordic.
nrf905: interface is SPI.
nrf905: driver version is 1.0.
nrf905: min supply voltage is 1.9V.
nrf905: max supply voltage is 3.6V.
nrf905: max current is 30.00mA.
nrf905: max temperature is 85.0C.
nrf905: min temperature is -40.0C.
```

```shell
nrf905 -p

nrf905: SCK connected to GPIOA PIN5.
nrf905: MISO connected to GPIOA PIN6.
nrf905: MOSI connected to GPIOA PIN7.
nrf905: CS connected to GPIOA PIN4.
nrf905: CE connected to GPIOB PIN1.
nrf905: TX_EN connected to GPIOB PIN2.
nrf905: PWR_UP connected to GPIOA PIN8.
nrf905: INT connected to GPIOB PIN0.
```

```shell
nrf905 -t reg

nrf905: chip is Nordic nRF905.
nrf905: manufacturer is Nordic.
nrf905: interface is SPI.
nrf905: driver version is 1.0.
nrf905: min supply voltage is 1.9V.
nrf905: max supply voltage is 3.6V.
nrf905: max current is 30.00mA.
nrf905: max temperature is 85.0C.
nrf905: min temperature is -40.0C.
nrf905: start register test.
nrf905: nrf905_set_tx_payload/nrf905_get_tx_payload test.
nrf905: check tx_payload ok.
nrf905: nrf905_get_rx_payload test.
nrf905: check rx payload ok.
nrf905: nrf905_set_tx_address/nrf905_get_tx_address test.
nrf905: check tx address ok.
nrf905: nrf905_set_frequency/nrf905_get_frequency test.
nrf905: check frequency ok.
nrf905: nrf905_frequency_convert_to_register/nrf905_frequency_convert_to_data test.
nrf905: set frequency 439.40 MHz.
nrf905: check frequency 439.40 MHz.
nrf905: nrf905_set_pll_mode/nrf905_get_pll_mode test.
nrf905: set pll mode 868_915_MHZ.
nrf905: check pll mode ok.
nrf905: set pll mode 433_MHZ.
nrf905: check pll mode ok.
nrf905: nrf905_set_output_power/nrf905_get_output_power test.
nrf905: set output power -10dBm.
nrf905: check output power ok.
nrf905: set output power -2dBm.
nrf905: check output power ok.
nrf905: set output power +6dBm.
nrf905: check output power ok.
nrf905: set output power +10dBm.
nrf905: check output power ok.
nrf905: nrf905_set_rx_mode/nrf905_get_rx_mode test.
nrf905: set rx mode reduced power.
nrf905: check rx mode ok.
nrf905: set rx mode normal.
nrf905: check rx mode ok.
nrf905: nrf905_set_auto_retransmit/nrf905_get_auto_retransmit test.
nrf905: enable the auto retransmit.
nrf905: check auto retransmit ok.
nrf905: disable the auto retransmit.
nrf905: check auto retransmit ok.
nrf905: nrf905_set_rx_address_width/nrf905_get_rx_address_width test.
nrf905: set rx address width 1 byte.
nrf905: check rx address width ok.
nrf905: set rx address width 4 byte.
nrf905: check rx address width ok.
nrf905: nrf905_set_tx_address_width/nrf905_get_tx_address_width test.
nrf905: set tx address width 1 byte.
nrf905: check tx address width ok.
nrf905: set tx address width 4 byte.
nrf905: check tx address width ok.
nrf905: nrf905_set_rx_payload_width/nrf905_get_rx_payload_width test.
nrf905: set rx payload width 9.
nrf905: check rx payload width ok.
nrf905: nrf905_set_tx_payload_width/nrf905_get_tx_payload_width test.
nrf905: set tx payload width 9.
nrf905: check tx payload width ok.
nrf905: nrf905_set_rx_address/nrf905_get_rx_address test.
nrf905: set rx address 0x0B 0x0A 0x60 0x44.
nrf905: check rx address ok.
nrf905: nrf905_set_output_clock_frequency/nrf905_get_output_clock_frequency test.
nrf905: set output clock frequency 4MHz.
nrf905: check output clock frequency ok.
nrf905: set output clock frequency 2MHz.
nrf905: check output clock frequency ok.
nrf905: set output clock frequency 1MHz.
nrf905: check output clock frequency ok.
nrf905: set output clock frequency 500KHz.
nrf905: check output clock frequency ok.
nrf905: nrf905_set_output_clock/nrf905_get_output_clock test.
nrf905: disable the output clock.
nrf905: check output clock ok.
nrf905: enable the output clock.
nrf905: check output clock ok.
nrf905: nrf905_set_crystal_oscillator_frequency/nrf905_get_crystal_oscillator_frequency test.
nrf905: set crystal oscillator frequency 4MHz.
nrf905: check crystal oscillator frequency ok.
nrf905: set crystal oscillator frequency 8MHz.
nrf905: check crystal oscillator frequency ok.
nrf905: set crystal oscillator frequency 12MHz.
nrf905: check crystal oscillator frequency ok.
nrf905: set crystal oscillator frequency 20MHz.
nrf905: check crystal oscillator frequency ok.
nrf905: set crystal oscillator frequency 16MHz.
nrf905: check crystal oscillator frequency ok.
nrf905: nrf905_set_crc/nrf905_get_crc test.
nrf905: disable the crc.
nrf905: check crc ok.
nrf905: enable the crc.
nrf905: check crc ok.
nrf905: nrf905_set_crc_mode/nrf905_get_crc_mode test.
nrf905: set crc mode 8.
nrf905: check crc mode ok.
nrf905: set crc mode 16.
nrf905: check crc mode ok.
nrf905: nrf905_read_conf/nrf905_write_conf test.
nrf905: check conf ok.
nrf905: nrf905_read_conf/nrf905_update_conf test.
nrf905: check conf ok.
nrf905: nrf905_channel_config/nrf905_read_conf test.
nrf905: check channel config ok.
nrf905: nrf905_get_status test.
nrf905: status is 0x00.
nrf905: finish register test.
```

```shell
nrf905 -t send  

nrf905: chip is Nordic nRF905.
nrf905: manufacturer is Nordic.
nrf905: interface is SPI.
nrf905: driver version is 1.0.
nrf905: min supply voltage is 1.9V.
nrf905: max supply voltage is 3.6V.
nrf905: max current is 30.00mA.
nrf905: max temperature is 85.0C.
nrf905: min temperature is -40.0C.
nrf905: start send test.
nrf905: tx done.
nrf905: send successful.
nrf905: finish send test.
```

```shell
nrf905 -t receive 

nrf905: chip is Nordic nRF905.
nrf905: manufacturer is Nordic.
nrf905: interface is SPI.
nrf905: driver version is 1.0.
nrf905: min supply voltage is 1.9V.
nrf905: max supply voltage is 3.6V.
nrf905: max current is 30.00mA.
nrf905: max temperature is 85.0C.
nrf905: min temperature is -40.0C.
nrf905: start receive test.
nrf905: irq receive with length 32.
0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x0E 0x0F 0x10 0x11 0x12 0x13 0x14 0x15 0x16 0x17 0x18 0x19 0x1A 0x1B 0x1C 0x1D 0x1E 0x1F .
nrf905: address match.
nrf905: finish receive test.
```

```shell
nrf905 -e send --data=libdriver--nrf905-nrf905-nrf905

nrf905: send libdriver--nrf905-nrf905-nrf905.
nrf905: tx done.
```

```shell
nrf905 -e receive --timeout=5000

nrf905: receiving with timeout 5000 ms.
nrf905: irq receive with length 32.
libdriver--nrf905-nrf905-nrf905 .
nrf905: address match.
nrf905: finish receiving.
```

```shell
nrf905 -h

Usage:
  nrf905 (-i | --information)
  nrf905 (-h | --help)
  nrf905 (-p | --port)
  nrf905 (-t reg | --test=reg)
  nrf905 (-t send | --test=send)
  nrf905 (-t receive | --test=receive)
  nrf905 (-e send | --example=send) [--data=<str>]
  nrf905 (-e receive | --example=receive) [--timeout=<time>]

Options:
      --data=<str>                   Set the send data.([default: LibDriver])
  -e <send | receive>, --example=<send | receive>
                                     Run the driver example.
  -h, --help                         Show the help.
  -i, --information                  Show the chip information.
  -p, --port                         Display the pin connections of the current board.
  -t <reg | send | receive>, --test=<reg | send | receive>
                                     Run the driver test.
      --timeout=<time>               Set the timeout in ms.([default: 1000])
```

