//+---------------------------------------------------------------------------------------------+
//|   Didatic KIT - LinkBox - Developed to support learning the following subjects:             |
//    Industrial Informatics II, Home Automation and Industrial Networks I of                   |
//    Automation and Control Engineering Course - Federal University of Uberlândia (FEELT/UFU)  |
//    by Prof. Fábio Vincenzi and Prof. Renato Carrijo                                          |
//                                                                 Updated: 2024/05/03          |         
//  
//+---------------------------------------------------------------------------------------------+
/***************  MODBUS ********************************/
// OBS: A biblioteca Modbus utiliza o padrao modbus novo (modbus.org) 
// desse modo, cada tipo de registrador (Coil, Discrete Inputs, Input Register e Holding Register)
// podem ser acrescentados a partir de 0 (zero) para o ScadaBR
// Todo a base modus é feita pelo projeto "Modbus Library for Arduino" de Andre Sarmento https://github.com/andresarmento/modbus-esp8266 
// The offsets for registers are 0-based. So be careful when setting your supervisory system or your testing software. For example, in ScadaBR (http://www.scadabr.com.br) offsets are 0-based, then, a register configured as 100 in the library is set to 100 in ScadaBR. On the other hand, in the CAS Modbus Scanner (http://www.chipkin.com/products/software/modbus-software/cas-modbus-scanner/) offsets are 1-based, so a register configured as 100 in library should be 101 in this software. 
//
//+--- Registrador de Offsets ---+
//Register Type   	Register Number  	Register Size 	Permission
//    Coil			    1-9999			    1 bit		   R/W
//Discrete Inputs     10001-19999			1 bit		   R/W
//Input Register	  30001-39999			16 bit		    R
//Holding Register	  40001-49999		    16 bit 	       R/W

//+---------------------------+                    //+----------------------------+
//+--- Function Code READ: ---+                    //+--- Function Code WRITE: ---+
//+---------------------------+                    //+----------------------------+                             
//01: Coils (FC=01)                                05: Single Coil (FC=05)
//02: Discrete Inputs (FC=02)                      06: Single Holding Register (FC=06)
//03: Multiple Holding Registers (FC=03)           0F: Multiple Coils (FC=15)
//04: Input Registers (FC=04)                      10: Multiple Holding Registers (FC=16)

//
//Modbus Jargon
//In this library was decided to use the terms used in Modbus to the methods names, then is important clarify the names of register types:
//------------------------------------------------------------------------+
//Register type	   |   Use as	         |  Access	    | Library methods
//------------------------------------------------------------------------+
//Coil	           |   Digital Output	 | Read/Write	  | addCoil(), Coil()
//Holding Register |	 Analog Output	 | Read/Write	  | addHreg(), Hreg()
//Discrete Inputs  |   Digital Input	 | Read Only	  | addIsts(), Ists()
//Input Register	 |   Analog Input	   | Read Only	  | addIreg(), Ireg()
//------------------------------------------------------------------------+
//
// New Features: 24/may/05 
// In the Modbus Protocol, to disable scan display of variables - write in the Coil address 9000 the value 1. This feature is to increase the response from the board to Modbus Commands. 



#include "global_vars.h"
#include <WiFi.h>
#include <WiFi_CONN.h>
#include <Alexa.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <Arduino.h>
#include <ArduinoOTA.h>
#include <ModbusIP_ESP8266.h>
#include "CONFIG.h"
#include "bsp_esteira.h"

//+--- MQTT LIBs ---+
#include <PubSubClient.h>
#include "MQTT.h"
//#include "SENSOR.h"

//PROTOCOLS
String s_client_id="";

//+--- Modbus objects ---+
LbBoard lbmb;
ModbusIP mb1;

//seleciona a rede wifi
// MODBUS_PROTOCOL_LinkBox ou  MODBUS_PROTOCOL_1C205
unsigned int by_stored_protocol_option=MODBUS_PROTOCOL_LinkBox;

uint8_t u_thisDeviceId=0; //Client/Slave Device Number (usado na comunicação MQTT para diferenciar dispositivos)


 /***************  Telnet ********************************/
#ifdef OTA_ENABLED   
WiFiServer TelnetServer(23); // Telnet Server Declaration port 23
WiFiClient SerialOTA;        // Telnet Client Declaration 
bool haveClient = false;     //client detection flag
#endif
//+------------------------------------------------------+/

//+----------------------------+
//+--- Temporização -----------+
//+----------------------------+

//+--- MQTT POLLING TIME ---+
#define def_mqtt_polling_ms 1  // MQTT polling time (milliseconds)
unsigned long ul_MqttPoll_PrevTime = 0;  // will store last time sensors was read
//#define MQTT_MAX_PACKET_SIZE 2048

//+--- One Second Timming ---+
unsigned long ul_OneSecond_PrevTime=0;


//+----------------------------+
//+--- MQTT -------------------+
//+----------------------------+
String s_mqtt_broker_IP = ""; 
bool b_MqttStatus = 0;
//
bool b_mqtt_reconnect=false;              
bool b_mqtt_reconnected_message=false;    
int i_mqtt_reconnected_display_seconds=0; 


String s_mac=""; //MAC Address

//+----------------------------------------------------------------------------------+

void setup() 
{
    configure_gpio();

    Serial.begin(115200);
    
    WiFi_CONNECT();

 #ifdef OTA_ENABLED   
     ArduinoOTA
    .onStart([]() 
    {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH)
          type = "sketch";
        else // U_SPIFFS
          type = "filesystem";

        // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
        Serial.println("Start updating " + type);
    })
    
    .onEnd([]() 
    {
        Serial.println("\nEnd");
    })
      
    .onProgress([](unsigned int progress, unsigned int total) 
    {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })

    .onError([](ota_error_t error) 
    {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

    ArduinoOTA.begin();

    //+----------------------------------------------------------------------------------+
    //OBS: ADD CODE BELOW THIS NOTE!!!
    Serial.println("Ready");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

     
    /*************** Telnet  *********************/
    //TelnetServer.begin();
    //TelnetServer.setNoDelay(true);
#endif

    //+-- Initiate the Modbus Points - Because MQTT use the Modbus Structure, needs to became available to the others 
    lbmb.ModbusInit();

    ul_OneSecond_PrevTime=millis();    

}


//+----------------------------------------------------------------------------------+
void loop() 
{
#if DEBUG_SERIAL_ENABLE    
    if ((millis() - ul_OneSecond_PrevTime) > 10000)
    {
       Serial.println(WiFi.localIP());
       ul_OneSecond_PrevTime=millis(); 
    }
#endif

#ifdef OTA_ENABLED    
    ArduinoOTA.handle();  

    SerialOTA.print("MAC: "); SerialOTA.println(s_mac);
    
    //+--- TELNET ------------------------------------+
    // Handle new/disconnecting clients.
    if (!haveClient) 
    {
        // Check for new client connections.
        //SerialOTA = TelnetServer.available();
        if (SerialOTA) 
        {
          haveClient = true;
        }
    } 
    else if (!SerialOTA.connected()) 
    {
      // The current client has been disconnected.
      SerialOTA.stop();
      SerialOTA = WiFiClient();
      haveClient = false;
    }

    #ifdef def_Show_SizeOf_DataTypes
    //  Show_SizeOf_DataTypes();
    #endif
#endif

    //+--- MODBUS ------------------------------------+
    if((by_stored_protocol_option==MODBUS_PROTOCOL_LinkBox)||(by_stored_protocol_option==MODBUS_PROTOCOL_1C205))
    {
        mb1.task();
    }

    //aqui ele atualiza as variaveis
    lbmb.Task();

#ifdef MQTT_ENABLE
    //+--- MQTT CONNECT ------------------------------+
    if((by_stored_protocol_option==MQTT_PROTOCOL)&&(client.state()!=0)&&(b_GoToLoop==false))
    {
        //bool ConnectMQTT(bool reconnect) 
        b_mqtt_reconnect=true;
        b_MqttStatus = ConnectMQTT(b_mqtt_reconnect); //Restart MQTT - Connect to broker and subscribe to topic
    }

    //+--- Loop - Checks if there is any publication in MQTT BROKER 
    if(millis()<ul_MqttPoll_PrevTime){ ul_MqttPoll_PrevTime=millis(); } //millis() will overflow (go back to zero), after approximately 50 days. So, when it happens ul_MqttPoll_PrevTime must be reseted
    if(millis()-ul_MqttPoll_PrevTime>=def_mqtt_polling_ms) 
    {
        ul_MqttPoll_PrevTime=millis();
  
        //+--- Checks if MQTT is the selected protocol ---+
        if(by_stored_protocol_option==MQTT_PROTOCOL)
        {
            client.loop();  //+--- Check if there is any publication in MQTT BROKER 
                            //+--- If there is any data, it will be received by the routine Callback(char *topic, byte *payload, unsigned int length)
        }
    }

  //+--- loop send sensor values ---+
    if(millis()<ul_SensorRead_PrevTime){ ul_SensorRead_PrevTime=millis(); } //millis() will overflow (go back to zero), after approximately 50 days. So, when it happens ul_SensorRead_PrevTime must be reseted
    if(millis()-ul_SensorRead_PrevTime>=def_sensors_polling_ms) 
    {
        ul_SensorRead_PrevTime=millis();

        //+--- MODBUS Updates the potentiometer Input Registers ---+
        //+--- Register Type   	Register Number  	Register Size 	Permission            +
        //+--- Input Register	  30001-39999		   16 bit	        R                       +
        //mb1.Ireg(ir_POT_LEFT_OFFSET,  analogRead(def_pin_POT_LEFT));  //+--- Copies the value from ADC1_3 (GPIO39) to the Modbus register ir_POT_LEFT_OFFSET
        //mb1.Ireg(ir_POT_RIGHT_OFFSET, analogRead(def_pin_POT_RIGHT)); //+--- Copies the value from ADC1_6 (GPIO34) to the Modbus register ir_POT_RIGHT_OFFSET 

    }
#endif

} // FIM - void loop()


//+----------------------------------------------------------------------------------+
