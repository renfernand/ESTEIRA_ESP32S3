
#if 0
//+--- Register Type   	Register Number  	Register Size 	Permission
//+-- Discrete Inputs     10001-19999			   1 bit			     R/W     ---+
#define di_RETN_BUTTON_OFFSET  0  // Offset do Discrete Input para acionar o botão retentivo vermelho (bit=Discrete Input) 
#define di_PUSH_BUTTON_OFFSET  1  // Offset do Discrete Input para acionar o botão não retentivo preto (bit=Discrete Input)

//+--- Register Type   	Register Number  	Register Size 	Permission
//+--- Input Register		  30001-39999			   16 bit	          R     ---+
#define ir_POT_LEFT_OFFSET   0    // Offset do Input Register Modbus (2 bytes) ADC1_3 GPIO39
#define ir_POT_RIGHT_OFFSET  1    // Offset do Input Register Modbus (2 bytes) ADC1_6 GPIO34

//+--- Register Type   	Register Number  	Register Size 	Permission
//+--- Input Register		  30001-39999			   16 bit	          R     ---+
#define ir_TEMPTURE_OFFSET  0    // Offset do Input Register Modbus (2 bytes) AHT10 TEMPTURE
#define ir_HUMIDITY_OFFSET  1    // Offset do Input Register Modbus (2 bytes) AHT10 HUMIDITY

//+--- Register Type   	Register Number  	Register Size 	Permission
//+- Holding Register		  40001-49999			  16 bit	          R     ---+
#define hr_RGB_R_OFFSET  0    // Offset do Holding Register Modbus (2 bytes) PWM GPIO15
#define hr_RGB_G_OFFSET  1    // Offset do Holding Register Modbus (2 bytes) PWM' GPIO2
#define hr_RGB_B_OFFSET  2    // Offset do Holding Register Modbus (2 bytes) PWM GPIO0

#endif
/*************** POTENTIOMETERS GPIO define ********************************/
//+--- pino usado ---+
#define def_pin_POT_LEFT  39 //GPIO39
#define def_pin_POT_RIGHT 34 //GPIO34

/*************** LEDs GPIO define ********************************/
//+--- pino usado ---+
#define def_pin_LED_GREEN 16 //GPIO16
#define def_pin_LED_RED 4    //GPIO04

/*************** PWM GPIO define ****************************************/
#define def_rgb_pin_red 15
#define def_rgb_red_channel 0
//
#define def_rgb_pin_blue 0
#define def_rgb_blue_channel 1
//
#define def_rgb_pin_green 2
#define def_rgb_green_channel 2
//
#define def_pwm_frequency 5000

#define def_pwm_resolution 10

/*************** BUTTONS GPIO define ****************************************/
#define def_pin_RETN_BUTTON 18  //GPIO5
#define def_pin_PUSH_BUTTON 5 //GPIO18


