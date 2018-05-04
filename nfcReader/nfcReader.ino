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
  String guvenlikKodu="A123";
  String kapiNo="A2";
  String musteri="MUSTERI";
  String kapi="KAPI";
  String sifirla="SIFIRLA";
  String personel="PERSONEL";


void Ege(String mesaj){
    int i=0,j=0,k=0;
    String array[6]={"","","","","",""};
    for(i=0;i<mesaj.length();i++){
        if(mesaj.charAt(i)==' '){
            array[k]=mesaj.substring(j,i);
            k++;
            j=i+1;
          }
      }

      //1 => tür (sıfırlama/kapı aç)
      //2 => kapı no
      //3 => personel tür (müşteri/personel)
      //4 => güvenlik kodu
      //5 => kişi bilgisi
      Serial.println("tür= "+array[1]);
      Serial.println("kapı no = "+array[2]);
      Serial.println("personel tür= "+array[3]);
      Serial.println("güvenlik kodu= "+array[4]);
      Serial.println("kişi bilgisi= "+array[5]);

      //KAPI - SIFIRLA () A2 () MUSTERI - PERSONEL () A123 () KİŞİ BİLGİSİ
      //KAPI () A2 () MUSTERI () A123 () KİŞİ BİLGİSİ

      
      
      if(sifirla==array[1] && kapiNo==array[2]){
          guvenlikKodu=array[4];
        }
        else if(kapi==array[1] && kapiNo==array[2] && guvenlikKodu==array[4] && array[3]=="MUSTERI"){
          musteriOdada=!musteriOdada;
          if(musteriOdada){
            Serial.println(array[3]+" "+array[5]+" giriş yaptı");
            }
            else{
              Serial.println(array[3]+" "+array[5]+" çıkış yaptı");
              }          
        }
        else if(kapi==array[1] && kapiNo==array[2] && guvenlikKodu==array[4] && array[3]=="PERSONEL"){
          temizlikciOdada=!temizlikciOdada;
          if(temizlikciOdada){
            Serial.println(array[3]+" "+array[5]+" giriş yaptı");
            }
            else{
              Serial.println(array[3]+" "+array[5]+" çıkış yaptı");
              }          
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
