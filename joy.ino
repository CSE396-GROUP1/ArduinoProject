#define CONNECT_AP "AT+CWJAP=\"AndroidAP123\",\"botf6654\""
#define START_SOCKET "AT+CIPSTART=\"TCP\",\"192.168.43.225\",8081"

#define DISCONNECT 0
#define CONNECT 1

const uint8_t PROGMEM xPin = A0; // A0-A5 analog pinlerinden herhangi birine bağlanabilir.
const uint8_t PROGMEM yPin = A1; // A0-A5 analog pinlerinden herhangi birine bağlanabilir.
const uint8_t PROGMEM butonPin = 2; // Joystick buton pini arduino bağlantısı (Joystick SW çıkışı)
uint8_t connectStatus = 0;
 
uint16_t xPozisyonu = 0;
uint16_t yPozisyonu = 0;
uint8_t butonDurum = 0;

unsigned long pre_time = 0;
uint8_t interval = 100;

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(75);

  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  pinMode(butonPin, INPUT_PULLUP);

  setupEsp();
}

void loop() {
  if(connectStatus != CONNECT){
      connectToServer();
  }

  

  if(Serial.available()>0){
      if(Serial.find("+IPD,")){
        Serial.println(Serial.readStringUntil(','));
      }
  }
      
  xPozisyonu = analogRead(xPin);
  yPozisyonu = analogRead(yPin);
  butonDurum = digitalRead(butonPin);

  unsigned long curr_time = millis();
  if(abs(curr_time - pre_time) >= interval)
  {
    pre_time = curr_time;

    if(xPozisyonu > 900){//Sağ
      Serial.println("Sag");
    }
    else if(xPozisyonu < 125){//Sol
      Serial.println("Sol");
    }
    else if(yPozisyonu > 900){//Yukarı
      Serial.println("Yukari");
      Serial.println("AT+CIPSEND=5");
      delay(50);
      Serial.println("83dad");      
    }
    else if(yPozisyonu < 125){//Asagi
      Serial.println("Asagi");
    }
  }
}

bool connectToServer(){
  while(!Serial.find("CONNECT")){
    Serial.println(F(START_SOCKET));
    delay(3000);
    if(Serial.find("CONNECT")){
      return true;
    }
  }
  return false;
} 

void setupEsp(){
  Serial.println(F("AT"));
    
  delay(3000);
   
  while(!Serial.find("OK")){ 
    Serial.println(F("AT"));
    delay(3000);
  }
  
  Serial.println(F("AT+CWMODE=1"));
  delay(2000);
  
  Serial.println(F(CONNECT_AP));
  delay(5000);

  while(!Serial.find("OK")){ 
    Serial.println(F(CONNECT_AP));
    delay(3000);
  }

  if(connectToServer()){
    Serial.println(F("AT+CIPSEND=9"));
    delay(400);
    Serial.println(F("Connect,0"));
    connectStatus = CONNECT;
  }
}

String readSocket()
{
  char tmp[5];
  for(uint8_t i = 0; i < 5; i++)
    tmp[i] = (char)Serial.read();

  tmp[5] = '\0';
  Serial.println("chptr");
  Serial.println(tmp);
  return String(tmp);
}


