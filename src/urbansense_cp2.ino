// ============================================================
//  UrbanSense - CP2
//  Nó sensor ESP32 com DHT22 + LDR + WiFi + MQTT (HiveMQ)
//  Curso: Tecnólogo em IA - FIAP
//  Integrantes:
//    Nicolas Lemos Ribeiro    - RM 553273
//    Ricardo de Paiva Melo   - RM 565522
//    Luís Fernando O. Salgado- RM 561401
//    Pedro Leal Murad        - RM 565460
//    Murilo Benhossi         - RM 562358
// ============================================================

#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>

// ── Pinos ───────────────────────────────────────────────────
#define DHTPIN        4
#define DHTTYPE       DHT22
#define LDR_PIN       34
#define LED_AMARELO   5
#define LED_VERMELHO  2

// ── WiFi (Wokwi) ────────────────────────────────────────────
const char* WIFI_SSID = "Wokwi-GUEST";
const char* WIFI_PASS = "";

// ── MQTT (HiveMQ público) ───────────────────────────────────
const char* MQTT_BROKER   = "broker.hivemq.com";
const int   MQTT_PORT     = 1883;
const char* MQTT_CLIENT   = "urbansense-no1-fiap";
const char* MQTT_TOPIC    = "urbansense/no1/dados";

// ── Limiares (herdados do CP1) ───────────────────────────────
const float TEMP_LIMITE  = 30.0;
const int   LUZ_LIMITE   = 2000;

// ── Intervalo de publicação (ms) ────────────────────────────
const unsigned long INTERVALO = 5000;

// ── Objetos ─────────────────────────────────────────────────
DHT               dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);
WiFiClient        wifiClient;
PubSubClient      mqttClient(wifiClient);

unsigned long ultimoEnvio = 0;

// ============================================================
//  Funções auxiliares
// ============================================================

void conectarWiFi() {
  Serial.println("========================================");
  Serial.print("[WiFi] Conectando a: ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("[WiFi] Conectado!");
  Serial.print("[WiFi] IP: ");
  Serial.println(WiFi.localIP());
  Serial.println("========================================");
}

void conectarMQTT() {
  while (!mqttClient.connected()) {
    Serial.print("[MQTT] Conectando ao broker ");
    Serial.print(MQTT_BROKER);
    Serial.print(" ...");

    if (mqttClient.connect(MQTT_CLIENT)) {
      Serial.println(" OK!");
      Serial.print("[MQTT] Client ID: ");
      Serial.println(MQTT_CLIENT);
      Serial.print("[MQTT] Tópico de publicação: ");
      Serial.println(MQTT_TOPIC);
      Serial.println("========================================");
    } else {
      Serial.print(" Falhou (rc=");
      Serial.print(mqttClient.state());
      Serial.println("). Tentando novamente em 3s...");
      delay(3000);
    }
  }
}

String avaliarStatus(float temp, int luz) {
  bool tempAlta = temp > TEMP_LIMITE;
  bool luzAlta  = luz  > LUZ_LIMITE;

  if (tempAlta && luzAlta) return "CRITICO";
  if (tempAlta || luzAlta) return "AVISO";
  return "OK";
}

void acionarLEDs(String status) {
  if (status == "CRITICO") {
    digitalWrite(LED_VERMELHO, HIGH);
    digitalWrite(LED_AMARELO, LOW);
  } else if (status == "AVISO") {
    digitalWrite(LED_VERMELHO, LOW);
    digitalWrite(LED_AMARELO, HIGH);
  } else {
    digitalWrite(LED_VERMELHO, LOW);
    digitalWrite(LED_AMARELO, LOW);
  }
}

void atualizarLCD(float temp, float umid, int luz, String status) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temp, 1);
  lcd.print("C U:");
  lcd.print((int)umid);
  lcd.print("%");

  lcd.setCursor(0, 1);
  lcd.print("Luz:");
  lcd.print(luz);
  lcd.print(" ");
  lcd.print(status);
}

void publicarMQTT(float temp, float umid, int luz, String status) {
  // Monta JSON
  StaticJsonDocument<200> doc;
  doc["dispositivo"]  = "urbansense-no1";
  doc["temperatura"]  = temp;
  doc["umidade"]      = umid;
  doc["luminosidade"] = luz;
  doc["status"]       = status;

  char payload[200];
  serializeJson(doc, payload);

  // Publica
  bool ok = mqttClient.publish(MQTT_TOPIC, payload);

  // Log Serial organizado
  Serial.println("----------------------------------------");
  Serial.println("[MQTT] Publicando...");
  Serial.print("[MQTT] Tópico : ");
  Serial.println(MQTT_TOPIC);
  Serial.print("[MQTT] Payload: ");
  Serial.println(payload);
  Serial.print("[MQTT] Status : ");
  Serial.println(ok ? "Enviado com sucesso!" : "FALHA no envio");
  Serial.println("----------------------------------------");
}

void logSerial(float temp, float umid, int luz, String status) {
  Serial.println("======== LEITURA DOS SENSORES ========");
  Serial.print("[DHT22] Temperatura : ");
  Serial.print(temp);
  Serial.println(" °C");
  Serial.print("[DHT22] Umidade     : ");
  Serial.print(umid);
  Serial.println(" %");
  Serial.print("[LDR]   Luminosidade: ");
  Serial.println(luz);
  Serial.print("[STATUS]             ");
  Serial.println(status);
}

// ============================================================
//  Setup
// ============================================================

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println();
  Serial.println("========================================");
  Serial.println("  UrbanSense CP2 - Iniciando sistema   ");
  Serial.println("========================================");

  // Pinos
  pinMode(LED_AMARELO,  OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);

  // LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("UrbanSense CP2");
  lcd.setCursor(0, 1);
  lcd.print("Iniciando...");

  // DHT
  dht.begin();

  // WiFi
  conectarWiFi();

  // MQTT
  mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
  conectarMQTT();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi+MQTT OK");
  lcd.setCursor(0, 1);
  lcd.print("Monitorando...");
  delay(2000);

  Serial.println("[Sistema] Pronto! Publicando a cada 5s.");
  Serial.println("========================================");
}

// ============================================================
//  Loop
// ============================================================

void loop() {
  // Mantém conexão MQTT ativa
  if (!mqttClient.connected()) {
    Serial.println("[MQTT] Conexão perdida. Reconectando...");
    conectarMQTT();
  }
  mqttClient.loop();

  unsigned long agora = millis();
  if (agora - ultimoEnvio >= INTERVALO) {
    ultimoEnvio = agora;

    // Leitura dos sensores
    float temp = dht.readTemperature();
    float umid = dht.readHumidity();
    int   luz  = analogRead(LDR_PIN);

    // Validação DHT
    if (isnan(temp) || isnan(umid)) {
      Serial.println("[ERRO] Falha na leitura do DHT22!");
      return;
    }

    // Avaliação
    String status = avaliarStatus(temp, luz);

    // Saídas
    logSerial(temp, umid, luz, status);
    acionarLEDs(status);
    atualizarLCD(temp, umid, luz, status);
    publicarMQTT(temp, umid, luz, status);
  }
}
