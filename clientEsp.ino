#define ag_ismi "AndroidAP123"
#define ag_sifresi "botf6654"


String msg = "";

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);

  Serial.println("AT"); //ESP modülümüz ile bağlantı kurulup kurulmadığını kontrol ediyoruz.
    
    delay(3000); //ESP ile iletişim için 3 saniye bekliyoruz.
   
    while(!Serial.find("OK")){         //esp modülü ile bağlantıyı kurabilmişsek modül "AT" komutuna "OK" komutu ile geri dönüş yapıyor.
      Serial.println("AT"); //ESP modülümüz ile bağlantı kurulup kurulmadığını kontrol ediyoruz.
      delay(3000); //ESP ile iletişim için 3 saniye bekliyoruz. 
    }
  
  Serial.println("AT+CWMODE=1"); //esp modülümüzün WiFi modunu STA şekline getiriyoruz. Bu mod ile modülümüz başka ağlara bağlanabilecek.
  delay(2000);
  String baglantiKomutu=String("AT+CWJAP=\"")+ag_ismi+"\",\""+ag_sifresi+"\"";
  Serial.println(baglantiKomutu);
      
  delay(5000);
  
  Serial.println("AT+CIPSTART=\"TCP\",\"192.168.43.225\",8081");
  delay(3000);

  if(Serial.find("OK")){
    if(Serial.available()>0){  //Eğer gelen veri varsa gir
    Serial.write(Serial.read());
    }
  }

  while(!Serial.find("OK")){
    Serial.println("AT+CIPSTART=\"TCP\",\"192.168.43.225\",8081");
    delay(3000);
  }


}

void loop() {
  // put your main code here, to run repeatedly:
  
  if(Serial.available()>0){    
    //Serial.println(Serial.readString());
    if(Serial.find("+IPD,")){
      msg = Serial.readString();
      uint8_t cutIndex = msg.indexOf(':');
      msg = msg.substring(cutIndex+1);
      //msg.substring(10);
      Serial.println(msg);
      Serial.println(msg.length());
    }
  }

  Serial.println("AT+CIPSEND=6");
  delay(750);
  Serial.println("AKINCI");


  /*if(esp.available()>0){
    Serial.println(esp.readString());
  }*/

  

}
