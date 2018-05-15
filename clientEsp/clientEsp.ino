//#include "Bil396_OLED.h"

#define CONNECT_AP "AT+CWJAP=\"TurkTelekom_T4A6A\",\"JY7s22vx\""
#define START_SOCKET "AT+CIPSTART=\"TCP\",\"192.168.1.111\",8081"

#define DISCONNECT 0
#define ISCONNECT 1

#define DEBUG 0
#define SIZE_X 10
#define SIZE_Y 10
#define MAXSIZE 20

#define FAIL '0'
#define SUCCESS '1'
#define CONNECT '2'
#define WAIT_FOR_DRAWING '3'
#define DRAWING '4'
#define WAIT_FOR_MOVING '5'
#define MOVING '6'
#define FINISHING '7'
#define GAMEOVER '8'

#define RIGHT '1'
#define LEFT '2'
#define UP '3'
#define DOWN '4'


/*typedef enum DIRECTION {
    UP,
    RIGHT,
    DOWN,
    LEFT
};*/

typedef enum GAMESTATE{
  DRAW_M,
  MOVE_M,
  GAMEOVER_M,
  FINISH_M,
  WAIT_FOR_DRAWING_M
};



const uint8_t PROGMEM xPin = A0; // A0-A5 analog pinlerinden herhangi birine bağlanabilir.
const uint8_t PROGMEM yPin = A1; // A0-A5 analog pinlerinden herhangi birine bağlanabilir.
const uint8_t PROGMEM butonPin = 2; // Joystick buton pini arduino bağlantısı (Joystick SW çıkışı)
uint8_t connectStatus = 0;
 
uint16_t xPozisyonu = 0;
uint16_t yPozisyonu = 0;
uint8_t butonDurum = 0;

unsigned long preTime = 0;
uint8_t interval = 175;



//int loopCounter = 0;
uint8_t isCursorActive = 0;
uint8_t x=0;
uint8_t y=0;
char board[10][10];

//DIRECTION direction;

GAMESTATE gameState;


void setup() {

  Serial.begin(115200);

  Serial.setTimeout(50);

  //initialize();//Serial.begin(115200);

  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  pinMode(butonPin, INPUT_PULLUP);
  
  setupEsp();

  board[0][0] = 'X';
  //clearDisplay();
}

void loop() {
  //clearDisplay();
  
  xPozisyonu = analogRead(xPin);
  yPozisyonu = analogRead(yPin);
  butonDurum = digitalRead(butonPin);

  if(connectStatus != ISCONNECT){

    connectToServer();
    
  }
  else{

    if(Serial.available()>0){    
    
      if(Serial.find("+IPD,")){
        String msg = "";
        uint8_t i = 0;
        msg = Serial.readString();
        uint8_t cutIndex = msg.indexOf(':');
        msg = msg.substring(cutIndex+1);
        Serial.println(msg);
        /*char buf[MAXSIZE];
        msg.toCharArray(buf,sizeof(buf));
        Serial.println(msg);
        char *p = buf;
        char *str;
        String data[MAXSIZE];
        while ((str = strtok_r(p, ",", &p)) != NULL){
          data[i] = str;
          Serial.println(data[i]);
          i++;
        }*/
  
        if(msg[0] == WAIT_FOR_DRAWING){
          gameState = WAIT_FOR_DRAWING_M;
        }
        else if(msg[0] == DRAWING){
          gameState = DRAW_M;
        }
        else if(msg[0] == MOVING){
          gameState = MOVE_M;
        }
        else if(msg[0] == FINISHING){
          gameState = FINISH_M;
          Serial.println(msg[1]);
        }
      }
    }
    if(gameState == DRAW_M){

      doAction(xPozisyonu,yPozisyonu,gameState);
      
    }
    else if(gameState == MOVE_M){
      doAction(xPozisyonu,yPozisyonu,gameState);
    }
  }
  
   /*board[x][y] = 'O';
    displayBoard(SIZE_X, SIZE_Y, board, 'X', x, y, isCursorActive);
    board[x][y] = 'X';
    if(isCursorActive) {
      isCursorActive = 0;
    } else {
      isCursorActive = 1;
    }*/
    
    //delay(200);
    /*Serial.println(F("AT+CIPSTATUS"));
  
    //delay(150);
  
    if(Serial.find("STATUS:")){
      msg = Serial.readString();
      if(msg[0] != '3'){
        if(connectToServer()){
          Serial.println("AT+CIPSEND=9");
          delay(400);
          Serial.println("Reconnect," + (String)id);
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

void doAction(uint16_t xPoz, uint16_t yPoz, GAMESTATE state){
 unsigned long currentTime = millis();
 String result =  "";
 if(abs(currentTime - preTime) >= interval){
  preTime = currentTime;
  if(state == DRAW_M){
    if(xPoz > 900){//Sağ
        result += DRAWING;
        result += RIGHT;
        sendServer(result);
    }
    else if(xPoz < 125){//Sol
      result += DRAWING;
      result += LEFT;
      sendServer(result);
    }
    else if(yPoz > 900){//Yukarı
      result += DRAWING;
      result += UP;
      sendServer(result);
    }
    else if(yPoz < 125){//Asagi
      result += DRAWING;
      result += DOWN;
      sendServer(result);
    }
  }
  else if(state == MOVE_M){
    if(xPoz > 900){//Sağ
        result += MOVING;
        result += RIGHT;
        sendServer(result);
    }
    else if(xPoz < 125){//Sol
      result += MOVING;
      result += LEFT;
      sendServer(result);
    }
    else if(yPoz > 900){//Yukarı
      result += MOVING;
      result += UP;
      sendServer(result);
    }
    else if(yPoz < 125){//Asagi
      result += MOVING;
      result += DOWN;
      sendServer(result);
    }
  }
 } 
}

void sendServer(String data){
  Serial.println("AT+CIPSEND=2");
  //delay(400);
  delay(75);
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
    String result = "";
    Serial.println(F("AT+CIPSEND=2"));
    delay(400);
    result += CONNECT;
    result += '0';
    Serial.println(result);
    connectStatus = ISCONNECT;
  }
}
