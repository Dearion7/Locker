#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

//Keypad Pins
const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] =
{
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
}; 
byte rowPins[ROWS] = {
  17, 16, 15, 14}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {
  6, 5, 4, 3}; // connect to the column pinouts of the keypad 
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

//RFID Pins
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);

//Locker code
Servo lockerservo;
char* lockercode = "111111";
char* emergencycode = "123456";
int position = 0;

//Relay Pin
#define RELAY_PIN 7
 
void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  pinMode(RELAY_PIN, OUTPUT); // Initiate Relay
  lockerservo.attach(4);
  position = 0;
  Serial.println("Welcome to Keypad Unlock");
  lcd.begin(16, 2);    // initialize the LCD
  lcd.backlight();    // Turn on the blacklight and print a message
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("*** Welcome! ***");
}
void loop() 
{
  
 char keypressed = keypad.getKey();
  
  if(keypressed != NO_KEY){
    
    Serial.println(keypressed);
    
    if(keypressed == lockercode[position]){
    position ++;
    Serial.println("Please enter next key");
    
    
  }

  if(position == 6){
    Serial.println("The RFID can now be used");
    RFID();
    position =0;
    
  }

    if(keypressed == emergencycode[position]){
    position ++;
    Serial.println("Please enter next key");
    if(position == 6){
    digitalWrite(RELAY_PIN, HIGH);
    delay(20000);
    digitalWrite(RELAY_PIN, LOW);
    position =0;
    
  }
    
  }
    
  }
}


void RFID(){
    Serial.println("Present Card or Tag to Reader.");
{
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
     
  }
  Serial.println(" ");
  content.toUpperCase();
  if (content.substring(1) == "D6 64 75 63" /*|| content.substring(1) == "75 41 25 D9"*/)
  {
    Serial.println("Access granted");
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("*** Access ***");
    lcd.setCursor(4, 1);
    lcd.print("Granted!");
    digitalWrite(RELAY_PIN, HIGH);
    delay(5000);
    digitalWrite(RELAY_PIN, LOW);
    delay(500);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("*** Welcome! ***");
  }
 
 else   {
    Serial.println("Access denied");
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("*** Access ***");
    lcd.setCursor(5, 1);
    lcd.print("Denied");
    delay(3000);
    lcd.clear();
  }
  delay(3000);
}
}


