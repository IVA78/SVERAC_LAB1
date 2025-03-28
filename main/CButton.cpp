#include <stdio.h>
#include "driver/gpio.h"
#include "esp_timer.h"
#include "CButton.h"


CButton::CButton(int port){

    //ToDo::
    //BOOT is GPIO0 (HIGH when released, LOW when pressed)

    m_pinNumber = (gpio_num_t)port; //dodjeljivanje porta na koji je spojen button
    gpio_set_direction((gpio_num_t)m_pinNumber, GPIO_MODE_INPUT);//postavljanje GPIO pina kao ulaz
    gpio_set_pull_mode((gpio_num_t)m_pinNumber, GPIO_PULLUP_ONLY); // Aktivira interni pull-up otpornik

}

void CButton::tick(){
    //ToDo

    bool buttonState = gpio_get_level((gpio_num_t)m_pinNumber); // dohvat trenutnog stanja buttona

    // detekcija pritiska buttona
    if (buttonState == LOW && lastButtonState == HIGH) { // ako je button trenutno LOW, a prije toga je bio HIGH, znači da je pritisnut (pull-up otpornik)
        pressTime = esp_timer_get_time(); // bilježenje vremena u kojem je pritisnut button
        longPressDetected = false; //nisam još detekirala LONG PRESS jer je gumb nanovo pritisnut
   
    }

    // detekcija otpuštanja buttona
    if (buttonState == HIGH && lastButtonState == LOW) { // ako je button trenutno HIGH, a prije toga je bio u LOW, znači da je button otpušten
        releaseTime = esp_timer_get_time(); //vrijeme otpuštanja buttona

        if(doubleClickDetected) {
            if(doubleClick) doubleClick();
            doubleClickDetected = false;
        } else {
            doubleClickDetected = true;
            longPressDetected = false;
        }

    }

    int64_t nowTime = esp_timer_get_time();
    if(buttonState == HIGH && doubleClickDetected && ((nowTime - releaseTime) > DOUBLE_CLICK_GAP)) {//ako tipka nije pritinusta i možda se dogodio double click
            if((releaseTime - pressTime) <  SIGNLE_CLICK) { //dodatna provjera zbog otpuštanja nakon LONG PRESS
                if(singleClick) singleClick();
            }
            doubleClickDetected = false;
            
    } else if(buttonState == LOW && !longPressDetected && ((nowTime - pressTime) > LONG_PRESS)) { //ako je tipka još pritisnuta, provjeravam je li u pitanju LONG PRESS
            if (longPress) longPress();
             longPressDetected = true;
             doubleClickDetected = false;
    
     }

    lastButtonState = buttonState; // Ažuriranje stanja tipke
}


