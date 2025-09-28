#ifndef GLOBAL_VARS_H
#define GLOBAL_VARS_H

//+----------------------------------------------------------------------------+
//+--- To enable features from the project uncomment directives below:
//#define Enable_OLED_MOTOR_messages
//#define Enable_ArduinoRelay  //Inverse Logic, because when input signal of Arduino Relay is 0 (Zero) the outpup is NO
//#define Enable_AHT10 //sensor de temperatura e umidade
//#define MOTOR "KitMotor"
//#define ENABLE_OLED //quando a placa tem display OLED
#define OTA_ENABLED 1 //quando desejar fazer download de firmware via wifi
#define DHCP_DISABLE 1 
#define DEBUG_SERIAL_ENABLE 1

//+----------------------------------------------------------------------------+
//+----------------------------------------------------------------------------+
//+--- If Enable_COMMON_ANODE_RGB is not defined, common  cathode is selected:
// #define Enable_COMMON_ANODE_RGB
//+----------------------------------------------------------------------------+
/* types for global vars */
//+--- MODBUS -------+
#include <LbBoard.h>  //Adicionar essa lib: Rascunho->Incluir Biblioteca->Adicionar Biblioteca.ZIP...  Modbus.zip
#include <ModbusIP_ESP8266.h>

//+--- Global ModbusIP object ---+
extern  LbBoard lbmb;
extern  ModbusIP mb1;

//+--- Register Type   	Register Number  	Register Size 	Permission
//+---    Coil				      1-9999				   1 bit			     R/W     ---+



//+--- Non-Volatile Memory Enable --------------+
#include <EEPROM.h>
#define EEPROM_SIZE 1  // Armazena a opção de protocolo atual - define the number of bytes you want to access

//+--- Alexa ---+
#include <fauxmoESP.h>
extern fauxmoESP fauxmo;


//+--- Main.cpp Global Vars ---+
//+- Menu Global Variables 
extern bool b_GoToMenu;
extern bool b_MenuFirstCall;
extern char by_menu_option;
extern unsigned int by_stored_protocol_option;
extern bool b_MenuHold; //Mantem o menu ativado até que uma escolha seja realizada ou detecte inatividade por 5 ou mais segundos
extern bool b_GoToLoop;
extern String s_mac;
//

//
extern String s_client_id;
extern bool b_mqtt_reconnected_message;
extern unsigned int ui_ms_button_released;
extern int i_mqtt_reconnected_display_seconds;
extern bool IsPushButtonReleased(unsigned int ms);
extern bool IsPushButtonPressed(unsigned int ms);

//+--- Arduino Json ---+
#include <ArduinoJson.h>
extern StaticJsonDocument<200> frame;


//+--- Menu.cpp Global Vars ---+
extern void Protocol_Menu();
extern void ButtomMenuCheck();
//+--- PUSH BUTTON ---+
extern unsigned long ul_PushButton_PrevTime;
extern bool b_Pressed_Block;
extern bool b_Released_Block;
extern unsigned int ui_ms_button_pressed;

//+--- PROTOCOL MENU OPTIONS ---+
enum Protocols
{
MODBUS_PROTOCOL_LinkBox = 0x00, // MODBUS PROTOCOL
MODBUS_PROTOCOL_1C205 = 0x01, // MODBUS PROTOCOL
MQTT_PROTOCOL   = 0x02, // MQTT PROTOCOL
OPC_UA_PROTOCOL  = 0x03, // OPC UA PROTOCOL
ALEXA_PROTOCOL  = 0X04, // ALEXA
};


/***************  Telnet ********************************/
//extern WiFiServer TelnetServer(23); // Telnet Server Declaration port 23
//extern WiFiClient SerialOTA;        // Telnet Client Declaration 
//extern bool haveClient;     //client detection flag
//+------------------------------------------------------+/

//+----------------------------+
//+--- MQTT -------------------+
//+----------------------------+ 
#include <PubSubClient.h>
//Global Object
extern PubSubClient client;

//Global Variables
extern bool b_MqttStatus;
extern String s_mqtt_broker_IP; 
extern uint8_t u_thisDeviceId; //Client/Slave Device Number (usado na comunicação MQTT para diferenciar dispositivos)

//+----------------------------+
//+--------- OLED -------------+
//+----------------------------+
#ifdef ENABLE_OLED
extern char s_oled_header[40];
#endif

//+----------------------------+
//+--------- AHT10 ------------+
//+----------------------------+
//+--- Sensor AHT10 ---+
#include <Adafruit_AHTX0.h>

//#ifdef Enable_AHT10
//#include <AHT10.h>
//int ahtValue;  //to store T/RH result
//AHTxx aht10(AHTXX_ADDRESS_X38, AHT1x_SENSOR); //sensor address, sensor type
//Adafruit_AHTX0 aht10; //sensor address, sensor type
//#endif
//extern bool b_AHT10_INIT_OK;
//extern Adafruit_AHTX0 aht10; //sensor address, sensor type





#endif