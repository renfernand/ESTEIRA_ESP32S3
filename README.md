# 🚀 ESP32S3 – Modbus TCP + MQTT LinkBox

Projeto desenvolvido para rodar em **ESP32-S3-WROOM-1** utilizando **PlatformIO (VS Code)** e framework **Arduino**.  
O firmware integra comunicação **Modbus TCP Slave** e **MQTT** para aplicações de automação industrial.

este projeto eh baseado no hardware desenvolvido pelo Douglas Almeida no seguinte repositorio
https://github.com/dougsthenri/esp32_rio
apesar de ter um codigo fonte desenvolvido pelo autor, foi alterado o codigo para ficar no padrao do laboratorio
usando o codigo do Prof. Fabio Vincenzi com o codigo deste repositorio
https://github.com/fabiovince/LinkBox

---

## 📑 Sumário
- [Descrição](#-descrição)
- [Requisitos de Hardware](#-requisitos-de-hardware)
- [Requisitos de Software](#-requisitos-de-software)
- [Instalação e Uso](#-instalação-e-uso)
- [Configuração de Rede](#-configuração-de-rede)
- [Exemplo de Saída Serial](#-exemplo-de-saída-serial)
- [Estrutura do Projeto](#-estrutura-do-projeto)
- [Licença](#-licença)

---

## 📝 Descrição
O sistema foi desenvolvido para:
- Operar como **Modbus TCP Slave** em rede local.
- Publicar dados em **broker MQTT**.
- Permitir atualização OTA (Over-The-Air).
- Suporte a **diferentes BSPs** (módulo de motor, caixa de botões/LEDs, sensores).

---

## 🔧 Requisitos de Hardware
- ESP32-S3-WROOM-1 (montado em PCB própria)
- Botões, LEDs e/ou sensores digitais
- Fonte de alimentação 24Vdc
- Conexão Wi-Fi com IP Fixo (ver WIFI_CONN.cpp)

---

## 💻 Requisitos de Software
- [PlatformIO](https://platformio.org/) (recomendado no VS Code)
- Framework **Arduino-ESP32**
- Bibliotecas:
  - `WiFi.h`
  - `PubSubClient.h` (para MQTT)
  - `ModbusIP_ESP8266.h` (ou equivalente ESP32)

---

## ⚙️ Instalação e Uso

Clone o repositório:

```bash
git clone https://github.com/seuusuario/seuprojeto.git
cd seuprojeto
Compile e faça o upload:

bash
Copiar código
pio run --target upload
Abra o monitor serial:

bash
Copiar código
pio device monitor
🌐 Configuração de Rede
No código principal (main.cpp):

cpp
Copiar código
const char* ssid = "LinkBox_ssid";
const char* password = "LinkBox_password";

IPAddress local_IP(192, 168, 1, 200);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
🖥️ Exemplo de Saída Serial
makefile
Copiar código
Setup funcionou!!!!
Teste1 = 0
Teste1 = 1
Teste1 = 2
📂 Estrutura do Projeto
makefile
Copiar código
├── src/
│   └── main.cpp         # Código principal
├── include/             # Headers (.h)
├── lib/                 # Bibliotecas locais
├── platformio.ini       # Configuração da build
└── README.md            # Este arquivo
📜 Licença
Este projeto está licenciado sob a MIT License.

yaml
Copiar código





