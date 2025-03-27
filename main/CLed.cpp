#include <stdio.h>// Standardna ulazno/izlazna biblioteka: Omogućava rad s printf, snprintf i drugim I/O funkcijama
#include "freertos/FreeRTOS.h"// FreeRTOS OS podrška: 	Podrška za multitasking na ESP32 (FreeRTOS OS)
#include "freertos/task.h"// FreeRTOS taskovi: Rad s taskovima (zadacima) u FreeRTOS-u
#include "driver/gpio.h"// GPIO driver za ESP32: GPIO driver za upravljanje pinovima ESP32
#include "esp_log.h"// ESP-IDF log sistem: ESP-IDF sistem za logiranje poruka
//#include "led_strip.h"
#include "sdkconfig.h"// SDK konfiguracijski fajl: Konfiguracijski fajl koji sadrži postavke projekta
#include "esp_timer.h"// ESP-IDF tajmer biblioteka: Tajmer biblioteka za mjerenje vremena u mikrosekundama
#include "CLed.h"// Deklaracija klase CLed

//
// CLED construction function - implementacija konstruktora
//
CLed::CLed(int port){
    //Config Port on constructor
    m_pinNumber = (gpio_num_t)port; //spremanje broja pina
    ESP_LOGI(LogName, "Configure port[%d] to output!!!", port);
    gpio_reset_pin(m_pinNumber); //postavljanje pina na početne postavke
    /* Set the GPIO as a output */
    gpio_set_direction(m_pinNumber, GPIO_MODE_OUTPUT);//postavljanje GPIO pina kao izlaz
                                                      //time osiguravamo da pin nema prethodne konfiguracije i da ga možemo koristiti
}

//
// Tick method, need to be called periodically
//
void CLed::tick(){
    
    switch(m_state){
        case OFF:
            //ESP_LOGI(LogName, "LED OFF.");
            gpio_set_level(m_pinNumber, 0); // LED ISKLJUČEN
            //vTaskDelay(1000 / portTICK_RATE_MS);
            break;
        case ON:
            //ESP_LOGI(LogName, "LED ON.");
            gpio_set_level(m_pinNumber, 1); // LED UKLJUČEN
            break;
        case BLINK:
            if((esp_timer_get_time() - m_lastBlink) > LED_BLINK){ //esp_timer_get_time() vraća trenutno vrijeme u mikrosekundama (µs) od pokretanja uređaja
                m_lastBlink = esp_timer_get_time();//m_lastBlink je varijabla koja čuva vrijeme (također u mikrosekundama) kada je LED-ica zadnji put promijenila svoje stanje
                m_lastBlinkState = (m_lastBlinkState == 0) ? 1 : 0;
                ESP_LOGI(LogName, "LED BLINK - STATE[%d]",m_lastBlinkState);
                gpio_set_level(m_pinNumber, m_lastBlinkState);
            };
            break;
        case SLOW_BLINK:
            if((esp_timer_get_time() - m_lastBlink) > LED_BLINK_SLOW){                
                m_lastBlink = esp_timer_get_time();
                m_lastBlinkState = (m_lastBlinkState == 0) ? 1 : 0;
                ESP_LOGI(LogName, "LED BLINK SLOW - STATE[%d]",m_lastBlinkState);
                gpio_set_level(m_pinNumber, m_lastBlinkState);
            };
            break;
        case FAST_BLINK:
            if((esp_timer_get_time() - m_lastBlink) > LED_BLINK_FAST){                
                m_lastBlink = esp_timer_get_time();
                m_lastBlinkState = (m_lastBlinkState == 0) ? 1 : 0;
                ESP_LOGI(LogName, "LED BLINK FAST - STATE[%d]",m_lastBlinkState);
                gpio_set_level(m_pinNumber, m_lastBlinkState);
            };
            break;
        default:
            //gpio_set_level(m_pinNumber, 1);
            break;
    }
}

//
// Set LED mode
//
void CLed::setLedState(LedStatus x){
    //const char str_tmp[50];
    //snprintf(str_tmp, 50, "Set LED state to %d", x);
    ESP_LOGI(LogName, "Set LED state %d", x);
    m_state = x;
          
}