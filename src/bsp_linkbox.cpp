#include "arduino.h"
#include "bsp_linkbox.h"

#ifdef Linkbox_Enable

void configure_gpio(void)
{

    //+----------------------------------------------------------------------------------+
    //+ Configure the Green and Red LED pins
    //
    pinMode(def_pin_LED_RED, OUTPUT);
    digitalWrite(def_pin_LED_RED, HIGH);
    //+----------------------------------------------------------------------------------+
    //+ Configures retentive and push buttons pins as input
    pinMode(def_pin_RETN_BUTTON, INPUT);
    pinMode(def_pin_PUSH_BUTTON, INPUT);
    //+----------------------------------------------------------------------------------+

    /*************** OLED *********************/
    lbmb.OledInitialize(); // initialize OLED display with I2C address 0x3C


    /*************** initialize EEPROM with predefined size *********************/
    EEPROM.begin(EEPROM_SIZE);

    //+--- EEPROM.write(address, value)
    //EEPROM.write(0, 1);  EEPROM.commit();  //Initializes the menu with the zero option       

    //by_stored_protocol_option = EEPROM.read(0);  //protocolo selecionado
    by_stored_protocol_option = EEPROM.read(0);  //protocolo selecionado


       // PWM Config
    //ledcSetup(def_rgb_red_channel, def_pwm_frequency, def_pwm_resolution);
    //ledcSetup(def_rgb_blue_channel, def_pwm_frequency, def_pwm_resolution);
    //ledcSetup(def_rgb_green_channel, def_pwm_frequency, def_pwm_resolution);
      
    //Associating the GPIO with the chosen channel
    //ledcAttachPin(def_rgb_pin_red, def_rgb_red_channel);
    //ledcAttachPin(def_rgb_pin_blue, def_rgb_blue_channel);
    //ledcAttachPin(def_rgb_pin_green, def_rgb_green_channel);

    //Turn de LEDs off
    //ledcWrite(def_rgb_red_channel, 1023);
    //ledcWrite(def_rgb_blue_channel, 1023);
    //ledcWrite(def_rgb_green_channel, 1023);
}

#endif  
