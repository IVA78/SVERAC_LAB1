/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
//#include "led_strip.h"
#include "sdkconfig.h"
#include "CLed.h"
#include "CButton.h"

static const char *TAG = "MAIN"; //Tekstualna oznaka koja identificira log poruke iz ovog fajla, npr. ESP_LOGI(TAG, "Start MAIN."); -> MAIN: Start MAIN. 

/* Use project configuration menu (idf.py menuconfig) to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
#define BLINK_GPIO 2 //CONFIG_BLINK_GPIO: Postavlja GPIO 2 kao izlaz za LED-icu.
#define BUTTON_GPIO 14
//
//Task Loop
//
void task_loop (void *parameters) //prima pokazivač na CLed objekt kao argument
{
    ESP_LOGI(TAG, "Start TASK Loop.");
    
    CLed *led;
    //Cast parameter
    led = (CLed*)parameters; //Pokazivač led se postavlja na ulazni parametar (downcast s void* na CLed*).
    ESP_LOGI(TAG, "Get Led pointer.");
    
    while(1) {
        //Do tick
        led->tick();  
        vTaskDelay(100 / portTICK_PERIOD_MS);//pauzira zadatak na određeno vrijeme  
                                             //vTaskDelay() je FreeRTOS funkcija definirana u biblioteci "freertos/task.h"
        /*
        Funkcija vTaskDelay() koristi Tick broj, a ne milisekunde!
        ESP32 FreeRTOS koristi sistemsku vremensku jedinicu zvanu "tick".
        Tick rate (brzina tikanja) definiran je u FreeRTOS konfiguraciji (menuconfig ili sdkconfig.h).
        ESP-IDF definira portTICK_PERIOD_MS, koji predstavlja koliko milisekundi traje jedan tick.
        U ovom slučaju, zadatak se pauzira 10 tick-ova (100 / 10)

        U sdkconfig.h definirano je #define CONFIG_FREERTOS_HZ 100 -> tick rate 100 Hz (1 tick = 10 ms).
        */
    }
}

// Callback funkcije
void onSingleClick() {
    ESP_LOGI(TAG, "Single Click Detected!");
}

void onDoubleClick() {
    ESP_LOGI(TAG, "Double Click Detected!");
}

void onLongPress() {
    ESP_LOGI(TAG, "Long Press Detected!");
}

void buttonTaskLoop(void *parameters) {
    ESP_LOGI(TAG, "Start BUTTON task Loop.");

    CButton *button;
    button = (CButton*)parameters;
    ESP_LOGI(TAG, "Get BUTTON pointer.");
    while(1) {
        button->tick();
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }

}

TaskHandle_t xHandle = NULL; //TaskHandle_t je tip podataka u FreeRTOS-u koji predstavlja pokazivač na kreirani zadatak (task)
                             //Koristi se za referenciranje i upravljanje zadacima: zapravo je pokazivač na internu strukturu FreeRTOS-a koji upravlja zadacima
                             //TaskHandle_t definiran je u FreeRTOS headeru i koristi se kada trebaš stvoriti, pauzirati, brisati ili mijenjati prioritet zadatka.



//ESP32 mian function
extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "Start MAIN.");
    
    //Create CLed object
    CLed led1(BLINK_GPIO); //kreiranje Cled objekta - instanciranje klase

    led1.setLedState(LedStatus::BLINK); //postavljanje statusa LED-ice

    //Create Task
    
    ESP_LOGI(TAG, "Start Task Create.");
    xTaskCreate(task_loop,      //Task function
                "ledLoop",      //Name of task in task scheduler
                1024*5,         //Stack size
                (void*)&led1,   //Parameter send to function
                1,              //Priority
                &xHandle);      //task handler 
    ESP_LOGI(TAG, "Task Created."); 
    /*
    xTaskCreate() stvara novi zadatak (task_loop) koji se izvodi paralelno s app_main()
    Parametri:
        task_loop → Funkcija koju će zadatak izvršavati.
        "ledLoop" → Naziv zadatka.
        1024*5 → Memorijski prostor za zadatak (5 KB stack) -> nema pravila kako se određuje, ako se program ruši, povećaj dok ne radi
        (void*)&led1 → Pokazivač na CLed objekt (led1), proslijeđen funkciji task_loop().
        1 → Prioritet zadatka (veća vrijednost = veći prioritet).
        &xHandle → Spremanje Task Handle-a za kasnije upravljanje.
    */
    
        /*
            //Main loop
    while(1) {
        led1.setLedState(LedStatus::BLINK);
        vTaskDelay(10000 / portTICK_PERIOD_MS); //10000ms -> 10s

        led1.setLedState(LedStatus::FAST_BLINK);
        vTaskDelay(10000 / portTICK_PERIOD_MS);

        led1.setLedState(LedStatus::SLOW_BLINK);
        vTaskDelay(10000 / portTICK_PERIOD_MS);
    }
        */

    //povezivanje callback funkcija za button
    CButton button1(BUTTON_GPIO);
    button1.attachSingleClick(onSingleClick);
    button1.attachDoubleClick(onDoubleClick);
    button1.attachLongPress(onLongPress);

    ESP_LOGI(TAG, "Start Button Task Create.");
    xTaskCreate(buttonTaskLoop,      //Task function
                "buttonLoop",      //Name of task in task scheduler
                1024*5,         //Stack size
                (void*)&button1,   //Parameter send to function
                2,              //Priority
                &xHandle);      //task handler 
    ESP_LOGI(TAG, "Button Task Created.");

    while(1) {
        
    }

}
