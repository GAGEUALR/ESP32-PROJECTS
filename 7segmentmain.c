/*
EARLY VERSION of simple 7-segment button-interrupt project.
This version is only to make sure code works with an LED

In final version, button gets pressed, adds 1 to the number displayed on the seven segment.
Number resets to 0 at 10, initially set as 0. 

Using 74LS42N 7-segment driver. Using KYX-3101BS display.
*/

#include <stdio.h>
#include "sdkconfig.h"
#include "esp_chip_info.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "esp_intr_alloc.h"
#include "esp_mac.h"


void IRAM_ATTR ISR(void* arg){

    gpio_set_level(GPIO_NUM_22, 1);                             //set LED high

}

void configureInterrupt(){

    esp_intr_dump(stdout);                                      //show interrupt configuration

    gpio_install_isr_service(ESP_INTR_FLAG_LEVEL1);             

    gpio_set_intr_type(GPIO_NUM_17, GPIO_INTR_NEGEDGE);         //trigger on negative edge

    gpio_isr_handler_add(GPIO_NUM_17, ISR, NULL);               //add isr 

    printf("Interrupt configure successful\n");

}

void configureGPIO(){


    gpio_set_direction(GPIO_NUM_17, GPIO_MODE_INPUT);        //configure button gpio pin
    gpio_set_pull_mode(GPIO_NUM_17, GPIO_PULLUP_ONLY);
    gpio_set_level(GPIO_NUM_22, 0);

    gpio_set_direction(GPIO_NUM_22, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_NUM_22, 0);

    gpio_dump_io_configuration(stdout, (1ULL << 22) | (1ULL << 17));        //show gpio configuration on monitor terminal

    printf("GPIO configure successful\n");

}


void app_main(void)
{
    configureGPIO();                            //run setup functions
    configureInterrupt();

    while (1) {

       vTaskDelay(5000 / portTICK_PERIOD_MS);   //wait 5 seconds and reset LED

       gpio_set_level(GPIO_NUM_22, 0);

    }
}
