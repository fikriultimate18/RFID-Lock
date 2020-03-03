/*
 * Initial Author: ryand1011 (https://github.com/ryand1011)
 *
 * Reads data written by a program such as "rfid_write_personal_data.ino"
 *
 * See: https://github.com/miguelbalboa/rfid/tree/master/examples/rfid_write_personal_data
 *
 * Uses MIFARE RFID card using RFID-RC522 reader
 * Uses MFRC522 - Library
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
*/

#include <SPI.h>
#include <MFRC522.h>

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define RST_PIN         9           // Configurable, see typical pin layout above
#define SS_PIN          10          // Configurable, see typical pin layout above

#define DOORLCK_PIN     3       
#define BUZZER_PIN      2

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);                                           // Initialize serial communications with the PC
  SPI.begin();                                                  // Init SPI bus
  mfrc522.PCD_Init();                                              // Init MFRC522 card
  Serial.println(F("Read personal data on a MIFARE PICC:"));    //shows in serial that it is ready to read
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(DOORLCK_PIN, OUTPUT);

  digitalWrite(3, HIGH);

  lcd.begin();
  lcd.backlight();

  printLCD("HMTE FMIPA Unpad", "Waiting for card");
  
  beepOn();
}

MFRC522::MIFARE_Key key;

char nameDB[16][20] = {"ADHI VIARI N    #",
                    "GHUFRON ZAHID M #",
                    "IBNU AMUNA M    #",
                    "ISMAIL AKROMUL J#",
                    "KAMARUDIN ANHAR #",
                    "DINDA FANIASIH S#",
                    "SIGIT TRIWIBOWO #",
                    "M AZIS KARIM    #",
                    "DAVID ADIKUSUMA #",
                    "IVMA SALSABILA  #",
                    "DAVIN D B       #",
                    "LABIB HANAFI    #",
                    "FIKRI RIDA P    #",
                    "MICHAEL A       #",
                    "RAVEDYA AUFA A  #"};
                    
void loop() {

  // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent()) return;
  if (!mfrc522.PICC_ReadCardSerial()) return;

  Serial.println("\n**Card Detected:**");

  mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid)); //dump some details about the card

  String nameCard = readCard(1);
  String titleCard = readCard(2);
  
  Serial.print("Name: ");
  Serial.println(nameCard);

  Serial.print("Title: ");  Serial.println(titleCard);
  
  if(searchName(nameCard)){ 
    beepOk(); 

    printLCD(nameCard, titleCard);
  
    digitalWrite(DOORLCK_PIN, LOW);
    delay(5000);
    digitalWrite(DOORLCK_PIN, HIGH);
  }else{
    beepFalse();
    lcd.clear();
    printLCD("Card isn't","recognized");
    delay(2000);
  }

  delay(1000);

  printLCD("HMTE FMIPA Unpad", "Waiting for card");

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}

String readCard(byte block){
  MFRC522::StatusCode status;

  byte len=18;
  byte buffer2[18];
  
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid)); //line 834
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

  String cardData;

  for (uint8_t i = 0; i < 16; i++) {
    if(buffer2[i] >= 32 && buffer2[i] <= 126){
      char c = buffer2[i];
      cardData += c;
    }
  }
  return cardData;
}

bool searchName(String nameToSearch){
  
  for(int i=0; i<15; i++){
    String nameDB_str = "";
    for(int j=0; j<16; j++){
      char c = nameDB[i][j];
      if(c == '#') break;
      nameDB_str += c;
    }
    if(nameDB_str == nameToSearch){
      Serial.println("FOUND!!!");
      return true;
      break;
    } 
  }
  return false;
}

void printLCD(String line1, String line2){
  lcd.clear();
  lcd.home();
  lcd.print(line1);
  lcd.setCursor(0,1);
  lcd.print(line2); 
}

void beepOk(){
  for(int i=0; i<2; i++){
    digitalWrite(BUZZER_PIN, HIGH);
    delay(100);
    digitalWrite(BUZZER_PIN, LOW);
    delay(100);
  }
}

void beepFalse(){
  for(int i=0; i<2; i++){
    digitalWrite(BUZZER_PIN, HIGH);
    delay(200);
    digitalWrite(BUZZER_PIN, LOW);
    delay(200);
  }
}

void beepOn(){
  digitalWrite(BUZZER_PIN, HIGH);
  delay(300);
  digitalWrite(BUZZER_PIN, LOW);
  delay(200);
  for(int i=0; i<2; i++){
    digitalWrite(BUZZER_PIN, HIGH);
    delay(100);
    digitalWrite(BUZZER_PIN, LOW);
    delay(100);
  }
}
