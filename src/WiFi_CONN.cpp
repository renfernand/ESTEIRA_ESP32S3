#include "Config.h"
//#include <SSID_PASSWORD.h>
#include "WiFi_CONN.h"
#include "global_vars.h"
#include "MQTT.h"
//#include <ArduinoOTA.h>

// Configuração de IP fixo
IPAddress local_IP(192, 168, 0, 218);  
IPAddress gateway(192, 168, 0, 1);     
IPAddress subnet(255, 255, 255, 0);    
IPAddress primaryDNS(192, 168, 0, 1);  
IPAddress secondaryDNS(8, 8, 4, 4);     // DNS secundário (Google)

void WiFi_CONNECT()
{
    //+---------------------------------------------------+
    //+--- SSID and PASSWORD CONFIG ----------------------+
    //+---------------------------------------------------+
    const char* Alexa_ssid = "Domotica";
    const char* Alexa_password = "domotica1c203a";

    const char* LinkBox_ssid = "Rede udi";
    const char* LinkBox_password = "1j4b7i2g$4";

    const char* Sala_1C205_ssid = "INDUSTRIA";
    const char* Sala_1C205_password = "industria50";
    //+---------------------------------------------------+

    //WiFi.mode(WIFI_STA);
    // Configura IP fixo
    if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
        Serial.println("Falha ao configurar IP fixo");
    }
    
    if(by_stored_protocol_option==MODBUS_PROTOCOL_LinkBox)
    {
        WiFi.begin(LinkBox_ssid, LinkBox_password); //MODBUS and MQTT LinkBox SSID and Password
    }
    else if(by_stored_protocol_option==MODBUS_PROTOCOL_1C205)
    {
        WiFi.begin(Sala_1C205_ssid, Sala_1C205_password); //Connects to the classroom router 
    }
    else if(by_stored_protocol_option==MQTT_PROTOCOL)
    {
        WiFi.begin(LinkBox_ssid, LinkBox_password); //MODBUS and MQTT LinkBox SSID and Password
    }
    else if(by_stored_protocol_option==ALEXA_PROTOCOL)
    {
        WiFi.begin(Alexa_ssid, Alexa_password); //Connects to the Home Automation  classroom router
    }
    else  //When the ESP32 is being recorded for the first time it is necessary to choose an option in the MENU 
    {
        #if 0
        EEPROM.write(0, 0); EEPROM.commit();  // ("MODBUS > SSID:LinkBox ") option
        delay(300);
        by_stored_protocol_option = EEPROM.read(0);  //selected protocol
        #endif
        WiFi.begin(LinkBox_ssid, LinkBox_password); 
    }

  
    while (WiFi.waitForConnectResult() != WL_CONNECTED) 
    {
        Serial.println("WiFi Connection Failed! Rebooting...");
        //SerialOTA.println("WiFi Connection Failed! Rebooting..."); //+--Putty PORT 23

        ESP.restart();
    }
    
    s_mac += String(WiFi.macAddress());
    
    Serial.print("MAC: ");   Serial.println(s_mac);  
    //SerialOTA.print("MAC: "); SerialOTA.println(s_mac);
    //const char mac[] = (String(WiFi.macAddress())).c_str();

    if(s_mac==LB0_Id1_MAC){ s_client_id += LB0_Id1_client; s_mqtt_broker_IP += LB0_mqtt_broker_IP; u_thisDeviceId= LB0_Id1_MQTT; }
    else if(s_mac==LB0_Id2_MAC){ s_client_id += LB0_Id2_client; s_mqtt_broker_IP += LB0_mqtt_broker_IP; u_thisDeviceId= LB0_Id2_MQTT; }
    //
    else if(s_mac==LB1_Id1_MAC){ s_client_id += LB1_Id1_client; s_mqtt_broker_IP += LB1_mqtt_broker_IP; u_thisDeviceId= LB1_Id1_MQTT; }
    else if(s_mac==LB1_Id2_MAC){ s_client_id += LB1_Id2_client; s_mqtt_broker_IP += LB1_mqtt_broker_IP; u_thisDeviceId= LB1_Id2_MQTT; }
    //
    else if(s_mac==LB2_Id1_MAC){ s_client_id += LB2_Id1_client; s_mqtt_broker_IP += LB2_mqtt_broker_IP; u_thisDeviceId= LB2_Id1_MQTT; }
    else if(s_mac==LB2_Id2_MAC){ s_client_id += LB2_Id2_client; s_mqtt_broker_IP += LB2_mqtt_broker_IP; u_thisDeviceId= LB2_Id2_MQTT; }
    //
    else if(s_mac==LB3_Id1_MAC){ s_client_id += LB3_Id1_client; s_mqtt_broker_IP += LB3_mqtt_broker_IP; u_thisDeviceId= LB3_Id1_MQTT; }
    else if(s_mac==LB3_Id2_MAC){ s_client_id += LB3_Id2_client; s_mqtt_broker_IP += LB3_mqtt_broker_IP; u_thisDeviceId= LB3_Id2_MQTT; }
    //
    else if(s_mac==LB4_Id1_MAC){ s_client_id += LB4_Id1_client; s_mqtt_broker_IP += LB4_mqtt_broker_IP; u_thisDeviceId= LB4_Id1_MQTT; }
    else if(s_mac==LB4_Id2_MAC){ s_client_id += LB4_Id2_client; s_mqtt_broker_IP += LB4_mqtt_broker_IP; u_thisDeviceId= LB4_Id2_MQTT; }
    //
    else if(s_mac==LB5_Id1_MAC){ s_client_id += LB5_Id1_client; s_mqtt_broker_IP += LB5_mqtt_broker_IP; u_thisDeviceId= LB5_Id1_MQTT; }
    else if(s_mac==LB5_Id2_MAC){ s_client_id += LB5_Id2_client; s_mqtt_broker_IP += LB5_mqtt_broker_IP; u_thisDeviceId= LB5_Id2_MQTT; }
    //
    else if(s_mac==LB6_Id1_MAC){ s_client_id += LB6_Id1_client; s_mqtt_broker_IP += LB6_mqtt_broker_IP; u_thisDeviceId= LB6_Id1_MQTT; }
    else if(s_mac==LB6_Id2_MAC){ s_client_id += LB6_Id2_client; s_mqtt_broker_IP += LB6_mqtt_broker_IP; u_thisDeviceId= LB6_Id2_MQTT; }
    //
    else if(s_mac==LB7_Id1_MAC){ s_client_id += LB7_Id1_client; s_mqtt_broker_IP += LB7_mqtt_broker_IP; u_thisDeviceId= LB7_Id1_MQTT; }
    else if(s_mac==LB7_Id2_MAC){ s_client_id += LB7_Id2_client; s_mqtt_broker_IP += LB7_mqtt_broker_IP; u_thisDeviceId= LB7_Id2_MQTT; }
    //
    else if(s_mac==LB8_Id1_MAC){ s_client_id += LB8_Id1_client; s_mqtt_broker_IP += LB8_mqtt_broker_IP; u_thisDeviceId= LB8_Id1_MQTT; }
    else if(s_mac==LB8_Id2_MAC){ s_client_id += LB8_Id2_client; s_mqtt_broker_IP += LB8_mqtt_broker_IP; u_thisDeviceId= LB8_Id2_MQTT; }
    //
    else if(s_mac==LB9_Id1_MAC){ s_client_id += LB9_Id1_client; s_mqtt_broker_IP += LB9_mqtt_broker_IP; u_thisDeviceId= LB9_Id1_MQTT; }
    else if(s_mac==LB9_Id2_MAC){ s_client_id += LB9_Id2_client; s_mqtt_broker_IP += LB9_mqtt_broker_IP; u_thisDeviceId= LB9_Id2_MQTT; }
    //
    else if(s_mac==MOTOR_Id1_MAC){ s_client_id += MOTOR_Id1_client; s_mqtt_broker_IP += MOTOR_mqtt_broker_IP; u_thisDeviceId= MOTOR_Id1_MQTT; }
    //
    else if(s_mac==FAB_Id1_MAC){ s_client_id += FAB_Id1_client; s_mqtt_broker_IP += FAB_mqtt_broker_IP; u_thisDeviceId= FAB_Id1_MQTT; }
    else if(s_mac==FAB_Id2_MAC){ s_client_id += FAB_Id2_client; s_mqtt_broker_IP += FAB_mqtt_broker_IP; u_thisDeviceId= FAB_Id2_MQTT; }
    //
    else { s_client_id += "ESP32_NOVA-"; s_mqtt_broker_IP += "192.168.1.233"; u_thisDeviceId= 1; }

    s_client_id += String(WiFi.macAddress());
    //ArduinoOTA.setHostname(s_client_id.c_str());

  
    // No authentication by default
    //ArduinoOTA.setPassword("admin");
    //ArduinoOTA.setPassword("");
    //
    //
    // cc_password can be set with it's md5 value as well
    // https://md5.gromweb.com/
    // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
    //ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3"); //Codified with MD5 Hash Generator
    //ArduinoOTA.setPasswordHash("0ac4fb5f0c170b08e9a7d2f4c07536be"); //Codified with MD5 Hash Generator

} //END - WiFi_Connect()