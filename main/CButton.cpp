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
        
        if((pressTime - lastPressTime) < DOUBLE_CLICK) { //detekcija DOUBLE CLICK-a
            doubleClickDetected = true;
            if (doubleClick) doubleClick();
        }

        lastPressTime = pressTime;
    }

    //ako je tipka još pritisnuta, provjeravam je li u pitanju LONG PRESS
    if(buttonState == LOW && !longPressDetected) { 
        int64_t nowTime = esp_timer_get_time();
        if((nowTime - pressTime) > LONG_PRESS) {
            longPressDetected = true;
            if (longPress) longPress();
        }

    }

    
    // detekcija otpuštanja buttona - RESET
    if (buttonState == HIGH && lastButtonState == LOW) { // ako je button trenutno HIGH, a prije toga je bio u LOW, znači da je button otpušten
        if (!longPressDetected && !doubleClickDetected) { //detekcija SINGLE CLICK-a
            if(singleClick) singleClick(); 
        }

        longPressDetected = false;
        doubleClickDetected = false;
    }

    lastButtonState = buttonState; // Ažuriranje stanja tipke
}
