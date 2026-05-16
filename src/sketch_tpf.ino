#include <SPI.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <MFRC522.h>

#define PIN_RC522_RESET 9
#define PIN_RC522_SLAVE_SELECT 10
#define T_RC522_ESPERA 500
#define PIN_RGB_AZUL 2
#define PIN_RGB_VERDE 3
#define PIN_RGB_ROJO 4

MFRC522 mfrc522(PIN_RC522_SLAVE_SELECT, PIN_RC522_RESET); // Crear objeto RC522
LiquidCrystal_I2C lcd(0x27, 16, 2); // 0x27 o 0x3F, según modelo

String tarjetasId[] = {
  "243522234",  // tarjeta buena (no tengo más de esta freq)
  "456313222" // valor cualquiera
};

void setup() {
  Serial.begin(9600);
  SPI.begin();

  // Configuración RC522
  mfrc522.PCD_Init();
  pinMode(PIN_RGB_AZUL, OUTPUT);
  pinMode(PIN_RGB_VERDE, OUTPUT);
  pinMode(PIN_RGB_ROJO, OUTPUT);

  // Inicializar Módulo Led RGB
  digitalWrite(PIN_RGB_AZUL, LOW);
  digitalWrite(PIN_RGB_VERDE, LOW);
  digitalWrite(PIN_RGB_ROJO, LOW);

  // Inicializar LCD 1602
  lcd.init();
  lcd.setCursor(0,0);
}

void loop() {
  bool huboLectura  = false;
  String resultado = "";

  // 1 - Leo tarjeta
  String datosTarjeta = "";
  int idLeido = 0;

  if (mfrc522.PICC_IsNewCardPresent()) {
    if (mfrc522.PICC_ReadCardSerial()) {
      digitalWrite(PIN_RGB_AZUL, HIGH);
      byte tamUidTarjeta = mfrc522.uid.size;
      Serial.print("ID: ");
      for (byte indice = 0; indice < tamUidTarjeta; indice++) {
        datosTarjeta.concat(String(mfrc522.uid.uidByte[indice]));
      }
      Serial.println(datosTarjeta);
      mfrc522.PICC_HaltA();
    }
    delay(T_RC522_ESPERA);
    digitalWrite(PIN_RGB_AZUL, LOW);
    huboLectura = true;
  }

  // 2 - Si hubo lectura, busco en las tarjetas empadronadas
  if (huboLectura) {
    int cantTarjetas = sizeof(tarjetasId)/sizeof(tarjetasId[0]);
    bool encontrada = false;

    Serial.println(cantTarjetas);
    for (int indice = 0; (indice < cantTarjetas) && (!encontrada); indice++) {
      if (datosTarjeta == tarjetasId[indice]) {
        encontrada = true;
      }
    }

    Serial.print("Encontrada? ");
    Serial.println(encontrada);

    if (encontrada) {
      resultado = "AUTORIZADO";
      digitalWrite(PIN_RGB_VERDE, HIGH);
      encontrada = false;
    } else {
      if (datosTarjeta == "") {
        resultado = "NO_LEIDO";
        digitalWrite(PIN_RGB_ROJO, HIGH);
        digitalWrite(PIN_RGB_VERDE, HIGH);
      } else {
        resultado = "SIN_ACCESO";
        digitalWrite(PIN_RGB_ROJO, HIGH);
      }
    }
    digitalWrite(PIN_RGB_VERDE, LOW);
    digitalWrite(PIN_RGB_ROJO, LOW);

    // 3 - Muestro en LCD la información
    
    delay(1500);

    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0,0);
    lcd.print(datosTarjeta);
    lcd.setCursor(0,1);
    lcd.print(resultado);
    delay(1500);
    lcd.clear();
    lcd.noBacklight();
  }
}
