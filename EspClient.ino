#define ag_ismi "GOZTEPE"
#define ag_sifresi "hasan3545"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>  //  Seri Haberleşme kütüphanesini ekledik

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


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
SoftwareSerial esp(10, 11); // RX, TX  // BT için ayrıca bir Seri Haberleşmesi

String msg = "";

int xPin = A0; // A0-A5 analog pinlerinden herhangi birine bağlanabilir.
int yPin = A1; // A0-A5 analog pinlerinden herhangi birine bağlanabilir.
int butonPin = 2; // Joystick buton pini arduino bağlantısı (Joystick SW çıkışı)
 
int xPozisyonu = 0;
int yPozisyonu = 0;
int butonDurum = 0;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  esp.begin(115200);

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

  esp.println("AT"); //ESP modülümüz ile bağlantı kurulup kurulmadığını kontrol ediyoruz.
    
    delay(3000); //ESP ile iletişim için 3 saniye bekliyoruz.
   
    while(!esp.find("OK")){         //esp modülü ile bağlantıyı kurabilmişsek modül "AT" komutuna "OK" komutu ile geri dönüş yapıyor.
      esp.println("AT"); //ESP modülümüz ile bağlantı kurulup kurulmadığını kontrol ediyoruz.
      delay(3000); //ESP ile iletişim için 3 saniye bekliyoruz. 
    }
  
  esp.println("AT+CWMODE=1"); //esp modülümüzün WiFi modunu STA şekline getiriyoruz. Bu mod ile modülümüz başka ağlara bağlanabilecek.
  delay(2000);
  String baglantiKomutu=String("AT+CWJAP=\"")+ag_ismi+"\",\""+ag_sifresi+"\"";
  esp.println(baglantiKomutu);
      
  delay(5000);
  
  esp.println("AT+CIPSTART=\"TCP\",\"192.168.1.25\",8081");
  delay(3000);

  if(esp.find("OK")){
    if(esp.available()>0){  //Eğer gelen veri varsa gir
    Serial.write(esp.read());
    }
  }

  while(!esp.find("OK")){
    esp.println("AT+CIPSTART=\"TCP\",\"192.168.1.25\",8081");
    delay(3000);
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  
  xPozisyonu = analogRead(xPin);
  yPozisyonu = analogRead(yPin);
  butonDurum = digitalRead(butonPin);

   if(Serial.available()>0){
    msg = Serial.readString();
    esp.print(msg);
    //Serial.print("Serial writing... : " + msg);
    testscrolltext(msg);
  }


  if(esp.available()>0){
    msg = esp.readString();
    Serial.print(msg);
    testscrolltext(msg);
  }
   
  if(xPozisyonu > 900){
    /*Serial.print("Sağ: ");
    Serial.println(xPozisyonu);*/
    esp.println("AT+CIPSEND=1");
    delay(2000);
    esp.println("1");
  }
  
  else if(xPozisyonu < 125){
    /*Serial.print("Sol: ");
    Serial.println(xPozisyonu);*/
    esp.println("AT+CIPSEND=1");
    delay(2000);
    esp.println("2");
  }
  
  else if(yPozisyonu > 900){
    /*Serial.print("Yukarı: ");
    Serial.println(yPozisyonu);*/
    esp.println("AT+CIPSEND=1");
    delay(2000);
    esp.println("3");
  }
  
  else if(yPozisyonu < 125){
    /*Serial.print("Asagi: ");
    Serial.println(yPozisyonu);*/
    esp.println("AT+CIPSEND=1");
    delay(2000);
    esp.println("4");
  }
  
  else if(butonDurum == 0){
    //Serial.println("tick");
    esp.println("AT+CIPSEND=1");
    delay(2000);
    esp.println("5");
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
