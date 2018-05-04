#define ag_ismi "AndroidAP123"
#define ag_sifresi "botf6654"

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define CONNECT 0
#define RECONNECT 1


#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 

static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

String msg = "";

const uint8_t xPin = A0; // A0-A5 analog pinlerinden herhangi birine bağlanabilir.
const uint8_t yPin = A1; // A0-A5 analog pinlerinden herhangi birine bağlanabilir.
const uint8_t butonPin = 2; // Joystick buton pini arduino bağlantısı (Joystick SW çıkışı)
const uint8_t id = 0;
 
uint16_t xPozisyonu = 0;
uint16_t yPozisyonu = 0;
uint16_t butonDurum = 0;


void setup() {

  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  pinMode(butonPin, INPUT_PULLUP);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
    // init done
    
    // Show image buffer on the display hardware.
    // Since the buffer is intialized with an Adafruit splashscreen
    // internally, this will display the splashscreen.
    display.display();
    delay(2000);
  
    // Clear the buffer.
    display.clearDisplay();
  
    // draw a single pixel
    display.drawPixel(10, 10, WHITE);
    // Show the display buffer on the hardware.
    // NOTE: You _must_ call display after making any drawing commands
    // to make them visible on the display hardware!
    display.display();
    delay(2000);
    display.clearDisplay();

  Serial.begin(115200);

  Serial.println("AT"); //ESP modülümüz ile bağlantı kurulup kurulmadığını kontrol ediyoruz.
    
    delay(3000); //ESP ile iletişim için 3 saniye bekliyoruz.
   
    while(!Serial.find("OK")){         //esp modülü ile bağlantıyı kurabilmişsek modül "AT" komutuna "OK" komutu ile geri dönüş yapıyor.
      Serial.println("AT"); //ESP modülümüz ile bağlantı kurulup kurulmadığını kontrol ediyoruz.
      delay(3000); //ESP ile iletişim için 3 saniye bekliyoruz. 
    }
  
  Serial.println("AT+CWMODE=1"); //esp modülümüzün WiFi modunu STA şekline getiriyoruz. Bu mod ile modülümüz başka ağlara bağlanabilecek.
  delay(2000);
  //String baglantiKomutu=String("AT+CWJAP=\"")+ag_ismi+"\",\""+ag_sifresi+"\"";
  //Serial.println(baglantiKomutu);
  Serial.println(String("AT+CWJAP=\"")+ag_ismi+"\",\""+ag_sifresi+"\"");
      
  delay(5000);
  
  Serial.println("AT+CIPSTART=\"TCP\",\"192.168.43.225\",8081");
  delay(3000);


  while(!Serial.find("CONNECT")){
    Serial.println("AT+CIPSTART=\"TCP\",\"192.168.43.225\",8081");
    delay(3000);
  }

  Serial.println("AT+CIPSEND=8");
  delay(400);
  Serial.println("Connect," + id);

}

void loop() {

  Serial.println("AT+CIPSTATUS");
  
  
  xPozisyonu = analogRead(xPin);
  yPozisyonu = analogRead(yPin);
  butonDurum = digitalRead(butonPin);
  
  if(Serial.available()>0){    
    //Serial.println(Serial.readString());
    if(Serial.find("+IPD,")){
      msg = Serial.readString();
      uint8_t cutIndex = msg.indexOf(':');
      msg = msg.substring(cutIndex+1);
      Serial.println(msg);
      testscrolltext(msg);
      Serial.println(msg.length());
    }
    else if(Serial.find("STATUS:")){
      msg = Serial.readString();
      if(msg != "3"){
        Serial.println("AT+CIPSEND=8");
        delay(400);
        Serial.println("Connect" + id);
      }
    }
  }

  if(xPozisyonu > 900){
    //Sağ
    Serial.println("AT+CIPSEND=1");
    delay(400);
    Serial.println("1");
  }
  
  else if(xPozisyonu < 125){
    //Sol
    Serial.println("AT+CIPSEND=1");
    delay(400);
    Serial.println("2");
  }
  
  else if(yPozisyonu > 900){
    //Yukarı
    Serial.println("AT+CIPSEND=1");
    delay(400);
    Serial.println("3");
  }
  
  else if(yPozisyonu < 125){
    //Asagi
    Serial.println("AT+CIPSEND=1");
    delay(400);
    Serial.println("4");
  }
  
  else if(butonDurum == 0){
    //Secme butonu
    Serial.println("AT+CIPSEND=1");
    delay(400);
    Serial.println("5");
  }
}

void testscrolltext(String param) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10,0);
  display.println(param);
  display.display(); 
}
