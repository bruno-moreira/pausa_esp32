#include <MFRC522v2.h>
#include <MFRC522DriverSPI.h>
// #include <MFRC522DriverI2C.h>
#include <MFRC522DriverPinSimple.h>
#include <MFRC522Debug.h>
#include <Wire.h>

// Learn more about using SPI/I2C or check the pin assigment for your board: https://github.com/OSSLibraries/Arduino_MFRC522v2#pin-layout
MFRC522DriverPinSimple ss_pin(5);

MFRC522DriverSPI driver{ss_pin}; // Create SPI driver
// MFRC522DriverI2C driver{}; // Create I2C driver
MFRC522 mfrc522{driver};

void setup()
{
    Serial.begin(9600);
    while (!Serial)
        ;
    mfrc522.PCD_Init();
    MFRC522Debug::PCD_DumpVersionToSerial(mfrc522, Serial); // Show details of PCD - MFRC522 Card Reader details.
    Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
}

void loop()
{

    if (!mfrc522.PICC_IsNewCardPresent())
    {
        return;
    }

    if (!mfrc522.PICC_ReadCardSerial())
    {
        return;
    }

    String tag_rfid_dec = "";
    String tag_rfid_hex = "";

    for (byte i = 0; i < mfrc522.uid.size; i++)
    {
        if (mfrc522.uid.uidByte[i] < 0x10)
        {
            tag_rfid_hex += "0";
            tag_rfid_dec += "0";
        }
        tag_rfid_hex += String(mfrc522.uid.uidByte[i], HEX);
        tag_rfid_dec += String(mfrc522.uid.uidByte[i], DEC);
    }

    Serial.println("TAG HEX: " + tag_rfid_hex);
    Serial.println("TAG DEC MSB: " + tag_rfid_dec);

    uint32_t uidLittleEndian = 0;
    for (byte i = 0; i < mfrc522.uid.size; i++)
    {
        uidLittleEndian |= (mfrc522.uid.uidByte[i] << (8 * i));
    }

    Serial.print("TAG DEC LSB): ");
    Serial.println(uidLittleEndian);

    delay(2000);
}
