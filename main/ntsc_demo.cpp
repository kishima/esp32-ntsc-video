#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LGFX_USE_V1
#include <LovyanGFX.hpp>

class LGFX : public lgfx::LGFX_Device
{
public:

  lgfx::Panel_CVBS _panel_instance;

  LGFX(void)
  {
    { // 表示パネル制御の設定を行います。
      auto cfg = _panel_instance.config();    // 表示パネル設定用の構造体を取得します。

      // 出力解像度を設定;
      cfg.memory_width  = 240; // 出力解像度 幅
      cfg.memory_height = 160; // 出力解像度 高さ

      // 実際に利用する解像度を設定;
      cfg.panel_width  = 208;  // 実際に使用する幅   (memory_width と同値か小さい値を設定する)
      cfg.panel_height = 128;  // 実際に使用する高さ (memory_heightと同値か小さい値を設定する)

      // 表示位置オフセット量を設定;
      cfg.offset_x = 16;       // 表示位置を右にずらす量 (初期値 0)
      cfg.offset_y = 16;       // 表示位置を下にずらす量 (初期値 0)

      _panel_instance.config(cfg);


// 通常は memory_width と panel_width に同じ値を指定し、 offset_x = 0 で使用します。;
// 画面端の表示が画面外に隠れるのを防止したい場合は、 panel_width の値をmemory_widthより小さくし、offset_x で左右の位置調整をします。;
// 例えば memory_width より panel_width を 32 小さい値に設定した場合、offset_x に 16 を設定することで左右位置が中央寄せになります。;
// 上下方向 (memory_height , panel_height , offset_y ) についても同様に、必要に応じて調整してください。;

    }

    {
      auto cfg = _panel_instance.config_detail();

      // 出力信号の種類を設定;
      // cfg.signal_type = cfg.signal_type_t::NTSC;
      cfg.signal_type = cfg.signal_type_t::NTSC_J;
      // cfg.signal_type = cfg.signal_type_t::PAL;
      // cfg.signal_type = cfg.signal_type_t::PAL_M;
      // cfg.signal_type = cfg.signal_type_t::PAL_N;

      // 出力先のGPIO番号を設定;
      cfg.pin_dac = 26;       // DACを使用するため、 25 または 26 のみが選択できます;

      // PSRAMメモリ割当の設定;
      cfg.use_psram = 1;      // 0=PSRAM不使用 / 1=PSRAMとSRAMを半々使用 / 2=全部PSRAM使用;

      // 出力信号の振幅の強さを設定;
      cfg.output_level = 128; // 初期値128
      // ※ GPIOに保護抵抗が付いている等の理由で信号が減衰する場合は数値を上げる。;
      // ※ M5StackCore2 はGPIOに保護抵抗が付いているため 200 を推奨。;

      // 彩度信号の振幅の強さを設定;
      cfg.chroma_level = 128; // 初期値128
      // 数値を下げると彩度が下がり、0で白黒になります。数値を上げると彩度が上がります。;

      // バックグラウンドでPSRAMの読出しを行うタスクの優先度を設定;
      // cfg.task_priority = 25;

      // バックグラウンドでPSRAMの読出しを行うタスクを実行するCPUを選択 (APP_CPU_NUM or PRO_CPU_NUM);
      // cfg.task_pinned_core = PRO_CPU_NUM;

      _panel_instance.config_detail(cfg);
    }

    setPanel(&_panel_instance);
  }
};


LGFX gfx;

void setup(void)
{
// 色数の指定 (省略時は rgb332_1Byte)
//gfx.setColorDepth( 8);        // RGB332 256色
//gfx.setColorDepth(16);        // RGB565 65536色
//gfx.setColorDepth(lgfx::color_depth_t::rgb332_1Byte);   // RGB332 256色
//gfx.setColorDepth(lgfx::color_depth_t::rgb565_2Byte);   // RGB565 65536色
//gfx.setColorDepth(lgfx::color_depth_t::grayscale_8bit); // モノクロ 256階調

//※ 実行中に setColorDepth で色数を変更することも可能ですが、
//   メモリの再割当を実行するため描画内容は無効になります。

  gfx.init();

  for (int x = 0; x < gfx.width(); ++x)
  {
    int v = x * 256 / gfx.width();
    gfx.fillRect(x, 0 * gfx.height() >> 3, 7, gfx.height() >> 3, gfx.color888(v, v, v));
    gfx.fillRect(x, 1 * gfx.height() >> 3, 7, gfx.height() >> 3, gfx.color888(v, 0 ,0));
    gfx.fillRect(x, 2 * gfx.height() >> 3, 7, gfx.height() >> 3, gfx.color888(0, v, 0));
    gfx.fillRect(x, 3 * gfx.height() >> 3, 7, gfx.height() >> 3, gfx.color888(0, 0, v));
  }
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}

void loop(void)
{
  //printf("ntsc_demo loop\n");
  gfx.fillRect(rand() % gfx.width() - 8, rand() % gfx.height() - 8, 16, 16, rand());
}


extern "C" void app_main(void)
{
    printf("ntsc_demo!\n");
    fflush(stdout);

    setup();
    while(true){
        loop();
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}