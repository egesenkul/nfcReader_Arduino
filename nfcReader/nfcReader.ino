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
    String array[10]; // array tanımlama örneğidir
  }

void setup() {
    Serial.begin(9600);
    // set up the LCD's number of rows and columns: 
    analogWrite(6,10);
    Serial.println("EGE ŞENKUL NFC OKUMA ÖRNEK KODUDUR");
}

void loop() {
    Serial.println("Cihazı yakınlaştırın");
    int msgSize = nfc.read(ndefBuf, sizeof(ndefBuf));
    if (msgSize > 0) {
        NdefMessage msg  = NdefMessage(ndefBuf, msgSize);
        //mesaj.print();
            
        NdefRecord record = msg.getRecord(0);

        int payloadLength = record.getPayloadLength();
        byte payload[payloadLength];
        record.getPayload(payload);        
        
        // The TNF and Type are used to determine how your application processes the payload
        // There's no generic processing for the payload, it's returned as a byte[]
        int startChar = 0;        
        if (record.getTnf() == TNF_WELL_KNOWN && record.getType() == "T") { // text message
          // skip the language code
          startChar = payload[0] + 1;
        } else if (record.getTnf() == TNF_WELL_KNOWN && record.getType() == "U") { // URI
          // skip the url prefix (future versions should decode)
          startChar = 1;
        }
                          
        // Force the data into a String (might fail for some content)
        // Real code should use smarter processing
        String payloadAsString = "";
        for (int c = startChar; c < payloadLength; c++) {
          payloadAsString += (char)payload[c];
        }
          
        // print on the LCD display
       // lcd.setCursor(1, 5);
        //lcd.print(payloadAsString);
        Ege(payloadAsString);
        
    } else {
        Serial.println("Başarısız");
    }
    
    delay(3000);
    
}
