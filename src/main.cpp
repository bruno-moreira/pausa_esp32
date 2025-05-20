#include <WiFi.h>
#include <HTTPClient.h>
#include <MFRC522v2.h>
#include <MFRC522DriverSPI.h>
#include <MFRC522DriverPinSimple.h>
#include <MFRC522Debug.h>
#include <Wire.h>

// WiFi config
const char* ssid = "Henet Casa";
const char* password = "moreira1994";

// RFID
MFRC522DriverPinSimple ss_pin(5);
MFRC522DriverSPI driver{ss_pin};
MFRC522 mfrc522{driver};

void setup()
{
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    Serial.print("Conectando ao Wi-Fi");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWi-Fi conectado!");

    mfrc522.PCD_Init();
    MFRC522Debug::PCD_DumpVersionToSerial(mfrc522, Serial);
    Serial.println(F("Aproxime o cartão RFID..."));
}

void loop()
{
 if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial())
        return;

    String tag_rfid_hex = "";
    for (byte i = 0; i < mfrc522.uid.size; i++)
    {
        if (mfrc522.uid.uidByte[i] < 0x10)
            tag_rfid_hex += "0";
        tag_rfid_hex += String(mfrc522.uid.uidByte[i], HEX);
    }

    tag_rfid_hex.toUpperCase();
    Serial.println("TAG HEX: " + tag_rfid_hex);

    // Envia via HTTP GET
    if (WiFi.status() == WL_CONNECTED)
    {
        HTTPClient http;
        String url = "http://192.168.1.158:3000/api/praca/insert?cracha_rfid=" + tag_rfid_hex + "&pausa_id=1";
        http.begin(url);
        int httpCode = http.GET();

        if (httpCode > 0)
        {
            String payload = http.getString();
            Serial.println("Resposta do servidor: " + payload);
        }
        else
        {
            Serial.println("Erro na requisição HTTP: " + http.errorToString(httpCode));
        }

        http.end();
    }
    else
    {
        Serial.println("Wi-Fi não conectado.");
    }

    delay(3000);  // Aguardar para evitar múltiplas leituras seguidas
}
