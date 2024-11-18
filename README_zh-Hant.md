[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver NRF905

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/nrf905/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

NRF905是一款適用於433/868/915MHz ISM頻段的單晶片無線電收發晶片。 收發器由一個完全集成的頻率合成器、帶解調器的接收器鏈、功率放大器、晶體振盪器和調製器組成。 ShockBurst ™  能自動處理前導碼和CRC。 您可以通過SPI輕鬆配寘NRF905。 晶片的電流消耗非常低，在輸出功率為-10dBm的情况下，傳輸模式下僅為9mA，接收模式下為12.5mA，晶片內寘掉電模式，省電容易實現。 NRF905用於無線資料通信、報警安全系統、家庭自動化、遠程控制等。
LibDriver NRF905是LibDriver推出的NRF905全功能驅動程序，提供無線發送、無線接收等功能並且它符合MISRA標準。

### 目錄

  - [說明](#說明)
  - [安裝](#安裝)
  - [使用](#使用)
    - [example basic send](#example-basic-send)
    - [example basic receive](#example-basic-receive)
  - [文檔](#文檔)
  - [貢獻](#貢獻)
  - [版權](#版權)
  - [聯繫我們](#聯繫我們)

### 說明

/src目錄包含了LibDriver NRF905的源文件。

/interface目錄包含了LibDriver NRF905與平台無關的SPI總線模板。

/test目錄包含了LibDriver NRF905驅動測試程序，該程序可以簡單的測試芯片必要功能。

/example目錄包含了LibDriver NRF905編程範例。

/doc目錄包含了LibDriver NRF905離線文檔。

/datasheet目錄包含了NRF905數據手冊。

/project目錄包含了常用Linux與單片機開發板的工程樣例。所有工程均採用shell腳本作為調試方法，詳細內容可參考每個工程裡面的README.md。

/misra目錄包含了LibDriver MISRA程式碼掃描結果。

### 安裝

參考/interface目錄下與平台無關的SPI總線模板，完成指定平台的SPI總線驅動。

將/src目錄，您使用平臺的介面驅動和您開發的驅動加入工程，如果您想要使用默認的範例驅動，可以將/example目錄加入您的工程。

### 使用

您可以參考/example目錄下的程式設計範例完成適合您的驅動，如果您想要使用默認的程式設計範例，以下是它們的使用方法。

#### example basic send

```C
#include "driver_nrf905_basic.h"

uint8_t (*g_gpio_irq)(void) = NULL;
uint8_t res;
uint8_t addr[4] = NRF905_BASIC_DEFAULT_RX_ADDR;
char data[] = "libdriver";

...

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

...
    
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

...

if (nrf905_basic_send((uint8_t *)addr, (uint8_t *)data, (uint8_t)strlen(data)) != 0)
{
    (void)nrf905_basic_deinit();
    (void)gpio_interrupt_deinit();
    g_gpio_irq = NULL;
}

...

if (nrf905_basic_deinit() != 0)
{
    (void)gpio_interrupt_deinit();
    g_gpio_irq = NULL;
}
(void)gpio_interrupt_deinit();
g_gpio_irq = NULL;

...

return 0;
```

#### example basic receive

```C
#include "driver_nrf905_basic.h"

uint8_t (*g_gpio_irq)(void) = NULL;
uint8_t res;
uint32_t timeout;
uint8_t addr[4] = NRF905_BASIC_DEFAULT_RX_ADDR;

...

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

...
    
timeout = 5000;
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

...

nrf905_interface_delay_ms(timeout);

...

if (nrf905_basic_deinit() != 0)
{
    (void)gpio_interrupt_deinit();
    g_gpio_irq = NULL;
}
(void)gpio_interrupt_deinit();
g_gpio_irq = NULL;

...

return 0;
```

### 文檔

在線文檔: [https://www.libdriver.com/docs/nrf905/index.html](https://www.libdriver.com/docs/nrf905/index.html)。

離線文檔: /doc/html/index.html。

### 貢獻

請參攷CONTRIBUTING.md。

### 版權

版權 (c) 2015 - 現在 LibDriver 版權所有

MIT 許可證（MIT）

特此免費授予任何獲得本軟件副本和相關文檔文件（下稱“軟件”）的人不受限制地處置該軟件的權利，包括不受限制地使用、複製、修改、合併、發布、分發、轉授許可和/或出售該軟件副本，以及再授權被配發了本軟件的人如上的權利，須在下列條件下：

上述版權聲明和本許可聲明應包含在該軟件的所有副本或實質成分中。

本軟件是“如此”提供的，沒有任何形式的明示或暗示的保證，包括但不限於對適銷性、特定用途的適用性和不侵權的保證。在任何情況下，作者或版權持有人都不對任何索賠、損害或其他責任負責，無論這些追責來自合同、侵權或其它行為中，還是產生於、源於或有關於本軟件以及本軟件的使用或其它處置。

### 聯繫我們

請聯繫lishifenging@outlook.com。