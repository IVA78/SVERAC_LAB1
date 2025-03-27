// CLed.h

#ifndef _CLed_h //ako header već nije uključen u datoteku, uključi ga
#define _CLed_h //sprječava duple deklaracije i pogreške u kompilaciji

#define BLINK_GPIO CONFIG_BLINK_GPIO //deklaracija makroa koji će se definirati kasnije u main programu
                                     //pomoću njih definira se GPIO port na koji se spaja ledica (output)

enum LedStatus{ OFF, ON, BLINK, FAST_BLINK, SLOW_BLINK}; //definiranje stanja ledice pomoću enuma


#define LED_BLINK_FAST   250000L //vremenski interval LED treptanja u mikrosekundama -> 0.25s
#define LED_BLINK       1000000L //1s
#define LED_BLINK_SLOW  2000000L //2s

class CLed{ 
    
    public: 
        CLed(int port); //konstruktor - prima broj GPIO pina na koji je spojena LED-ica
        void tick(); //metoda za ažuriranje stanja LED-ice
        void setLedState(LedStatus x); //postavljanje novog stanja LED-ice pomoću enum-a LedStatus
        //void kao povratni tip u obje fje jer ne vraćaju nikakvu vrijednost, već samo mijenjaju unutarnje stanje objekta    

    private:
        LedStatus m_state = OFF; //stanje LED-ice (inicijalno OFF)
        gpio_num_t m_pinNumber; //GPIO pin na koji je LED-ica spojena (driver/gpio.h)
        const char *LogName = "CLed"; //String za logiranje poruka, npr.  (12329) CLed: LED BLINK FAST - STATE[0]
        int64_t  m_lastBlink = 0; //pohranjuje vrijeme zadnjeg treptaja u mikrosekundama (stdint.h)
        int m_lastBlinkState = 0; //zadnje stanje LED-ice (inicijalno 0)
};
#endif