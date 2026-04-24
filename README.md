# UrbanSense

Protótipo da plataforma UrbanSense, desenvolvido para os checkpoints das disciplinas de IoT, Computação Quântica, Governança em IA e PLN do curso de Tecnólogo em Inteligência Artificial – FIAP.

## Integrantes

- Nicolas Lemos Ribeiro - RM 553273
- Ricardo de Paiva Melo - RM 565522
- Luís Fernando de Oliveira Salgado - RM 561401
- Pedro Leal Murad - RM 565460
- Murilo Benhossi - RM 562358

## Sobre o Projeto

A UrbanSense é uma startup fictícia que propõe uma rede de sensores inteligentes espalhados pela cidade para monitorar condições ambientais em tempo real. A cada checkpoint, o protótipo evolui: de um nó sensor local até um sistema conectado à nuvem com capacidade de Edge AI.

---

## CP1 – Nó Sensor Local

O primeiro protótipo faz a leitura dos sensores e reage localmente:

- Lê **temperatura e umidade** com sensor DHT22
- Lê **luminosidade** com sensor LDR
- Classifica o ambiente em 3 níveis: **OK**, **AVISO** e **CRITICO**
- Aciona **LED amarelo** (aviso) ou **LED vermelho** (crítico)
- Exibe os dados em tempo real num **display LCD 16x2 I2C**
- Registra tudo no **Serial Monitor** em formato CSV

### Como Rodar o CP1 (Wokwi)

1. Acesse [wokwi.com](https://wokwi.com) e crie um novo projeto ESP32
2. Cole o código de `src/urbansense_esp32.ino`
3. Adicione os componentes: DHT22, LDR, LED vermelho, LED amarelo e LCD 16x2 I2C
4. Conecte nos pinos: DHT22 → D4, LDR → D34, LED vermelho → D2, LED amarelo → D5, LCD SDA → D21 e SCL → D22
5. Rode a simulação

---

## CP2 – Conectividade IoT (WiFi + MQTT)

Evolução do CP1 com conectividade em nuvem:

- Mantém toda a lógica do CP1 (leitura, limiares, LEDs, LCD)
- Conecta à rede **WiFi** (`Wokwi-GUEST`)
- Publica dados a cada 5 segundos via **MQTT** no broker público **HiveMQ**
- Tópico: `urbansense/no1/dados`
- Payload estruturado em **JSON** com temperatura, umidade, luminosidade e status
- Serial Monitor organizado com logs de WiFi, MQTT e cada publicação

### Como Rodar o CP2 (Wokwi)

1. Acesse [wokwi.com](https://wokwi.com) e crie um novo projeto ESP32
2. Cole o código de `src/urbansense_cp2.ino`
3. Monte o mesmo circuito do CP1 (mesmos pinos)
4. Instale as bibliotecas: `DHT`, `LiquidCrystal_I2C`, `PubSubClient`, `ArduinoJson`
5. Rode a simulação e acompanhe o Serial Monitor

### Verificar mensagens no broker

Acesse o HiveMQ Websocket Client:  
🔗 https://www.hivemq.com/demos/websocket-client/  
Broker: `broker.hivemq.com` | Porta: `8884` | Tópico: `urbansense/no1/dados`

### Explicação Técnica

Consulte [`docs/explicacao_tecnica_cp2.md`](docs/explicacao_tecnica_cp2.md) para detalhes sobre sensores, estrutura da mensagem, tópico MQTT e fluxo do Serial Monitor.

---

## Estrutura do Repositório

```
urbansense/
├── src/
│   ├── urbansense_esp32.ino      # Código CP1 - nó sensor local
│   ├── urbansense_cp2.ino        # Código CP2 - WiFi + MQTT
│   └── analise_llm.ipynb         # Notebook com análise via LLM (com output)
├── data/
│   └── dados_urbansense.csv      # Dados coletados pelo sensor
├── evidencias/
│   ├── circuito-ok.png           # Circuito em estado OK
│   ├── circuito-aviso.png        # Circuito em estado AVISO
│   ├── circuito-critico.png      # Circuito em estado CRITICO
│   └── mqtt-publicacao.png       # Evidência de publicação MQTT (CP2)
├── docs/
│   ├── urbansense_cp1.docx       # Documento completo CP1
│   └── explicacao_tecnica_cp2.md # Explicação técnica CP2
└── README.md
```

---

## Disciplinas

- **IoT (CP1)** – Nó sensor com ESP32, sensores, atuadores e limiares
- **IoT (CP2)** – Conectividade WiFi + publicação MQTT em nuvem
- **Governança em IA (CP1)** – Matriz de riscos técnicos, éticos, operacionais e de governança
- **PLN (CP3)** – Análise de dados com LLM (GPT-4o-mini)
- **Computação Quântica (CP2)** – Em breve
