#include "Maze.h"

using namespace DisplaySetup;
using namespace DisplayOperation;
using namespace BoardOperation;
using namespace StateOperation;
using namespace Test;

#define CONNECT_AP "AT+CWJAP=\"GOZTEPE\",\"hasan3545\""
#define START_SOCKET "AT+CIPSTART=\"TCP\",\"192.168.1.26\",8081"

#define ISCONNECT 1

#define DEBUG 0

#define FAIL '0'
#define SUCCESS '1'
#define CONNECT '2'
#define WAIT_FOR_DRAWING '3'
#define DRAWING '4'
#define WAIT_FOR_MOVING '5'
#define MOVING '6'
#define FINISHING '7'
#define GAMEOVER '8'
#define RESTARTING '9'
#define NEWGAME 'N'

#define R '1'
#define L '2'
#define U '3'
#define D '4'

typedef enum GAMESTATE{
  DRAW_M,
  MOVE_M,
  GAMEOVER_M,
  FINISH_M,
  WAIT_FOR_DRAWING_M,
  WAIT_FOR_MOVING_M,
  RESTART_M,
  NEWGAME_M
};

char board[ROW][COL];             
int16_t cursorXPosition = 11;
int16_t cursorYPosition = 0;
int8_t isCursorActive = 1;

const uint8_t PROGMEM xPin = A0; // A0-A5 analog pinlerinden herhangi birine bağlanabilir.
const uint8_t PROGMEM yPin = A1; // A0-A5 analog pinlerinden herhangi birine bağlanabilir.
const uint8_t PROGMEM butonPin = 2; // Joystick buton pini arduino bağlantısı (Joystick SW çıkışı)
uint8_t connectStatus = 0;
 
uint16_t xPozisyonu = 0;
uint16_t yPozisyonu = 0;
uint8_t butonDurum = 0;

unsigned long preTime = 0;
uint8_t interval = 185;

bool sendStatus = true;

GAMESTATE gameState;


void setup() {

  Serial.begin(115200);

  Serial.setTimeout(50);

  initialize();
  initializeBoard(board);
  refreshBoard();
  

  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  pinMode(butonPin, INPUT_PULLUP);
  
  setupEsp();
}

void loop() {

  refreshBoard();
  
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
  
        if(msg[0] == WAIT_FOR_DRAWING){
          gameState = WAIT_FOR_DRAWING_M;
        }
        else if(msg[0] == DRAWING){
          gameState = DRAW_M;
        }
        else if(msg[0] == WAIT_FOR_MOVING){
          gameState = WAIT_FOR_MOVING_M;
        }
        else if(msg[0] == MOVING){
          gameState = MOVE_M;
          clearBoard(board);
        }
        else if(msg[0] == FINISHING){
          gameState = FINISH_M;
        }
        else if(msg[0] == GAMEOVER){
          gameState = MOVE_M;
          clearBoard(board);
        }
        else if(msg[0] == NEWGAME){
          gameState = RESTART_M;
          clearBoard(board);
        }
        else if(msg[0] == SUCCESS){
          sendStatus = false;
          if(gameState == DRAW_M || gameState == MOVE_M)
            moveOnLed(msg[1]);
        }
      }
    }

    if(gameState == DRAW_M || gameState == MOVE_M){
      if(sendStatus)
        doAction(xPozisyonu,yPozisyonu,gameState);
    }
    
    if(gameState == FINISH_M){
      if(sendStatus)
        doAction(butonDurum,0,gameState);
    }
    else if(gameState == RESTART_M){
      gameState = DRAW_M;
    }
    /*else if(gameState == NEWGAME_M){
      gameState = DRAW_M;
      //board sifirlanacak!!!
    }*/
  }
  sendStatus = true;
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

void clearBoard(char board[][COL]){
  cursorXPosition = ROW-1;
  cursorYPosition = 0;
  
  for(uint8_t i = 0; i<ROW; i++){
    for(uint8_t j=0; j<COL; j++){

        board[i][j] = 'O';
      
    }
  }
  board[ROW - 1][0] = 'X';
}

void moveOnLed(char direct){
  if(direct == R){
    goRight(board, &cursorXPosition, &cursorYPosition);
  }
  else if(direct == L){
    goLeft(board, &cursorXPosition, &cursorYPosition);
  }
  else if(direct == U){
    goUp(board, &cursorXPosition, &cursorYPosition); 
  }
  else if(direct == D){
    goDown(board, &cursorXPosition, &cursorYPosition); 
  }
}

void doAction(uint16_t xPoz, uint16_t yPoz, GAMESTATE state){
 unsigned long currentTime = millis();
 String result =  "";
 if(abs(currentTime - preTime) >= interval){
  preTime = currentTime;
  if(state == DRAW_M){
    if(xPoz > 900){//Sağ
        result += DRAWING;
        result += R;
        sendServer(result);
        //goRight(board, &cursorXPosition, &cursorYPosition);
        
    }
    else if(xPoz < 125){//Sol
      result += DRAWING;
      result += L;
      sendServer(result);
      //goLeft(board, &cursorXPosition, &cursorYPosition);
      
    }
    else if(yPoz > 900){//Yukarı
      result += DRAWING;
      result += U;
      sendServer(result);
      //goUp(board, &cursorXPosition, &cursorYPosition);
      
    }
    else if(yPoz < 125){//Asagi
      result += DRAWING;
      result += D;
      sendServer(result);
      //goDown(board, &cursorXPosition, &cursorYPosition);
      
    }
  }
  else if(state == MOVE_M){
    if(xPoz > 900){//Sağ
        result += MOVING;
        result += R;
        sendServer(result);
        //goRight(board, &cursorXPosition, &cursorYPosition);
    }
    else if(xPoz < 125){//Sol
      result += MOVING;
      result += L;
      sendServer(result);
      //goLeft(board, &cursorXPosition, &cursorYPosition);
    }
    else if(yPoz > 900){//Yukarı
      result += MOVING;
      result += U;
      sendServer(result);
      //goUp(board, &cursorXPosition, &cursorYPosition);
    }
    else if(yPoz < 125){//Asagi
      result += MOVING;
      result += D;
      sendServer(result);
      //goDown(board, &cursorXPosition, &cursorYPosition);
    }
  }
  else if(state = FINISH_M){
    if(xPoz == 0){
      result += RESTARTING;
      sendServer(result);
    }
  }
 }
}

void refreshBoard(){

  displayBoard(board, cursorXPosition, cursorYPosition, isCursorActive);
    
  changeCursorState(&isCursorActive);
  
  displayBoard(board, cursorXPosition, cursorYPosition, isCursorActive);
  
}


void sendServer(String data){
  Serial.println("AT+CIPSEND=2");
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
