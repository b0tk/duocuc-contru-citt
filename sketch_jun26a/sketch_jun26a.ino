#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "TU_SSID";
const char* password = "TU_PASSWORD";

const char* serverName = "http://TU_IP_DEL_SERVIDOR:3000/data";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }
  
  Serial.println("Conectado a la red WiFi");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    int32_t rssi = WiFi.RSSI();
    String json = "{\"rssi\":" + String(rssi) + "}";

    int httpResponseCode = http.POST(json);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.print("Error en la petición: ");
      Serial.println(httpResponseCode);
    }
    
    http.end();
  } else {
    Serial.println("Error en la conexión WiFi");
  }

  delay(10000); // Enviar datos cada 10 segundos
}

