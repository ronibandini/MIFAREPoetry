
// Mifare Poetry 
// Roni Bandini, August 2024, @ronibandini
// MIT License

#include "WiFiS3.h"
#include <MFRC522.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include "arduino_secrets.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128  
#define SCREEN_HEIGHT 32  
#define OLED_RESET     -1  
#define SCREEN_ADDRESS 0x3C  
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define RST_PIN         9            
#define SS_PIN          10           
MFRC522 mfrc522(SS_PIN, RST_PIN); 

char ssid[] = SECRET_SSID;        
char pass[] = SECRET_PASS;    
int keyIndex = 0;            

int status = WL_IDLE_STATUS;

WiFiClient client;

// Loading scripts server
char server[] = "YOURSERVERHERE.COM";

String myResponse="";
String writeLine1="";
String writeLine2="";
String writeLine3="";
String myLine="";
byte byteArray[16];
JsonDocument doc;

// Update delay
const unsigned long postingInterval = 5L * 1000L; 

unsigned long lastConnectionTime = 0;         

void readCard(){
       
       mfrc522.PCD_Init();
       delay(3);

      // Keys are set to FFFFFFFFFFFFh for blank cards
        MFRC522::MIFARE_Key key;
        for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

        byte block;
        byte len;
        MFRC522::StatusCode status;

        if ( ! mfrc522.PICC_IsNewCardPresent()) {
          Serial.println(F("No MIFARE card detected...")); 
          displayCover("No card detected..."); 
          return;
        }


        if ( ! mfrc522.PICC_ReadCardSerial()) {
          Serial.println(F("MIFARE card cannot be read...")); 
          displayCover("Cannot read card..."); 
          return;
        }

        displayCover("Reading card..."); 

        Serial.println(F("-------------------------"));

        byte buffer1[18];

        //---------------------------------------- Read counter

        block = 8;
        len = 18; 

        status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, block, &key, &(mfrc522.uid)); 

        if (status != MFRC522::STATUS_OK) {
          Serial.print(F("Authentication failed: "));
          Serial.println(mfrc522.GetStatusCodeName(status));
          return;
        }

        status = mfrc522.MIFARE_Read(block, buffer1, &len);
        if (status != MFRC522::STATUS_OK) {
          Serial.print(F("Reading failed: "));
          Serial.println(mfrc522.GetStatusCodeName(status));
          return;
        }

        String myCounterString = String((char *)buffer1);
        int remainingReadings=myCounterString.toInt();

        remainingReadings=remainingReadings-1;
        Serial.println("Remaining readings ");
        Serial.println(remainingReadings);    

        displayReadings(remainingReadings);
        delay(2000);

        if (remainingReadings<=0){

          Serial.println("Deleting MIFARE poem...");

          displayCover("Deleting poem..."); 

          block = 4;

          status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, block, &key, &(mfrc522.uid));
          if (status != MFRC522::STATUS_OK) {
            Serial.print(F("PCD_Authenticate() failed: "));
            Serial.println(mfrc522.GetStatusCodeName(status));
            return;
          }


          for (byte i = 0; i < 16; i++) byteArray[i] = ' ';     // pad with spaces 
                       
          status = mfrc522.MIFARE_Write(block, byteArray, 16);
          if (status != MFRC522::STATUS_OK) {
           Serial.print(F("MIFARE_Write() failed: "));
            Serial.println(mfrc522.GetStatusCodeName(status));
            return;
          }    

          block = 5;
          status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, block, &key, &(mfrc522.uid));
          if (status != MFRC522::STATUS_OK) {
            Serial.print(F("Authentication failed: "));
            Serial.println(mfrc522.GetStatusCodeName(status));
            return;
          }
        
        // Write line
          status = mfrc522.MIFARE_Write(block, byteArray, 16);
          if (status != MFRC522::STATUS_OK) {
            Serial.print(F("MIFARE_Write() failed: "));
            Serial.println(mfrc522.GetStatusCodeName(status));
            return;
          }

       
        // Line 3
      
        block = 6;
        status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, block, &key, &(mfrc522.uid));
        if (status != MFRC522::STATUS_OK) {
          Serial.print(F("Authentication failed: "));
          Serial.println(mfrc522.GetStatusCodeName(status));
          return;
        }

        status = mfrc522.MIFARE_Write(block, byteArray, 16);
        if (status != MFRC522::STATUS_OK) {
          Serial.print(F("MIFARE_Write() failed: "));
          Serial.println(mfrc522.GetStatusCodeName(status));
          return;
        }
        else {
          Serial.println(F("Poetry deleted..."));
          displayCover("Poem deleted...");           
        }

        }
        else{      

                // update remaining readings

                block = 8;

                myCounterString=String(remainingReadings);

                status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, block, &key, &(mfrc522.uid));
                if (status != MFRC522::STATUS_OK) {
                  Serial.print(F("Auth block 1 failed: "));
                  Serial.println(mfrc522.GetStatusCodeName(status));
                  return;
                }
            
                for (byte i = len; i < 16; i++) byteArray[i] = ' ';     // pad with spaces 
                myCounterString.getBytes(byteArray, 16);  
                      
                status = mfrc522.MIFARE_Write(block, byteArray, 16);
                if (status != MFRC522::STATUS_OK) {
                  Serial.print(F("MIFARE_Write() failed: "));
                  Serial.println(mfrc522.GetStatusCodeName(status));
                  return;
                }
                else {
                  Serial.println(F("Counter updated "));                  
                  }
        } // updating readings

        Serial.println("");


        //---------------------------------------- line 1

      if (remainingReadings>0){
        block = 4;
        len = 18; 

        status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, block, &key, &(mfrc522.uid)); 
        if (status != MFRC522::STATUS_OK) {
          Serial.print(F("Authentication failed: "));
          Serial.println(mfrc522.GetStatusCodeName(status));
          return;
        }

        status = mfrc522.MIFARE_Read(block, buffer1, &len);
        if (status != MFRC522::STATUS_OK) {
          Serial.print(F("Reading failed: "));
          Serial.println(mfrc522.GetStatusCodeName(status));
          return;
        }

        String myLine1 = String((char *)buffer1);

        Serial.println(myLine1);

        Serial.println("");


        //---------------------------------------- line 2

        byte buffer2[18];
        block = 5;

        status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, block, &key, &(mfrc522.uid)); 
        if (status != MFRC522::STATUS_OK) {
          Serial.print(F("Authentication failed: "));
          Serial.println(mfrc522.GetStatusCodeName(status));
          return;
        }

        status = mfrc522.MIFARE_Read(block, buffer2, &len);
        if (status != MFRC522::STATUS_OK) {
          Serial.print(F("Reading failed: "));
          Serial.println(mfrc522.GetStatusCodeName(status));
          return;
        }

      String myLine2 = String((char *)buffer2);

      Serial.println(myLine2);

      Serial.println("");
      
      //---------------------------------------- line 3

        byte buffer3[18];
        block = 6;

        status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, block, &key, &(mfrc522.uid)); 
        if (status != MFRC522::STATUS_OK) {
          Serial.print(F("Authentication failed: "));
          Serial.println(mfrc522.GetStatusCodeName(status));
          return;
        }

        status = mfrc522.MIFARE_Read(block, buffer3, &len);
        if (status != MFRC522::STATUS_OK) {
          Serial.print(F("Reading failed: "));
          Serial.println(mfrc522.GetStatusCodeName(status));
          return;
        }

        String myLine3 = String((char *)buffer3);

        Serial.println(myLine);

        Serial.println(F("\n-------------------------\n"));

        displayPoem(myLine1, myLine2, myLine3);   

        mfrc522.PICC_HaltA();
        mfrc522.PCD_StopCrypto1();

      } // remaining readings ok

}

void writeCard(){
        
        mfrc522.PCD_Init();
        delay(3);        

        // FFFFFFFFFFFFh for factory new cards
        MFRC522::MIFARE_Key key;
        for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;
        
        if ( ! mfrc522.PICC_IsNewCardPresent()) {
          Serial.println(F("No new card present...")); 
          displayCover("No new card present..."); 
          return;
        }
        
        // Select one of the cards
        if ( ! mfrc522.PICC_ReadCardSerial()) {
          Serial.println(F("Returning, card cannot be read..."));  
          displayCover("Card cannot be read..."); 
          return;
        }

        Serial.println(F("Writing card..."));    
        displayCover("Writing card..."); 

        byte block;
        MFRC522::StatusCode status;
        byte len;

        // Write counter     

        block = 8;

        status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, block, &key, &(mfrc522.uid));
        if (status != MFRC522::STATUS_OK) {
          Serial.print(F("Auth counter failed: "));
          Serial.println(mfrc522.GetStatusCodeName(status));
          return;
        }

       String writeCounterLine="3";

        for (byte i = len; i < 16; i++) byteArray[i] = ' ';     // pad with spaces 
        writeCounterLine.getBytes(byteArray, 16);  
               
        status = mfrc522.MIFARE_Write(block, byteArray, 16);
        if (status != MFRC522::STATUS_OK) {
          Serial.print(F("MIFARE_Write() failed: "));
          Serial.println(mfrc522.GetStatusCodeName(status));
          return;
        }
        else {
          Serial.println(F("Counter saved..."));
          displayCover("Counter saved..."); 
        }
        
        // Line 1        

        block = 4;

        status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, block, &key, &(mfrc522.uid));
        if (status != MFRC522::STATUS_OK) {
          Serial.print(F("PCD_Authenticate() failed: "));
          Serial.println(mfrc522.GetStatusCodeName(status));
          return;
        }

        // Write line    

        for (byte i = len; i < 16; i++) byteArray[i] = ' ';     // pad with spaces 
        writeLine1.getBytes(byteArray, 16);  
               
        status = mfrc522.MIFARE_Write(block, byteArray, 16);
        if (status != MFRC522::STATUS_OK) {
          Serial.print(F("MIFARE_Write() failed: "));
          Serial.println(mfrc522.GetStatusCodeName(status));
          return;
        }

        block = 5;
        status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, block, &key, &(mfrc522.uid));
        if (status != MFRC522::STATUS_OK) {
          Serial.print(F("Authentication failed: "));
          Serial.println(mfrc522.GetStatusCodeName(status));
          return;
        }

        for (byte i = len; i < 16; i++) byteArray[i] = ' ';     // pad with spaces 
        writeLine2.getBytes(byteArray, 16); 

        // Write line
        status = mfrc522.MIFARE_Write(block, byteArray, 16);
        if (status != MFRC522::STATUS_OK) {
          Serial.print(F("MIFARE_Write() failed: "));
          Serial.println(mfrc522.GetStatusCodeName(status));
          return;
        }
      
        block = 6;
        status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, block, &key, &(mfrc522.uid));
        if (status != MFRC522::STATUS_OK) {
          Serial.print(F("Authentication failed: "));
          Serial.println(mfrc522.GetStatusCodeName(status));
          return;
        }

        for (byte i = len; i < 16; i++) byteArray[i] = ' ';     // pad with spaces 
        writeLine3.getBytes(byteArray, 16); 

        // Write line
        status = mfrc522.MIFARE_Write(block, byteArray, 16);
        if (status != MFRC522::STATUS_OK) {
          Serial.print(F("MIFARE_Write() failed: "));
          Serial.println(mfrc522.GetStatusCodeName(status));
          return;
        }
        else {
        Serial.println(F("Poem saved..."));
        displayCover("Saved, remove card..."); 
        delay(4000);
        }

}

void displayReadings( int readings){
  display.clearDisplay();
  display.setTextSize(1);              
  display.setTextColor(SSD1306_WHITE);        
  display.setCursor(8,0);              
  display.println(F("Remaining readings"));          
  display.setCursor(45,15);   
  display.setTextSize(2);              
  display.println(String(readings)+"/3");
  display.display();

}

void displayCover(String myMessage) {
  display.clearDisplay();
  display.setTextSize(1);              
  display.setTextColor(SSD1306_WHITE);        
  display.setCursor(0,0);              
  display.println(F("MIFARE Poetry"));          
  display.setCursor(0,10);   
  display.println(F("13.56 MHZ - NFC - 1.0"));
  display.setCursor(0,20);   
  display.println((myMessage));
  display.display();
}

void displayPoem(String line1, String line2, String line3) {
  display.clearDisplay();

  display.setTextSize(1); 
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println((line1));
  display.display();      
  delay(100);
  display.startscrollright(0x00, 0x0F);
  delay(5000);
  display.stopscroll();
  display.clearDisplay();
  display.setCursor(0, 10);
  display.println((line2));
  display.display();      
  display.startscrollright(0x00, 0x0F);
  delay(5000);
  display.stopscroll();
  display.clearDisplay();
  display.setCursor(0, 20);
  display.println((line3));
  display.display();      
  display.startscrollright(0x00, 0x0F);
  delay(4000);
  display.stopscroll();
  display.clearDisplay();

  display.setCursor(0, 0);
  display.println((line1));
  display.setCursor(0, 10);
  display.println((line2));
  display.setCursor(0, 20);
  display.println((line3));
  display.display();  
  delay(4000);

  displayCover("Remove the card");
  delay(4000);

}

void setup() {
  
  SPI.begin();                                                  
  mfrc522.PCD_Init();  
  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
  }


  display.display();
  display.clearDisplay();

  displayCover("Starting...");     


  while (!Serial) {
  }

  Serial.print("MIFARE Poetry started - Roni Bandini, August 2024");

  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Comm with WiFi module failed...");
    while (true);
  }

  displayCover("Connecting..."); 

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);

  }

}

void read_request() {
  uint32_t received_data_num = 0;

  while (client.available()) {
    char c = client.read();
    myResponse=myResponse+c;
    received_data_num++;
  }  

if (myResponse!=""){

  int startAt = myResponse.indexOf('{');
  String myJson=myResponse.substring(startAt);
  deserializeJson(doc, myJson);

  String mode = doc[String("mode")];
  String myLine1 = doc[String("line1")];
  String myLine2 = doc[String("line2")];
  String myLine3 = doc[String("line3")];

  if (mode=="w"){
    Serial.println("Write mode...");
    writeLine1=myLine1;
    writeLine2=myLine2;
    writeLine3=myLine3;
    Serial.println(myLine1);
    Serial.println(myLine2);
    Serial.println(myLine3);
    writeCard();
  }
  else{
    Serial.println("Read mode...");
    readCard();
  }

}

myResponse="";

delay(2000);

}

void loop() {

  read_request();
  
  if (millis() - lastConnectionTime > postingInterval) {
    httpRequest();
  }

}


void httpRequest() {

  client.stop();

  if (client.connect(server, 80)) {
    Serial.println("Connecting...");
    // json folder
    client.println("GET mifare/mifare.json HTTP/1.1");
    client.println("Host: YOURSERVERHERE.COM");
    client.println("User-Agent: ArduinoWiFi/1.1");
    client.println("Connection: close");
    client.println();
    lastConnectionTime = millis();
  } else {
    Serial.println("Connection failed");
  }
}


