#include "BluetoothSerial.h"
#include "model_emb24_h4_l2_d001_ep300_bs8_lr001_model.h"

using namespace model_emb24_h4_l2_d001_ep300_bs8_lr001;

BluetoothSerial SerialBT;


#define ANSWER_BUFFER_SIZE 256 
char answerBuffer[ANSWER_BUFFER_SIZE];
float start_time = -1;
float end_time = -1;
float width_time = -1;

// Pergunta padrão
const String defaultQuestion =
  "How old are moped riders typically?";

// Controle de tempo
unsigned long waitStartTime = 0;
const unsigned long SERIAL_TIMEOUT = 10000; // 10 segundos


int maxNewTokensForAnswer = 10;

String buffer = "";  // armazena mensagem recebida

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_BT_SERIAL");
  Serial.println("Bluetooth clássico pronto!");
}

void processQuestion(const String& question) {
  Serial.print("Question: ");
  Serial.println(question);

  Serial.println("Generating answer...");
  start_time = millis();
  model_emb24_h4_l2_d001_ep300_bs8_lr001::generate_qna_answer(
    answerBuffer,
    ANSWER_BUFFER_SIZE,
    question,
    maxNewTokensForAnswer
);
  end_time = millis();
  Serial.print("Answer: ");
  Serial.println(answerBuffer);
  width_time = end_time - start_time;
  Serial.print("Processing time (ms): ");
  Serial.println(width_time);
  Serial.println("\nNext question:\n");
}


void loop() {

  // =========================
  // Receber do celular
  // =========================
  while (SerialBT.available()) {
    char c = SerialBT.read();

    // Mostra no Serial Monitor
    Serial.write(c);

    // Monta a string
    if (c == '\n') {
      buffer.trim();  // remove espaços e \r

      if (buffer.length() > 0) {
        Serial.println("\nMensagem completa recebida:");
        Serial.println(buffer);

        processQuestion(buffer);

        // =========================
        // RESPOSTA AUTOMÁTICA
        // =========================
        //String resposta = "ESP32 recebeu: " + answerBuffer;
        SerialBT.println(answerBuffer);

        Serial.print("Resposta enviada: ");
        Serial.println(answerBuffer);
      }

      buffer = ""; // limpa buffer
    } else {
      buffer += c;
    }
  }

  // =========================
  // Enviar do Serial → celular
  // =========================
  while (Serial.available()) {
    char c = Serial.read();
    SerialBT.write(c);
  }
}