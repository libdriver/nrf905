### 1. chip

#### 1.1 chip info

chip name : Raspberry Pi 4B

spi pin: SCLK/MOSI/MISO/CS GPIO11/GPIO10/GPIO9/GPIO8

gpio pin: CE/TX_EN/PWR_UP GPIO22/GPIO27/GPIO5

int pin: INT GPIO17

### 2. install

#### 2.1 install info

```shell
sudo apt-get install libgpiod-dev

make
```

### 3. nrf905

#### 3.1 command Instruction

​           nrf905 is a basic command which can test all nrf905 driver function:

​           -i        show nrf905 chip and driver information.

​           -h       show nrf905 help.

​           -p       show nrf905 pin connections of the current board.

​           -t (reg | sent | receive)

​           -t  reg        run nrf905 register test.

​           -t sent        run nrf905 sent test.

​           -t receive        run nrf905 receive test. 

​           -c (sent <data> | receive <timeout>)

​           -c sent <data>        run nrf905 sent function.data is the send data and it's length must be less 32.

​           -c receive <timeout>        run nrf905 receive function. timeout is the timeout time.

#### 3.2 command example

```shell
./nrf905 -i

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
./nrf905 -p

nrf905: SCK connected to GPIO11(BCM).
nrf905: MISO connected to GPIO9(BCM).
nrf905: MOSI connected to GPIO10(BCM).
nrf905: CS connected to GPIO8(BCM).
nrf905: CE connected to GPIO22(BCM).
nrf905: TX_EN connected to GPIO27(BCM).
nrf905: PWR_UP connected to GPIO5(BCM).
nrf905: INT connected to GPIO17(BCM).
```

```shell
./nrf905 -t reg

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
./nrf905 -t sent  

nrf905: chip is Nordic nRF905.
nrf905: manufacturer is Nordic.
nrf905: interface is SPI.
nrf905: driver version is 1.0.
nrf905: min supply voltage is 1.9V.
nrf905: max supply voltage is 3.6V.
nrf905: max current is 30.00mA.
nrf905: max temperature is 85.0C.
nrf905: min temperature is -40.0C.
nrf905: start sent test.
nrf905: tx done.
nrf905: sent successful.
nrf905: finish sent test.
```

```shell
./nrf905 -t receive 

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
./nrf905 -c sent libdriver-nrf905libdriver-nrf905

nrf905: sent libdriver-nrf905libdriver-nrf905.
nrf905: tx done.
```

```shell
./nrf905 -c receive 5000

nrf905: receiving with timeout 5000 ms.
nrf905: irq receive with length 32.
libdriver-nrf905libdriver-nrf905.
nrf905: address match.
nrf905: finish receiving.
```

```shell
./nrf905 -h

nrf905 -i
	show nrf905 chip and driver information.
nrf905 -h
	show nrf905 help.
nrf905 -p
	show nrf905 pin connections of the current board.
nrf905 -t reg
	run nrf905 register test.
nrf905 -t sent
	run nrf905 sent test.
nrf905 -t receive
	run nrf905 receive test.
nrf905 -c sent <data>
	run nrf905 sent function.data is the send data and it's length must be less 32.
nrf905 -c receive <timeout>
	run nrf905 receive function.timeout is the timeout time.
```

