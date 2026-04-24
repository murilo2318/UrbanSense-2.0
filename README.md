# UrbanSense

Protótipo da plataforma UrbanSense, desenvolvido para os checkpoints da disciplina de IoT do curso de Tecnólogo em Inteligência Artificial – FIAP.

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

---

## CP2 – Conectividade IoT (WiFi + MQTT)

Evolução do CP1 com conectividade em nuvem:

- Mantém toda a lógica do CP1 (leitura, limiares, LEDs, LCD)
- Conecta à rede **WiFi** (`Wokwi-GUEST`)
- Publica dados a cada 5 segundos via **MQTT** no broker público **HiveMQ**
- Tópico: `urbansense/no1/dados`
- Payload estruturado em **JSON** com temperatura, umidade, luminosidade e status
- Serial Monitor organizado com logs de WiFi, MQTT e cada publicação
- Circuito simulado via **Wokwi** com `diagram.json` incluso

### Verificar mensagens no broker

Acesse o HiveMQ Websocket Client:  
🔗 https://www.hivemq.com/demos/websocket-client/  
Broker: `broker.hivemq.com` | Porta: `8884` | Tópico: `urbansense/no1/dados`

### Explicação Técnica

Consulte [`docs/explicacao_tecnica_cp2.md`](docs/explicacao_tecnica_cp2.md) para detalhes sobre sensores, estrutura da mensagem, tópico MQTT e fluxo do Serial Monitor.

---

## Estrutura do Repositório

```
UrbanSense-2.0/
├── src/
│   ├── urbansense_esp32.ino        # Código CP1 - nó sensor local
│   └── urbansense_cp2.ino          # Código CP2 - WiFi + MQTT
├── data/
│   └── dados_urbansense.csv        # Dados coletados pelo sensor
├── evidencias/
│   ├── circuito-ok.png             # Circuito em estado OK
│   ├── circuito-aviso.png          # Circuito em estado AVISO
│   ├── circuito-critico.png        # Circuito em estado CRITICO
│   └── mqtt-publicacao.png         # Evidência de publicação MQTT (CP2)
├── docs/
│   └── explicacao_tecnica_cp2.md   # Explicação técnica CP2
├── diagram.json                    # Diagrama do circuito Wokwi (CP2)
└── README.md
```

---

## Disciplinas

- **IoT (CP1)** – Nó sensor com ESP32, sensores, atuadores e limiares
- **IoT (CP2)** – Conectividade WiFi + publicação MQTT em nuvem

---

## Decisões Técnicas e Problemas Encontrados

### Broker MQTT — Por que usamos o Mosquitto?

Durante o desenvolvimento, testamos dois brokers públicos antes de chegar à solução final:

**HiveMQ (`broker.hivemq.com`)**  
Era nossa primeira escolha por ser amplamente usado em projetos IoT. Porém, o ambiente de simulação do Wokwi (plano gratuito) não permite conexões externas à internet sem a ativação de um **Gateway Privado** (recurso pago). Ao tentar conectar, o Wokwi exibiu a mensagem *"Ativar Gateway Privado"*, bloqueando a comunicação com o broker.

**Mosquitto (`test.mosquitto.org`)**  
Após o problema com o HiveMQ, migramos para o broker público da Mosquitto. Este broker é compatível com o ambiente do Wokwi e não exige gateway privado, permitindo a publicação dos dados via MQTT normalmente na simulação.

> Em um ambiente de produção com ESP32 físico ou com o Gateway Privado do Wokwi ativado, o HiveMQ funcionaria sem problemas, pois o código original foi desenvolvido para ele.
