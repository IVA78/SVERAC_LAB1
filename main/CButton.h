// CButton.h

#ifndef _CButton_h
#define _CButton_h

#define DOUBLE_CLICK 400000 //400ms
#define LONG_PRESS 1000000 //1s

#define LOW 0
#define HIGH 1

 
// Pointer to event handling methods
extern "C" {
    typedef void (*ButtonEventHandler)(void);
}
// void my_singeClick_function(){}

/*
Ovo definira tip pokazivača na funkciju (ButtonEventHandler) koja:
    Ne prima argumente (void)
    Ne vraća ništa (void)

ButtonEventHandler je tip podataka koji može pohraniti adresu bilo koje funkcije s ovakvim potpisom:
    void mojaFunkcija(void) {
        // Nešto se događa kad se pritisne tipka
    }
*/

class CButton{
    public:
        CButton(int port);
        void attachSingleClick(ButtonEventHandler method){singleClick = method;}; //postavljanje funkcije za singleClick: poziva se kad se dogodi single click
        void attachDoubleClick(ButtonEventHandler method){doubleClick = method;}; //postavljanje funkcije za doubleClick: poziva se kad se dogodi double click
        void attachLongPress(ButtonEventHandler method){longPress = method;}; //postavljanje funkcije za longPress: poziva se kad se dogodi long press

        void tick();

    private:

        gpio_num_t m_pinNumber; //GPIO pin na koji je spojen button
        bool lastButtonState = HIGH; //posljednje stanje button-a: inicijalno HIGH zbog ugrađenog pull-up otpornika
        int64_t pressTime = 0; //vrijeme kada je tipka pritisnuta
        int64_t lastPressTime = 0; //vrijeme kada je tipka pritisnuta zadnji put
        int64_t releaseTime = 0; //vrijeme kada je tipka puštena
        bool longPressDetected = false; //dogodio se long press
        bool doubleClickDetected = false;

        ButtonEventHandler singleClick = NULL; //pokazivač na dodijeljenu funkciju oblika void functionName(void)
        ButtonEventHandler doubleClick = NULL; //pokazivač na dodijeljenu funkciju oblika void functionName(void)
        ButtonEventHandler longPress = NULL; //pokazivač na dodijeljenu funkciju oblika void functionName(void)
};


#endif