#include "bk_gpio.h"
#include <driver/gpio.h>
#include <driver/hal/hal_gpio_types.h>
#include "gpio_driver.h"
#include "bk_saradc.h"
#include <driver/adc.h>
#include "adc_statis.h"
#include <os/os.h>
#include "hal_button.h"

#define TAG     "HAL_BUTTON"
#define debug_printf(...)       BK_LOGW(TAG,__func__,##__VA_ARGS__)
#define ADC_KEY_GPIO_ID   GPIO_28
#define ADC_KEY_SADC_CHAN_ID   4
#define MODEL_TOTAL     4


beken_thread_t button_h;
beken_semaphore_t button_semaphore = NULL;
hal_button_cb hal_button_ptr = {NULL};
static uint8_t model_count = 0;

static void hal_button_init(hal_button_isr next_cb,hal_button_isr pre_cb){
	adc_config_t config = {0};
	config.chan = ADC_KEY_SADC_CHAN_ID;
	config.adc_mode = ADC_CONTINUOUS_MODE;
	config.src_clk = ADC_SCLK_XTAL_26M;
	config.clk = 203125;
	config.saturate_mode = 3;
	config.steady_ctrl= 7;
	config.adc_filter = 0;
	config.sample_rate = 0;
	BK_LOG_ON_ERR(bk_adc_set_config(&config));

    hal_button_ptr.next_cb = next_cb;
    hal_button_ptr.prev_cb = pre_cb;

}

static void button_next_model(void){
    model_count++;
    model_count %= MODEL_TOTAL;
    bk_printf("button_next_model\r\n");
}

static void button_pre_model(void){
    if(model_count == 1){
        model_count = MODEL_TOTAL;
        return ;
    }
     bk_printf("button_pre_model\r\n");
    model_count--;
}

static uint8_t cur_model(void){

    return model_count;
}

static void hal_button_thread_handle(void){
    uint8_t adc_value = 0;

    while (1){
        /* code */
        adc_value = hal_adc_get_value();
        if(adc_value >= 0 && adc_value < 100){
            if(hal_button_ptr.next_cb != NULL){
                hal_button_ptr.next_cb();
            }
        }else{
            if(hal_button_ptr.prev_cb != NULL){
                hal_button_ptr.prev_cb();
            }
        }
        
    }
    
}

static uint16_t hal_adc_get_value(void){
	uint16_t value = 0;
	float cali_value = 0;

	if(bk_adc_set_channel(ADC_KEY_SADC_CHAN_ID))
		return 9999;

	bk_adc_read(&value, 100);
	cali_value = saradc_calculate(value);
	value = cali_value * 1000;

    return value;
}
//model 1
static void dvpCarmer_displayLcd(void){

}
//model 2
static void dvpCarmer_displayApp(void){

}
//model 3
static void uvcCarmer__dispalyMuti(void){

}
//model 4
static void display_lvgl(void){

}

hal_button_t button_ctr_model[] = {
    {"model1",   dvpCarmer_displayLcd}

    {"model2",   dvpCarmer_displayApp}

    {"model3",   uvcCarmer__dispalyMuti}

    {"model4",   display_lvgl}
};

void hal_button_thread(void){
    hal_button_init(button_next_model,button_pre_model);

    // 1.create sem
    bk_err_t err = rtos_init_semaphore(&button_semaphore,1);
    if(err != kNoErr){
        debug_printf("kNoErr,line_:(%d)\r\n", __LINE__);
    }

    // 2.create task
    bk_err_t err = rtos_create_thread(&button_h,BUTTON_H_PRIORITY,NULL,hal_button_thread_handle,BUTTON_H_SIZE,NULL);
    if(err != kNoErr){
        debug_printf("kNoErr,line_:(%d)\r\n", __LINE__);
    }

}