[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver NRF905

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/nrf905/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

NRF905 は、433/868/915MHz ISM バンド用のシングルチップ無線トランシーバです。 トランシーバーは、完全に統合された周波数シンセサイザー、復調器を備えたレシーバー チェーン、パワー アンプ、水晶発振器、および変調器で構成されています。 ShockBurst™ 機能は、プリアンブルと CRC を自動的に処理します。 SPIを介してnRF905を簡単に設定できます。 消費電流は非常に低く、送信時は-10dBmの出力電力でわずか9mA、受信モードでは12.5mAです。 組み込みのパワー ダウン モードにより、省電力を簡単に実現できます。NRF905 は、ワイヤレス データ通信、アラーム セキュリティ システム、ホーム オートメーション、リモート コントロールなどで使用されます。

LibDriver NRF905 は、LibDriver が起動する NRF905 のフル機能ドライバーです。ワイヤレス送信、ワイヤレス受信などの機能を提供します。LibDriver は MISRA に準拠しています。

### 目次

  - [説明](#説明)
  - [インストール](#インストール)
  - [使用](#使用)
    - [example basic send](#example-basic-send)
    - [example basic receive](#example-basic-receive)
  - [ドキュメント](#ドキュメント)
  - [貢献](#貢献)
  - [著作権](#著作権)
  - [連絡して](#連絡して)

### 説明

/ srcディレクトリには、LibDriver NRF905のソースファイルが含まれています。

/ interfaceディレクトリには、LibDriver NRF905用のプラットフォームに依存しないSPIバステンプレートが含まれています。

/ testディレクトリには、チップの必要な機能を簡単にテストできるLibDriver NRF905ドライバーテストプログラムが含まれています。

/ exampleディレクトリには、LibDriver NRF905プログラミング例が含まれています。

/ docディレクトリには、LibDriver NRF905オフラインドキュメントが含まれています。

/ datasheetディレクトリには、NRF905データシートが含まれています。

/ projectディレクトリには、一般的に使用されるLinuxおよびマイクロコントローラー開発ボードのプロジェクトサンプルが含まれています。 すべてのプロジェクトは、デバッグ方法としてシェルスクリプトを使用しています。詳細については、各プロジェクトのREADME.mdを参照してください。

/ misraはLibDriver misraコードスキャン結果を含む。

### インストール

/ interfaceディレクトリにあるプラットフォームに依存しないSPIバステンプレートを参照して、指定したプラットフォームのSPIバスドライバを完成させます。

/src ディレクトリ、プラットフォームのインターフェイス ドライバー、および独自のドライバーをプロジェクトに追加します。デフォルトのサンプル ドライバーを使用する場合は、/example ディレクトリをプロジェクトに追加します。

### 使用

/example ディレクトリ内のサンプルを参照して、独自のドライバーを完成させることができます。 デフォルトのプログラミング例を使用したい場合の使用方法は次のとおりです。

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

```c
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

### ドキュメント

オンラインドキュメント: [https://www.libdriver.com/docs/nrf905/index.html](https://www.libdriver.com/docs/nrf905/index.html)。

オフラインドキュメント: /doc/html/index.html。

### 貢献

CONTRIBUTING.mdを参照してください。

### 著作権

著作権（c）2015-今 LibDriver 全著作権所有

MITライセンス（MIT）

このソフトウェアおよび関連するドキュメントファイル（「ソフトウェア」）のコピーを取得した人は、無制限の使用、複製、変更、組み込み、公開、配布、サブライセンスを含む、ソフトウェアを処分する権利を制限なく付与されます。ソフトウェアのライセンスおよび/またはコピーの販売、および上記のようにソフトウェアが配布された人の権利のサブライセンスは、次の条件に従うものとします。

上記の著作権表示およびこの許可通知は、このソフトウェアのすべてのコピーまたは実体に含まれるものとします。

このソフトウェアは「現状有姿」で提供され、商品性、特定目的への適合性、および非侵害の保証を含むがこれらに限定されない、明示または黙示を問わず、いかなる種類の保証もありません。 いかなる場合も、作者または著作権所有者は、契約、不法行為、またはその他の方法で、本ソフトウェアおよび本ソフトウェアの使用またはその他の廃棄に起因または関連して、請求、損害、またはその他の責任を負わないものとします。

### 連絡して

お問い合わせくださいlishifenging@outlook.com。