#include "rfid.h"
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 5
#define RST_PIN 22

MFRC522 rfid(SS_PIN, RST_PIN);

void initRFID() {
  SPI.begin(18, 19, 23, SS_PIN);
  rfid.PCD_Init();
  Serial.println("RFID Ready - bring card close to reader");
}

String checkRFID() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    return "";

  String uid = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    // Đảm bảo mỗi byte có 2 ký tự HEX (ví dụ 0x0A -> "0A")
    if (rfid.uid.uidByte[i] < 0x10) uid += "0";
    uid += String(rfid.uid.uidByte[i], HEX);
  }

  uid.toUpperCase();   // đồng bộ chữ hoa
  uid.trim();

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();

  Serial.print("RFID UID: ");
  Serial.println(uid);

  return uid;
}
