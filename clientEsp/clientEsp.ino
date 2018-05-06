#include "Bil396_OLED.h"

#define CONNECT_AP "AT+CWJAP=\"TurkTelekom_T4A6A\",\"JY7s22vx\""
#define START_SOCKET "AT+CIPSTART=\"TCP\",\"192.168.1.111\",8081"

#define DISCONNECT 0
#define CONNECT 1

#define DEBUG 0
#define SIZE_X 10
#define SIZE_Y 10

typedef enum DIRECTION {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

String msg = "";

const uint8_t PROGMEM xPin = A0; // A0-A5 analog pinlerinden herhangi birine bağlanabilir.
const uint8_t PROGMEM yPin = A1; // A0-A5 analog pinlerinden herhangi birine bağlanabilir.
const uint8_t PROGMEM butonPin = 2; // Joystick buton pini arduino bağlantısı (Joystick SW çıkışı)
uint8_t connectStatus = 0;
 
uint16_t xPozisyonu = 0;
uint16_t yPozisyonu = 0;
uint8_t butonDurum = 0;

//int loopCounter = 0;
uint8_t isCursorActive = 0;
uint8_t x=0;
uint8_t y=0;
char board[10][10];

DIRECTION direction;


void setup() {

  initialize();//Serial.begin(115200);

  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  pinMode(butonPin, INPUT_PULLUP);

  

  setupEsp();

  board[0][0] = 'X';
  clearDisplay();
}

void loop() {
  clearDisplay();
  
  xPozisyonu = analogRead(xPin);
  yPozisyonu = analogRead(yPin);
  butonDurum = digitalRead(butonPin);

  if(connectStatus == CONNECT){

    if(xPozisyonu > 900){//Sağ
      sendServer('1');
    }
  
  else if(xPozisyonu < 125){//Sol
      sendServer('2');
    }
  
  else if(yPozisyonu > 900){//Yukarı
      sendServer('3');
    }
  
  else if(yPozisyonu < 125){//Asagi
      sendServer('4');
    }
  
  else if(butonDurum == 0){//Secme butonu
      sendServer('5');
    }
  }

  if(Serial.available()>0){    
  
    if(Serial.find(F("+IPD,"))){
      msg = Serial.readString();
      uint8_t cutIndex = msg.indexOf(':');
      msg = msg.substring(cutIndex+1);
      //Serial.println(F(msg));
      //testscrolltext(msg);
      if(msg[0] == 'O' && msg[1] == 'K'){
        if (msg[3] == '1'){
          if(DEBUG)
            Serial.println(F("RIGHT"));
          direction = RIGHT;
          if(y<SIZE_Y-1)
            board[x][++y] = 'X';      
        }
        else if (msg[3] == '2'){
          if(DEBUG)
            Serial.println(F("LEFT"));
        direction = LEFT; 
        if(y>0)
          board[x][--y] = 'X';      
        }
        else if (msg[3] == '3'){
          if(DEBUG)
            Serial.println(F("UP"));
        direction = UP;
        if(x>0)
          board[--x][y] = 'X';      
        }
        else if (msg[3] == '4'){
          if(DEBUG)
            Serial.println(F("DOWN"));
        direction = DOWN;
        if(x<SIZE_X-1)
        board[++x][y] = 'X';
        }
       }
    }
  } 

 board[x][y] = 'O';
  displayBoard(SIZE_X, SIZE_Y, board, 'X', x, y, isCursorActive);
  board[x][y] = 'X';
  if(isCursorActive) {
    isCursorActive = 0;
  } else {
    isCursorActive = 1;
  }
  
  delay(200);
  /*Serial.println(F("AT+CIPSTATUS"));

  //delay(150);

  if(Serial.find("STATUS:")){
    msg = Serial.readString();
    if(msg[0] != '3'){
      if(connectToServer()){
        Serial.println("AT+CIPSEND=9");
        delay(400);
        Serial.println("Connect," + (String)id);
      }
      else
        connectStatus = DISCONNECT;
    }
    else if (msg[0] == '3'){
      connectStatus = CONNECT;
    }
  }*/
}


/*void testscrolltext(String param) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10,0);
  display.println(param);
  display.display(); 
}*/

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

void sendServer(char data){
  Serial.println(F("AT+CIPSEND=1"));
  delay(400);
  Serial.println(data);
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

