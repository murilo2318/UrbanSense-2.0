# UrbanSense CP2 – Explicação Técnica (IoT)

## Sensores Utilizados

### DHT22 (Temperatura e Umidade)
- **Pino:** D4 (GPIO 4)
- **Dados coletados:** temperatura em °C e umidade relativa do ar em %
- **Biblioteca:** `DHT` (Adafruit)
- O sensor é lido a cada 5 segundos. Caso retorne `NaN`, o sistema registra erro no Serial Monitor e ignora o ciclo.

### LDR (Luminosidade)
- **Pino:** D34 (GPIO 34 – entrada analógica)
- **Dados coletados:** nível de luminosidade (0 a 4095, leitura ADC 12 bits do ESP32)
- Valores mais altos indicam mais luz. Limiar definido em 2000.

---

## Lógica de Status (herdada do CP1)

| Condição | Status |
|---|---|
| Temperatura ≤ 30°C **e** Luminosidade ≤ 2000 | `OK` |
| Temperatura > 30°C **ou** Luminosidade > 2000 | `AVISO` |
| Temperatura > 30°C **e** Luminosidade > 2000 | `CRITICO` |

- **LED amarelo (D5):** acende no status AVISO
- **LED vermelho (D2):** acende no status CRÍTICO
- **LCD 16x2 I2C (SDA: D21, SCL: D22):** exibe temperatura, umidade, luminosidade e status em tempo real

---

## Conectividade WiFi

- **Biblioteca:** `WiFi.h` (nativa do ESP32 no Arduino)
- **SSID:** `Wokwi-GUEST` (rede simulada do Wokwi, sem senha)
- O sistema aguarda a conexão antes de prosseguir, exibindo logs no Serial Monitor a cada tentativa.

---

## Protocolo MQTT

- **Biblioteca:** `PubSubClient`
- **Broker:** `test.mosquitto.org` (Mosquitto público, gratuito, sem autenticação)
- **Porta:** `1883` (TCP padrão MQTT)
- **Client ID:** `urbansense-no1-fiap`
- **Tópico de publicação:** `urbansense/no1/dados`

O sistema tenta reconectar automaticamente ao broker caso a conexão seja perdida durante o loop.

---

## Estrutura da Mensagem (Payload JSON)

As mensagens são publicadas em formato **JSON**, escolhido por ser legível, amplamente suportado por plataformas IoT e facilmente parseável para etapas futuras (armazenamento, dashboards e modelos de IA).

```json
{
  "dispositivo": "urbansense-no1",
  "temperatura": 28.5,
  "umidade": 65.0,
  "luminosidade": 1500,
  "status": "OK"
}
```

| Campo | Tipo | Descrição |
|---|---|---|
| `dispositivo` | string | Identificador único do nó sensor |
| `temperatura` | float | Temperatura em graus Celsius |
| `umidade` | float | Umidade relativa do ar em % |
| `luminosidade` | int | Leitura ADC do LDR (0–4095) |
| `status` | string | Nível de alerta: OK, AVISO ou CRITICO |

---

## Fluxo do Serial Monitor

O Serial Monitor exibe logs organizados em três blocos:

1. **Inicialização** – WiFi, IP obtido, conexão MQTT e tópico
2. **Leitura dos sensores** – valores de temperatura, umidade, luminosidade e status a cada 5s
3. **Publicação MQTT** – tópico, payload completo e confirmação de envio

---

## Bibliotecas Necessárias (Arduino IDE / Wokwi)

| Biblioteca | Versão recomendada |
|---|---|
| DHT sensor library (Adafruit) | ≥ 1.4.4 |
| LiquidCrystal I2C | ≥ 1.1.2 |
| PubSubClient | ≥ 2.8 |
| ArduinoJson | ≥ 6.x |

---

## Como Rodar no Wokwi

1. Acesse [wokwi.com](https://wokwi.com) e crie um novo projeto ESP32
2. Cole o conteúdo de `src/urbansense_cp2.ino`
3. Monte o circuito com os componentes abaixo e conecte nos pinos indicados:
   - DHT22 → D4
   - LDR → D34
   - LED vermelho → D2
   - LED amarelo → D5
   - LCD 16x2 I2C → SDA: D21, SCL: D22
4. Instale as bibliotecas no painel de bibliotecas do Wokwi
5. Execute a simulação e acompanhe o Serial Monitor

Para verificar as mensagens publicadas no broker, acesse o **HiveMQ Websocket Client**:  
🔗 https://www.hivemq.com/demos/websocket-client/  
- Broker: `test.mosquitto.org` | Porta: `8884` | Tópico: `urbansense/no1/dados`
