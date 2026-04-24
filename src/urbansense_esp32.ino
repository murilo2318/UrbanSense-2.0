// ============================================================
//  UrbanSense - CP1
//  Nó sensor ESP32 com DHT22 + LDR + LEDs + LCD
//  Curso: Tecnólogo em IA - FIAP
//
//  NOTA: Este é o código base do CP1, evoluído no CP2
//  para incluir conectividade WiFi e MQTT.
//  Consulte urbansense_cp2.ino para a versão completa.
// ============================================================

#include <DHT.h>
#include <LiquidCrystal_I2C.h>

// ── Pinos ───────────────────────────────────────────────────
#define DHTPIN        4
#define DHTTYPE       DHT22
#define LDR_PIN       34
#define LED_AMARELO   5
#define LED_VERMELHO  2

// ── Limiares ────────────────────────────────────────────────
const float TEMP_LIMITE = 30.0;
const int   LUZ_LIMITE  = 2000;

// ── Objetos ─────────────────────────────────────────────────
DHT               dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

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

void setup() {
  Serial.begin(115200);
  Serial.println("========================================");
  Serial.println("  UrbanSense CP1 - Iniciando sistema   ");
  Serial.println("========================================");

  pinMode(LED_AMARELO,  OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("UrbanSense CP1");
  lcd.setCursor(0, 1);
  lcd.print("Iniciando...");

  dht.begin();
  delay(2000);
}

void loop() {
  float temp = dht.readTemperature();
  float umid = dht.readHumidity();
  int   luz  = analogRead(LDR_PIN);

  if (isnan(temp) || isnan(umid)) {
    Serial.println("[ERRO] Falha na leitura do DHT22!");
    return;
  }

  String status = avaliarStatus(temp, luz);

  // Log Serial em CSV
  Serial.print(temp); Serial.print(",");
  Serial.print(umid); Serial.print(",");
  Serial.print(luz);  Serial.print(",");
  Serial.println(status);

  acionarLEDs(status);
  atualizarLCD(temp, umid, luz, status);

  delay(2000);
}
