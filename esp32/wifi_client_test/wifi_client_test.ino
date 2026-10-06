/*
 * Prueba de comunicación ESP32 -> servidor TCP (PC) por WiFi.
 *
 * Basado en el ejemplo WiFiClient de arduino-esp32
 * (libraries/WiFi/examples/WiFiClient).
 *
 * 1. Cambiar ssid, password y serverIp (IP de la PC que corre basic_server.py).
 * 2. Cargar el sketch y abrir el Serial Monitor a 115200 baudios.
 * 3. La ESP32 envía "HELLO <n>" cada segundo e imprime la respuesta del servidor.
 */

#include <Arduino.h>
#include <WiFi.h>

const char *ssid = "your-ssid";          // Cambiar por el nombre de la red WiFi
const char *password = "your-password";  // Cambiar por la contraseña

const char *serverIp = "192.168.0.102";  // IP de la PC con el servidor
const uint16_t serverPort = 5000;

const unsigned long SEND_INTERVAL_MS = 1000;

WiFiClient client;
unsigned long counter = 0;

void connectWiFi() {
  Serial.print("Conectando a WiFi ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP de la ESP32: ");
  Serial.println(WiFi.localIP());
}

bool connectServer() {
  Serial.print("Conectando al servidor ");
  Serial.print(serverIp);
  Serial.print(":");
  Serial.println(serverPort);
  if (!client.connect(serverIp, serverPort)) {
    Serial.println("Conexion fallida, reintentando...");
    return false;
  }
  Serial.println("Conectado al servidor TCP");
  return true;
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  connectWiFi();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }
  if (!client.connected() && !connectServer()) {
    delay(2000);
    return;
  }

  String message = "HELLO " + String(counter++);
  client.print(message + "\n");
  Serial.print("Enviado: ");
  Serial.println(message);

  unsigned long start = millis();
  while (!client.available() && millis() - start < 1000) {
    delay(10);
  }
  if (client.available()) {
    String reply = client.readStringUntil('\n');
    Serial.print("Respuesta: ");
    Serial.println(reply);
  } else {
    Serial.println("Sin respuesta del servidor");
  }

  delay(SEND_INTERVAL_MS);
}
