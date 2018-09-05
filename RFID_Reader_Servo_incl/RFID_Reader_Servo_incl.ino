#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <Keypad.h>
 const byte ROWS = 4; //four rows
  const byte COLS = 3; //three columns
  char keys[ROWS][COLS] =
 {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
  }; 
byte rowPins[ROWS] = {
  19, 18, 17, 16}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {
  15, 14, 5}; // connect to the column pinouts of the keypad 
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   
Servo lockerservo;
char* lockercode = "230500";
char* emergencycode = "151723";
int position = 0;

 
void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  lockerservo.attach(4);
  position = 0;
  Serial.println("Welcome to Keypad Unlock");

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
    lockerservo.write(180);
    delay(20000);
    lockerservo.write(90);
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
  if (content.substring(1) == "40 11 9F A5" || content.substring(1) == "75 41 25 D9")
  {
    Serial.println("Access granted");
    lockerservo.write(180);
    delay(20000);
    lockerservo.write(90);
    delay(3000);
  }
 
 else   {
    Serial.println("Access denied");
    delay(3000);
  }
}
}


