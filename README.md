# Sensor MPU-9250 con Cliente UDP en ESP32

Este proyecto implementa un cliente UDP en un ESP32 para recopilar datos del sensor **MPU-9250** y enviarlos a un servidor remoto. El sistema calcula la orientación del dispositivo utilizando los sensores integrados (giroscopio, acelerómetro y magnetómetro) y aplica un filtro complementario para mejorar la precisión.

## Características

- **Conexión Wi-Fi:** El ESP32 se conecta a una red Wi-Fi y envía datos al servidor mediante el protocolo UDP.
- **Configuración de sensores:** Configuración personalizada para:
  - Giroscopio: rango ±2000 °/s.
  - Acelerómetro: rango ±2 g.
  - Filtro de paso bajo: 5 Hz.
  - Tasa de muestreo: 10 Hz.
- **Cálculo de orientación:**
  - Pitch (*inclinación*) y roll (*balanceo*) calculados a partir del acelerómetro y giroscopio.
  - Yaw (*rumbo*) calculado utilizando datos ajustados del magnetómetro.

## Requisitos

### Hardware
- **ESP32:** Microcontrolador principal.
- **MPU-9250:** Sensor para giroscopio, acelerómetro y brújula.

### Software
- **Arduino IDE:** Para cargar el código al ESP32.
- **Librerías necesarias:**
  - [`SparkFunMPU9250-DMP`](https://github.com/sparkfun/SparkFun_MPU-9250-DMP_Arduino_Library)
  - [`WiFi`](https://www.arduino.cc/en/Reference/WiFi)
  - [`WiFiUdp`](https://www.arduino.cc/en/Reference/WiFiUDP)


