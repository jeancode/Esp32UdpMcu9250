#include <Arduino.h>
#include <SparkFunMPU9250-DMP.h>
#include <WiFi.h>
#include <WiFiUdp.h>

// Configuración Wi-Fi
const char* ssid = "redWifi";           // Reemplaza con tu SSID
const char* password = "password";   // Reemplaza con tu contraseña

// Configuración UDP
WiFiUDP udp;
const char* serverAddress = "192.168.0.193"; // Dirección IP del servidor UDP
const int serverPort = 50;                // Puerto del servidor UDP

MPU9250_DMP imu;

float pitch, roll, yaw;
float alpha = 0.98; // Coeficiente de filtro complementario

void setup() {
  Serial.begin(115200);

  // Conectar al Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Conectando a Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConectado al Wi-Fi");
  Serial.print("Dirección IP local: ");
  Serial.println(WiFi.localIP());

  // Inicializar el sensor MPU9250
  if (imu.begin() != INV_SUCCESS) {
    while (1) {
      Serial.println("No se puede comunicar con MPU-9250");
      Serial.println("Verifica las conexiones e inténtalo de nuevo.");
      delay(5000);
    }
  }

  imu.setSensors(INV_XYZ_GYRO | INV_XYZ_ACCEL | INV_XYZ_COMPASS);
  imu.setGyroFSR(2000);
  imu.setAccelFSR(2);
  imu.setLPF(5);
  imu.setSampleRate(10);
  imu.setCompassSampleRate(10);

  Serial.println("Cliente UDP listo para enviar datos.");
}

void loop() {
  if (imu.dataReady()) {
    imu.update(UPDATE_ACCEL | UPDATE_GYRO | UPDATE_COMPASS);

    float accelX = imu.calcAccel(imu.ax);
    float accelY = imu.calcAccel(imu.ay);
    float accelZ = imu.calcAccel(imu.az);
    float gyroX = imu.calcGyro(imu.gx);
    float gyroY = imu.calcGyro(imu.gy);
    float gyroZ = imu.calcGyro(imu.gz);
    float magX = imu.calcMag(imu.mx);
    float magY = imu.calcMag(imu.my);
    float magZ = imu.calcMag(imu.mz);

    // Calcular pitch y roll a partir de los datos del acelerómetro:
    float accPitch = atan2(accelY, sqrt(accelX * accelX + accelZ * accelZ)) * RAD_TO_DEG;
    float accRoll = atan2(-accelX, accelZ) * RAD_TO_DEG;

    pitch = alpha * (pitch + gyroX * 0.1) + (1.0 - alpha) * accPitch;
    roll = alpha * (roll + gyroY * 0.1) + (1.0 - alpha) * accRoll;

    // Calcular yaw (heading) a partir de los datos del magnetómetro
    float adjustedMagX = magX * cos(pitch * DEG_TO_RAD) + magZ * sin(pitch * DEG_TO_RAD);
    float adjustedMagY = magX * sin(roll * DEG_TO_RAD) * sin(pitch * DEG_TO_RAD) +
                         magY * cos(roll * DEG_TO_RAD) -
                         magZ * sin(roll * DEG_TO_RAD) * cos(pitch * DEG_TO_RAD);

    yaw = atan2(-adjustedMagY, adjustedMagX) * RAD_TO_DEG;

    // Preparar los datos para enviar
    String data = String(accelX) + "," + String(accelY) + "," + String(accelZ) + "|";

    // Enviar datos al servidor UDP
    udp.beginPacket(serverAddress, serverPort);
    udp.print(data);
    udp.endPacket();

    Serial.println("Datos enviados al servidor UDP: " + data);

    delay(10); // Ajustar según la frecuencia de actualización deseada
  }
}
