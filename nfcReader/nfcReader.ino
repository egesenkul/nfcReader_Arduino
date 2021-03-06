#include "SPI.h"
#include "PN532_SPI.h"
#include "snep.h"
#include "NdefMessage.h"

#include "Wire.h"
#include "LiquidCrystal.h"

PN532_SPI pn532spi(SPI, 10);
SNEP nfc(pn532spi);
uint8_t ndefBuf[128];

void Ege(String mesaj){
    String tarih = mesaj.substring(mesaj.indexOf("TARİHİ:")+9,mesaj.indexOf(" MESAJ"));
    String icerik = mesaj.substring(mesaj.indexOf("İÇERİĞİ: ")+14,mesaj.length());
    Serial.println(tarih);
    Serial.println(icerik);
    Serial.println();
  }

void setup() {
    Serial.begin(9600);
    Serial.println("EGE ŞENKUL NFC OKUMA ÖRNEK KODUDUR");
}

void loop() {
    Serial.println("Cihazı yakınlaştırın");
    int msgSize = nfc.read(ndefBuf, sizeof(ndefBuf));
    if (msgSize > 0) {
        NdefMessage msg  = NdefMessage(ndefBuf, msgSize);
            
        NdefRecord record = msg.getRecord(0);

        int payloadLength = record.getPayloadLength();
        byte payload[payloadLength];
        record.getPayload(payload);        
        
        int startChar = 0;        
        if (record.getTnf() == TNF_WELL_KNOWN && record.getType() == "T") { 
          startChar = payload[0] + 1;
        } else if (record.getTnf() == TNF_WELL_KNOWN && record.getType() == "U") { 
          startChar = 1;
        }
                          
        String payloadAsString = "";
        for (int c = startChar; c < payloadLength; c++) {
          payloadAsString += (char)payload[c];
        }
        Ege(payloadAsString);
        
    } else {
        Serial.println("Başarısız");
    }
    
    delay(3000);
    
}
