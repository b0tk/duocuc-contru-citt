#include <Wire.h>
#include <MPU6050_tockn.h>
#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoHttpClient.h>

MPU6050 mpu(Wire);

// Configuración de WiFi
const char* ssid = "Red-2.4";
const char* password = "Red_2454";
String serverUrl = "http://10.10.10.19:3000/receive_data"; // IP del servidor Python

float ax_offset = 0;
float ay_offset = 0;
float az_offset = 0;
float vx = 0;
float vy = 0;
float vz = 0;
float px = 0;
float py = 0;
float pz = 0;
float roll, pitch;

unsigned long lastTime;
unsigned long lastPrintTime = 0;  // Para controlar la frecuencia de las lecturas

const float alpha = 0.98;  // Filtro complementario
const float accelerationThreshold = 0.02;  // Umbral de aceleración
const int filterWindowSize = 5;  // Tamaño de la ventana para la media móvil

float ax_filter[filterWindowSize];
float ay_filter[filterWindowSize];
float az_filter[filterWindowSize];
int filterIndex = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  // Configuración de WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Conectado a la red WiFi");

  Serial.println("Iniciando el MPU6050...");
  mpu.begin();
  mpu.calcGyroOffsets(true);

  calibrateSensor();

  lastTime = millis();
  memset(ax_filter, 0, sizeof(ax_filter));
  memset(ay_filter, 0, sizeof(ay_filter));
  memset(az_filter, 0, sizeof(az_filter));
}

void loop() {
  unsigned long currentTime = millis();

  // Realizar lecturas y enviar datos cada 5 segundos (5000 ms)
  if (currentTime - lastPrintTime >= 5000) {
    mpu.update();

    float dt = (currentTime - lastTime) / 1000.0;
    lastTime = currentTime;
    lastPrintTime = currentTime;

    // Leer y filtrar la aceleración usando una media móvil simple
    ax_filter[filterIndex] = mpu.getAccX() - ax_offset;
    ay_filter[filterIndex] = mpu.getAccY() - ay_offset;
    az_filter[filterIndex] = mpu.getAccZ() - az_offset;
    filterIndex = (filterIndex + 1) % filterWindowSize;

    float ax = 0, ay = 0, az = 0;
    for (int i = 0; i < filterWindowSize; i++) {
      ax += ax_filter[i];
      ay += ay_filter[i];
      az += az_filter[i];
    }
    ax /= filterWindowSize;
    ay /= filterWindowSize;
    az /= filterWindowSize;

    // Calcular roll y pitch a partir de las lecturas del acelerómetro
    float rollAcc = atan2(ay, az) * 180.0 / PI;
    float pitchAcc = atan2(-ax, sqrt(ay * ay + az * az)) * 180.0 / PI;

    // Filtro complementario para combinar acelerómetro y giroscopio
    roll = alpha * (roll + mpu.getGyroX() * dt) + (1 - alpha) * rollAcc;
    pitch = alpha * (pitch + mpu.getGyroY() * dt) + (1 - alpha) * pitchAcc;

    // Aplicar un umbral mínimo para ignorar pequeñas lecturas de aceleración
    if (abs(ax) < accelerationThreshold) ax = 0;
    if (abs(ay) < accelerationThreshold) ay = 0;
    if (abs(az) < accelerationThreshold) az = 0;

    // Integrar para obtener la velocidad
    vx += ax * dt;
    vy += ay * dt;
    vz += az * dt;

    // Integrar la velocidad para obtener el desplazamiento
    px += vx * dt;
    py += vy * dt;
    pz += vz * dt;

    // Convertir desplazamiento a milímetros
    px *= 1000;
    py *= 1000;
    pz *= 1000;

    // Verificar si ocurre un overflow (ovf) o valores infinitos
    if (isinf(px) || isinf(py) || isinf(pz)) {
      Serial.println("Overflow detectado, reiniciando las lecturas.");
      vx = 0;
      vy = 0;
      vz = 0;
      px = 0;
      py = 0;
      pz = 0;
      lastTime = millis();  // Reiniciar el tiempo para evitar errores en la integración
    } else {
      // Imprimir los resultados si no hay overflow
      Serial.print("Desplazamiento en X: ");
      Serial.print(px);
      Serial.print(" mm, Y: ");
      Serial.print(py);
      Serial.print(" mm, Z: ");
      Serial.println(pz);

      Serial.print("Roll: ");
      Serial.print(roll);
      Serial.print(" °, Pitch: ");
      Serial.println(pitch);

      // Enviar datos al servidor
      if (WiFi.status() == WL_CONNECTED) {
        WiFiClient wifiClient;
        HttpClient httpClient(wifiClient, "10.10.10.19", 3000); // Crear objeto HttpClient
        
        String jsonData = "{\"px\":" + String(px) + 
                          ",\"py\":" + String(py) + ",\"pz\":" + String(pz) +
                          ",\"roll\":" + String(roll) + ",\"pitch\":" + String(pitch) + "}";

        httpClient.post("/receive_data", "application/json", jsonData);

        int httpResponseCode = httpClient.responseStatusCode();
        String responseBody = httpClient.responseBody(); // Obtener cuerpo de la respuesta

        if (httpResponseCode > 0) {
          Serial.print("Datos enviados, código de respuesta: ");
          Serial.println(httpResponseCode);
          Serial.println("Respuesta del servidor: " + responseBody);
        } else {
          Serial.print("Error al enviar los datos. Código de respuesta HTTP: ");
          Serial.println(httpResponseCode);
          Serial.println("Respuesta del servidor: " + responseBody);
        }
      } else {
        Serial.println("Error: No conectado a WiFi");
      }
    }
  }
}

void calibrateSensor() {
  const int numReadings = 100;
  for (int i = 0; i < numReadings; i++) {
    mpu.update();
    ax_offset += mpu.getAccX();
    ay_offset += mpu.getAccY();
    az_offset += mpu.getAccZ();
    delay(50);
  }

  ax_offset /= numReadings;
  ay_offset /= numReadings;
  az_offset /= numReadings;

  Serial.println("Calibración completada");
  Serial.print("Offsets - Aceleración: X=");
  Serial.print(ax_offset);
  Serial.print(", Y=");
  Serial.print(ay_offset);
  Serial.print(", Z=");
  Serial.println(az_offset);
}
