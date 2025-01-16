//
// Created by Administrator on 2025/1/15.
//

#include <cstdio>
#include "esp_log.h"
#include <esp_timer.h>
#include <driver/gpio.h>
#include <esp_rom_gpio.h>
#include "esp_task_wdt.h"
#include "demos/widgets/lv_demo_widgets.h"
#include "screen/lcd_drive/st7789_driver.h"
#include <lvgl.h>

void show_dome(int t) {
    static lv_obj_t *button = lv_btn_create(lv_scr_act());
    lv_obj_set_size(button, 100, 50);
    lv_obj_align(button, LV_ALIGN_CENTER, 0, 0);

    static lv_obj_t *label = lv_label_create(button);
//    lv_label_set_text(label, "Hello, World!");

    lv_label_set_text_fmt(label, "World! %d", t);

    lv_timer_create(
            [](lv_timer_t *timer) {
                static int i = 0;
                i++;
                lv_label_set_text_fmt(label, "World! %d", i);
            },
            1,
            NULL
    );
}


void init_power_io() {
    // 拉高48脚
    esp_rom_gpio_pad_pullup_only(PIN_POWER_ON);
// 定义 GPIO 配置结构体
    gpio_config_t io_conf = {
            .pin_bit_mask = 1ULL << PIN_POWER_ON,
            .mode = GPIO_MODE_OUTPUT,
            .pull_up_en = GPIO_PULLUP_ENABLE,
            .pull_down_en = GPIO_PULLDOWN_DISABLE,
            .intr_type = GPIO_INTR_DISABLE,
    };

    // 应用配置
    gpio_config(&io_conf);
    // 设置 GPIO3 为高电平
    gpio_set_level((gpio_num_t) PIN_POWER_ON, 1);
}

void app_main_cpu1(void *) {
    init_power_io();
    lcd_init(); // 初始化 LCD
    lcd_set_backlight(255); // 设置背光亮度为 50%
    app_lvgl();

    int i = 0;
    show_dome(i);
    lv_timer_handler();

//    typedef struct {
//        uint32_t timeout_ms;        /**< TWDT timeout duration in milliseconds */
//        uint32_t idle_core_mask;    /**< Bitmask of the core whose idle task should be subscribed on initialization where 1 << i means that core i's idle task will be monitored by the TWDT */
//        bool trigger_panic;         /**< Trigger panic when timeout occurs */
//    } esp_task_wdt_config_t;

    esp_task_wdt_config_t config = {
            .timeout_ms = 5000,  // 设置超时为5秒
            .idle_core_mask = (1 << 0),  // 监控CPU 0的空闲任务
            .trigger_panic = true,  // 超时后触发 panic
    };
    esp_task_wdt_init(&config);
    esp_task_wdt_add(NULL);  // 添加当前任务到看门狗中



    while (1) {
        // 打印系统时间
        printf("System time: %lld\n", esp_timer_get_time() / 1000LL);
        esp_task_wdt_print_triggered_tasks(NULL, NULL, NULL);  // 打印触发看门狗超时的任务信息
        esp_task_wdt_reset();
        lv_timer_handler();
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

#ifdef __cplusplus
extern "C" {
#endif

void app_main(void) {
    // 创建并绑定到 CPU 1 的任务
    xTaskCreatePinnedToCore(app_main_cpu1, "app_main_cpu1", 8192, NULL, 1, NULL, 1);

    // 如果有其他初始化代码需要运行在 CPU0，可以放在这里
    // 例如：初始化 WiFi，或者是日志系统等
}

#ifdef __cplusplus
}
#endif
