#include <Servo.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN); // Instancia del lector RFID

#define CNY_PIN A0
#define ULTRASONIC_TRIGGER_PIN 7
#define ULTRASONIC_ECHO_PIN 6
#define PIR_PIN 2
#define LED_PIN 3

#define BUZZER_PIN 5

Servo myServo; // Instancia del servo

int movementCount = 0;

void setup() {
  Serial.begin(9600);
  SPI.begin(); // Inicia la comunicación SPI
  mfrc522.PCD_Init(); // Inicializa el lector RFID

  pinMode(CNY_PIN, INPUT);
  pinMode(ULTRASONIC_TRIGGER_PIN, OUTPUT);
  pinMode(ULTRASONIC_ECHO_PIN, INPUT);
  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  myServo.attach(8); // Pin del servo
  myServo.write(180); // Mueve el servo a 0 grados al inicio
}

void loop() {
  // Verifica si se detecta una etiqueta RFID
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    // Mueve el servo a 90 grados
    myServo.write(90);
    delay(10000);
    myServo.write(0); // Vuelve a la posición inicial
    mfrc522.PICC_HaltA(); // Detiene la comunicación con la tarjeta RFID
  }

  // Verifica la distancia detectada por el sensor ultrasónico
  long duration, distance;
  digitalWrite(ULTRASONIC_TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRASONIC_TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_TRIGGER_PIN, LOW);
  duration = pulseIn(ULTRASONIC_ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2;

  // Activa la alarma si algo está a menos de 5 cm
  if (distance < 5) {
    Serial.println("¡ALERTA! Objeto detectado a menos de 5 cm");
    activateBuzzer(); // Activa el zumbador como alarma
  } else {
    deactivateBuzzer(); // Desactiva el zumbador si no hay objeto cercano
  }
  
  if (digitalRead(PIR_PIN) == HIGH) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }
  
  delay(100); // Pequeña pausa entre iteraciones
}

void activateBuzzer() {
  digitalWrite(BUZZER_PIN, HIGH); // Enciende el zumbador
}

void deactivateBuzzer() {
  digitalWrite(BUZZER_PIN, LOW); // Apaga el zumbador
}
