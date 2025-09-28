#include <Arduino.h>
#include "bsp_esteira.h"
#include "global_vars.h"
#include <WiFi.h>

#define ESP32_RIO_NUM_IO_CHANNELS 10
#define ESP32_RIO_NUM_IO_CHANNELS_TOTAL ESP32_RIO_NUM_IO_CHANNELS+ESP32_RIO_NUM_IO_CHANNELS

//GPIO PINS
static const uint8_t DI_Pin[ESP32_RIO_NUM_IO_CHANNELS]     = {4, 5, 6, 7, 15, 16, 17, 9, 8, 18};
static const uint8_t DQ0_Pin[ESP32_RIO_NUM_IO_CHANNELS]    = {10, 12, 14, 47, 39, 40, 41, 42, 2, 1};
static const uint8_t DQ1_Pin[ESP32_RIO_NUM_IO_CHANNELS]    = {46, 11, 13, 21, 48, 45, 35, 36, 37, 38};

#define STATUS_LED      43  //IO43 (TXD0)
#define OE_TOGGLE_BTN   3   //IO3

/*
 Modbus parameters declaring modbus address space for each modbus register type (coils, discrete inputs, holding registers, input registers)
 
 Coils bank 0:
 Address    Assignment
 0          DQ00
 1          DQ01
 2          DQ02
 3          DQ03
 4          DQ04
 5          DQ05
 6          DQ06
 7          DQ07
 8          DQ08
 9          DQ09
 
 Coils bank 1:
 Address    Assignment
 10         DQ10
 11         DQ11
 12         DQ12
 13         DQ13
 14         DQ14
 15         DQ15
 16         DQ16
 17         DQ17
 18         DQ18
 19         DQ19
 20         Enable_Output
 
 Discrete Inputs:
 Address    Assignment
 0          DI0
 1          DI1
 2          DI2
 3          DI3
 4          DI4
 5          DI5
 6          DI6
 7          DI7
 8          DI8
 9          DI9
 10         Output_Enabled
 */
//esta area eh somente onde estao os "enderecos modbus" 
static const uint8_t MB_DI[ESP32_RIO_NUM_IO_CHANNELS]  = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
static const uint8_t MB_DQ0[ESP32_RIO_NUM_IO_CHANNELS] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
static const uint8_t MB_DQ1[ESP32_RIO_NUM_IO_CHANNELS] = {10,11,12,13,14,15,16,17,18,19};
#define MB_ENABLE_OUTPUT 20
#define MB_OUTPUT_ENABLED 10
uint8_t lastMB_DQ0[ESP32_RIO_NUM_IO_CHANNELS] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
uint8_t lastMB_DQ1[ESP32_RIO_NUM_IO_CHANNELS] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

//esta eh a area onde vai ser colocada os dados vindos do modbus (usado pelo DISPLAY_OLED)
#if 0
word coils_offset[ESP32_RIO_NUM_IO_CHANNELS_TOTAL];
word digital_input_offset[ESP32_RIO_NUM_IO_CHANNELS];
word input_regs_offset[ESP32_RIO_NUM_IO_CHANNELS];
word holding_regs_offset[ESP32_RIO_NUM_IO_CHANNELS];
#endif

char ca_line1[40]="";  //"IP:xxx.xxx.xxx.xxx"
char ca_line2[18]="";  //
char ca_line3[18]="";  //

int  x=0;
int  minX=0;
int  i_nModbusMessages=0;   // Number of Modbus messages


bool b_laststatoutput=0;  

byte gucframe[10];  //Modbus Frame Header received for the Modbus Routine. Only to Animate the OLED display
uint32_t reqcount;    
uint32_t lastreqcount=0;     

static bool hw_outputs_enabled = false;
static bool mb_outputs_enabled = false;
static bool outputs_enabled = false;
volatile bool buttonPressed = false;  

/*
 Disable all digital outputs
*/
void esp32_rio_disable_outputs(void) {
    for (int i = 0; i < ESP32_RIO_NUM_IO_CHANNELS; i++) {
        digitalWrite(DQ0_Pin[i], LOW);
        digitalWrite(DQ1_Pin[i], LOW);
    }
}

/*
 Turn status LED on/off
*/

void esp32_rio_turn_status_led_on(void) {
    digitalWrite(STATUS_LED, HIGH);
}


void esp32_rio_turn_status_led_off(void) {
    digitalWrite(STATUS_LED, LOW);
}


/*
 Turn on/off a given digital output of a given output bank
*/
void esp32_rio_turn_output_on(unsigned int bank_number, unsigned int output_number) {
    if (bank_number == 0) {
        digitalWrite(DQ0_Pin[output_number], HIGH);
    } else if (bank_number == 1) {
        digitalWrite(DQ1_Pin[output_number], HIGH);
    }
}
void esp32_rio_turn_output_off(unsigned int bank_number, unsigned int output_number) {
    if (bank_number == 0) {
        digitalWrite(DQ0_Pin[output_number], LOW);
    } else if (bank_number == 1) {
        digitalWrite(DQ1_Pin[output_number], LOW);
    }
}

// ISR (função de interrupção)
void IRAM_ATTR handleButtonInterrupt() {
  buttonPressed = true;   // marca que o botão foi pressionado
}

void configure_gpio(void)
{
    int i;
    for (i = 0; i < ESP32_RIO_NUM_IO_CHANNELS; ++i) {
        pinMode(DI_Pin[i], INPUT);
        pinMode(DQ0_Pin[i], OUTPUT);
        pinMode(DQ1_Pin[i], OUTPUT);
    }

    pinMode(STATUS_LED, OUTPUT);
    
    //LED off, outputs disabled by default
    esp32_rio_turn_status_led_off();

    //disable all the outputs 
    esp32_rio_disable_outputs();

    pinMode(OE_TOGGLE_BTN, INPUT);
    attachInterrupt(digitalPinToInterrupt(OE_TOGGLE_BTN), handleButtonInterrupt, FALLING);

}


/*
 Query if Enable Button is pressed then Enable de Outputs
 Esta funcao é necessaria pois a saida da esteira pode ser acionada ou pela borneira (CLP) ou por este modulo
 Neste caso, se o botao esta pressionado as saidas estao habilitadas e neste caso a prioridade eh deste modulo
*/

static void on_oe_button_toggle(void) 
{
    outputs_enabled = !outputs_enabled;
}


void check_output_enable(void)
{
    hw_outputs_enabled = outputs_enabled | mb_outputs_enabled;

    if (hw_outputs_enabled){
        esp32_rio_turn_status_led_on(); 
#if DEBUG_SERIAL_ENABLE
    //Serial.println("Digital outputs enabled.");
#endif        
    }
    else{
        esp32_rio_disable_outputs();
        esp32_rio_turn_status_led_off(); 
#if DEBUG_SERIAL_ENABLE
    //Serial.println("Digital outputs disabled.");
#endif        
    }


}


/***************************************************************************************** 
  treat the board 
******************************************************************************************/


LbBoard::LbBoard() {
    _regs_head = 0;
    _regs_last = 0;
}
/*
LBTRegister* LbBoard::searchRegister(word address) {
    LBTRegister *reg = _regs_head;
    //if there is no register configured, bail
    if(reg == 0) return(0);
    //scan through the linked list until the end of the list or the register is found.
    //return the pointer.
    do {
        if (reg->address == address) return(reg);
        reg = reg->next;
	} while(reg);
	return(0);
}
*/
//+--- dos #defines dos dispositivos Modbus do arquivo principal LinkBox.ino para essa biblioteca Modbus.h
//esta area eh utilizada para o tratamento do DISPLAY_OLED
#if 0
void LbBoard::Set_CoilsOffset(int device, word offset)
{ 
    coils_offset[device]=offset;
}

void LbBoard::Set_DigitalInputOffset(int device, word offset)
{ 
    digital_input_offset[device]=offset;
}

void LbBoard::Set_InputRegsOffset(int device, word offset)
{ 
    input_regs_offset[device]=offset;
}

void LbBoard::Set_HoldingRegsOffset(int device, word offset)
{ 
    holding_regs_offset[device]=offset;
}
#endif
//+---  LbBoard::ModbusInit()
//+---  Initiate the Modbus Points in the respective area modbus. 
//+---  Important!!!! The modbus area and scan is used ether to MQTT and Alexa    

void LbBoard::ModbusInit()
{
    int i,j;
    
    #if 0
    //endereco modbus 0..9
    for (i = 0; i < ESP32_RIO_NUM_IO_CHANNELS; ++i) {
       Set_CoilsOffset(i, MB_DQ0[i]);
    }

    //Modbus address - 10..19
    j = 0;
    for (i=ESP32_RIO_NUM_IO_CHANNELS; i< ESP32_RIO_NUM_IO_CHANNELS+ESP32_RIO_NUM_IO_CHANNELS; i++){
       Set_CoilsOffset(i, MB_DQ1[j]);
       j++;
    }

    //Modbus address - Enable_Output
    Set_CoilsOffset(i, MB_ENABLE_OUTPUT);
    #endif

    //+-- Add binary Modbus Registers (bit=COIL) ---+
    for (i=0; i< ESP32_RIO_NUM_IO_CHANNELS; i++){
       mb1.addCoil(MB_DQ0[i]);
       mb1.Coil(MB_DQ0[i], false); 
    }    

    for (i=0; i< ESP32_RIO_NUM_IO_CHANNELS; i++){
       mb1.addCoil(MB_DQ1[i]);
       mb1.Coil(MB_DQ1[i], false); 
    }    

    mb1.addCoil(MB_ENABLE_OUTPUT);
    b_laststatoutput=0;

    //+-- Add binary Modbus Registers (bit=INPUT STATUS) ---+
    for (i=0; i< ESP32_RIO_NUM_IO_CHANNELS; i++){
       mb1.addIsts(MB_DI[i]);
    }    

    mb1.addIsts(MB_OUTPUT_ENABLED);
    
    //initialize modbus connection
    mb1.server();
}
 
void LbBoard::Task()
{
    bool bAux;
    int i;

    //Verifica se o botao foi pressionado, neste caso aciona as saidas
    if (buttonPressed){
        buttonPressed = false;
        outputs_enabled = !outputs_enabled;
    }

    //somente atualiza as saidas se botao das saidas estiver habilitado 
    if (hw_outputs_enabled) {

        //+-- Discrete Outputs     1-999		     1 bit		       R/W     ---+
        for (i=0; i < ESP32_RIO_NUM_IO_CHANNELS; i++){

            //trata banco 0
            if (mb1.Coil(MB_DQ0[i]))
               esp32_rio_turn_output_on(0, i);
            else
               esp32_rio_turn_output_off(0, i);

#if DEBUG_SERIAL_ENABLE               
            //digitalWrite(DQ0_Pin[i], (mb1.Coil(MB_DQ0[i])));
            if (mb1.Coil(MB_DQ0[i]) && (lastMB_DQ0[i] == 0)) {
                lastMB_DQ0[i] = mb1.Coil(MB_DQ0[i]);
                Serial.print("ligou DQ0_");
                Serial.println(i);
            } 
#endif
            //trata banco 1
           if (mb1.Coil(MB_DQ1[i]))
                esp32_rio_turn_output_on(1, i);
            else 
                esp32_rio_turn_output_off(1, i);
                
#if DEBUG_SERIAL_ENABLE            
            //digitalWrite(DQ1_Pin[i], (mb1.Coil(MB_DQ1[i])));
            if (mb1.Coil(MB_DQ1[i]) && (lastMB_DQ1[i] == 0)) {
                    lastMB_DQ1[i] = mb1.Coil(MB_DQ1[i]);
                    Serial.print("ligou DQ1_");
                    Serial.println(i);
            }          
#endif       
        }

    }

    mb_outputs_enabled = mb1.Coil(MB_ENABLE_OUTPUT);
    
    check_output_enable();

    //+-- Discrete Inputs     10001-19999		   1 bit		  Read Only  ---+
    for (i=0; i< (ESP32_RIO_NUM_IO_CHANNELS); i++){
       mb1.Ists(MB_DI[i],digitalRead(DI_Pin[i]));
    }
    
    //verifica status das saidas (mesmo status do LED)
    mb1.Ists(MB_OUTPUT_ENABLED,outputs_enabled);


} 