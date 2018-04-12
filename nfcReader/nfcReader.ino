#include "SPI.h"
#include "PN532_SPI.h"
#include "snep.h"
#include "NdefMessage.h"

#include "Wire.h"
#include "LiquidCrystal.h"

PN532_SPI pn532spi(SPI, 10);
SNEP nfc(pn532spi);
uint8_t ndefBuf[128];

  bool temizlikciOdada = false;
  bool musteriOdada = false;

bool Odakontrol (String mesaj){
  //oda numarasını kontrol edecek fonksiyondur
  if(mesaj.equals("A2")){
      return true;
    }
    return false;
  }

void Ege(String mesaj){
    String tarih = mesaj.substring(0,9);
    String icerik = mesaj.substring(9,mesaj.length());
    int i=0,j=0,k=0;
    String array[6]={"","","","","",""};
    for(i=0;i<icerik.length();i++){
        if(icerik.charAt(i)==' '){
            array[k]=icerik.substring(j,i);
            k++;
            j=i+1;
          }
      }
      if(Odakontrol(array[0])){
         if(array[1].equals("TEMIZLIKCI") && !temizlikciOdada){
          //temizlikçi odaya giris yapsın
            temizlikciOdada = true;
            Serial.println(tarih+" "+array[1]+" "+array[2]+" odaya giriş yaptı");
          }
          else if (array[1].equals("TEMIZLIKCI") && temizlikciOdada){
            //temizlikçi odadan çıkış yapsın
            temizlikciOdada = false;
            Serial.println(tarih+" "+array[1]+" "+array[2]+" odadan çıkış yaptı");
            }
            else if (array[1].equals("MUSTERI") && !musteriOdada){
            //müşteri odaya giriş yapsın
            musteriOdada = true;
            Serial.println(tarih+" "+array[1]+" "+array[2]+" odaya giriş yaptı");
            }
            else if (array[1].equals("MUSTERI") && musteriOdada){
            //müşteri odadan çıkış yapsın
            musteriOdada = false;
            Serial.println(tarih+" "+array[1]+" "+array[2]+" odadan çıkış yaptı");
            }
      }
      else{
        Serial.println("Oda giriş izniniz bulunmuyor.");
        }
  }

void setup() {
    Serial.begin(9600);
   // Serial.println("EGE ŞENKUL NFC OKUMA ÖRNEK KODUDUR");
}

void loop() {
    //Serial.println("Cihazı yakınlaştırın");
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
