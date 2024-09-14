#include <Wire.h>
#include <MPU6050_tockn.h>
#include <Arduino.h>
#include <WiFi.h>  // Librería para la conexión WiFi

// Credenciales de la red Wi-Fi
const char* ssid = "DUOC_IoT";
const char* password = "1ns.D%0c1ot23";

MPU6050 mpu(Wire);

// Variables para almacenar los offsets
float ax_offset = 0;
float ay_offset = 0;
float az_offset = 0;
float gx_offset = 0;
float gy_offset = 0;
float gz_offset = 0;

// Configuración del servidor web en el puerto 80
WiFiServer server(80);

void setup() {
  Serial.begin(9600);
  Wire.begin();

  // Conexión a la red Wi-Fi
  Serial.print("Conectando a ");
  Serial.println(ssid);

  // Intentar conectarse hasta un máximo de 10 intentos
  int maxRetries = 10;
  int attempt = 0;

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED && attempt < maxRetries) {
    delay(500);
    Serial.print(".");
    attempt++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConectado a la red Wi-Fi");
    Serial.print("Dirección IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nError: No se pudo conectar a la red Wi-Fi.");
    Serial.println("Verifica las credenciales o la señal de la red.");
    return;
  }

  // Iniciar el servidor web
  server.begin();
  Serial.println("Servidor web iniciado");

  Serial.println("Iniciando el MPU6050...");

  mpu.begin();
  mpu.calcGyroOffsets(true);

  Serial.println("Calibrando el sensor...");

  // Calibración inicial para determinar los offsets
  const int numReadings = 100;
  for (int i = 0; i < numReadings; i++) {
    mpu.update();

    ax_offset += mpu.getAccX();
    ay_offset += mpu.getAccY();
    az_offset += mpu.getAccZ();
    gx_offset += mpu.getGyroX();
    gy_offset += mpu.getGyroY();
    gz_offset += mpu.getGyroZ();

    delay(50);
  }

  ax_offset /= numReadings;
  ay_offset /= numReadings;
  az_offset /= numReadings;
  gx_offset /= numReadings;
  gy_offset /= numReadings;
  gz_offset /= numReadings;

  Serial.println("Calibración completada");
  Serial.print("Offsets - Aceleración: X=");
  Serial.print(ax_offset);
  Serial.print(", Y=");
  Serial.print(ay_offset);
  Serial.print(", Z=");
  Serial.println(az_offset);
  Serial.print("Offsets - Giroscopio: X=");
  Serial.print(gx_offset);
  Serial.print(", Y=");
  Serial.print(gy_offset);
  Serial.print(", Z=");
  Serial.println(gz_offset);
}

void loop() {
  // Actualizar los valores del sensor
  mpu.update();

  // Leer los valores del acelerómetro y giroscopio
  float ax = mpu.getAccX() - ax_offset;
  float ay = mpu.getAccY() - ay_offset;
  float az = mpu.getAccZ() - az_offset;
  float gx = mpu.getGyroX() - gx_offset;
  float gy = mpu.getGyroY() - gy_offset;
  float gz = mpu.getGyroZ() - gz_offset;

  // Calcular los ángulos de Roll y Pitch
  float roll = atan2(ay, sqrt(ax * ax + az * az)) * 180.0 / PI;
  float pitch = atan2(-ax, sqrt(ay * ay + az * az)) * 180.0 / PI;

  // Esperar una conexión de cliente
  WiFiClient client = server.available();
  if (client) {
    Serial.println("Cliente conectado");
    // Esperar hasta que el cliente envíe datos
    while (client.connected()) {
      if (client.available()) {
        // Leer la solicitud del cliente
        String request = client.readStringUntil('\r');
        Serial.println(request);
        client.flush();

        // Responder al cliente con los datos del sensor en formato HTML
        client.println("HTTP/1.1 200 OK");
        client.println("Content-type:text/html");
        client.println();

        client.println("<!DOCTYPE HTML>");
        client.println("<html>");
        client.println("<h2>Datos del Sensor MPU6050</h2>");
        client.println("<p>Aceleración en X: " + String(ax) + " g</p>");
        client.println("<p>Aceleración en Y: " + String(ay) + " g</p>");
        client.println("<p>Aceleración en Z: " + String(az) + " g</p>");
        client.println("<p>Giroscopio en X: " + String(gx) + " °/s</p>");
        client.println("<p>Giroscopio en Y: " + String(gy) + " °/s</p>");
        client.println("<p>Giroscopio en Z: " + String(gz) + " °/s</p>");
        client.println("<p>Roll: " + String(roll) + " °</p>");
        client.println("<p>Pitch: " + String(pitch) + " °</p>");
        client.println("</html>");

        break;
      }
    }

    // Cerrar la conexión con el cliente
    client.stop();
    Serial.println("Cliente desconectado");
  }

  delay(1000);
}
