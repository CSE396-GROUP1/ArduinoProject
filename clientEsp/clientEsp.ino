//#include "Bil396_OLED.h"

#define CONNECT_AP "AT+CWJAP=\"AndroidAP123\",\"botf6654\""
#define START_SOCKET "AT+CIPSTART=\"TCP\",\"192.168.43.225\",8081"

#define DISCONNECT 0
#define CONNECT 1

#define DEBUG 0
#define SIZE_X 10
#define SIZE_Y 10
#define MAXSIZE 100

typedef enum DIRECTION {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

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



//int loopCounter = 0;
uint8_t isCursorActive = 0;
uint8_t x=0;
uint8_t y=0;
char board[10][10];

DIRECTION direction;

GAMESTATE gameState;

String serialResponse = "";


void setup() {

  Serial.begin(115200);

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

  if(connectStatus != CONNECT){

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
        char buf[MAXSIZE];
        msg.toCharArray(buf,sizeof(buf));
        char *p = buf;
        char *str;
        String data[MAXSIZE];
        while ((str = strtok_r(p, ",", &p)) != NULL){
          data[i] = str;
          Serial.println(data[i]);
          i++;
        }
  
        if(data[0] == "WAIT_FOR_DRAWING"){
          gameState = WAIT_FOR_DRAWING_M;
        }
        else if(data[0] == "Drawing"){
          gameState = DRAW_M;
        }
        else if(data[0] == "Moving"){
          gameState = MOVE_M;
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
    
    delay(200);
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
  if(state == DRAW_M){
    if(xPoz > 900){//Sağ
        sendServer("Draw,1",6);
    }
    else if(xPoz < 125){//Sol
      sendServer("Draw,2",6);
    }
    else if(yPoz > 900){//Yukarı
      sendServer("Draw,3",6);
    }
    else if(yPoz < 125){//Asagi
      sendServer("Draw,4",6);
    }
  }
  else if(state == MOVE_M){
    if(xPoz > 900){//Sağ
        sendServer("Move,1",6);
    }
    else if(xPoz < 125){//Sol
      sendServer("Move,2",6);
    }
    else if(yPoz > 900){//Yukarı
      sendServer("Move,3",6);
    }
    else if(yPoz < 125){//Asagi
      sendServer("Move,4",6);
    }
  }
}

void sendServer(String data,int len){
  Serial.println("AT+CIPSEND=6");
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
