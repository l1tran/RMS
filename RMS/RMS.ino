//#define BLYNK_PRINT Serial

#include <Adafruit_Fingerprint.h>
#include <BlynkSimpleSerialBLE.h>
#include <SoftwareSerial.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
SoftwareSerial fprint(10, 11); // RX, TX 
LiquidCrystal_I2C lcd(0x27,20,4); //LED library
Servo servo;

//Importing class for fingerprint scanner
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&fprint);

// Bluetooth app authorization
char auth[] = "ruHH9OhNgbNQDEnH_NI4US2wPtfc-wXK";

/* RGB PINS */
int bluePin = 7;
int greenPin = 6;
int redPin = 5;



/** 
 *  Unlocks door, changes lighting, and prints message for entering 
 *  based on user through bluetooth app buttons.
**/
BLYNK_WRITE(V3)
{
  servo.write(param.asInt());

  
  if(param.asInt() == 45){
    setColor(0,0,255);
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("Heeeeere's");
    lcd.setCursor(5,1);
    lcd.print("LAM");
    delay(1000);
  }
  else if(param.asInt() == 46){
    setColor(0,255,0);
    lcd.clear();
    lcd.setCursor(6,0);
    lcd.print("Hola");
    lcd.setCursor(4,1);
    lcd.print("Papi");
    delay(1000);
  }

  else if(param.asInt() == 47){
    setColor(128,0,128);
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("Welcome Home");
    lcd.setCursor(4,1);
    lcd.print("Daddy");
    delay(1000);
  }
  
  if(param.asInt() == 0){
    setColor(255,0,0);
    lcd.clear();
    lcd.setCursor(6,0);
    lcd.print("Door");
    lcd.setCursor(5,1);
    lcd.print("LOCKED");
    delay(1000);
  }
  
}

void setup()
{
  // Debug console
  Serial.begin(9600);
  Serial1.begin(9600);
  finger.begin(57600);
  Blynk.begin(Serial1, auth);

  servo.attach(9);

  pinMode(redPin,OUTPUT);
  pinMode(greenPin,OUTPUT);
  pinMode(bluePin,OUTPUT);

  finger.verifyPassword();
}


/** 
 *  Continuously checks fingerprint scanner for household members. If the user
 *  is a valid home member then the door will unlock, change lighting, and print
 *  a message. The door will lock after 5 seconds.
**/
void loop()
{
  Blynk.run();
  
    lcd.init();
    lcd.backlight();
    lcd.setCursor(6,0);
    lcd.print("Door");
    
    int member = getFingerprintIDez(); // Reading fingerprint scanner
    delay(50); 
    
    // Checks if user is a member
    if(finger.fingerID == member){
        if(member == 1 || member == 2 || member == 3){
            setColor(0,0,255);
            ServoknobCCW();
            lcd.clear();
            lcd.setCursor(2,0);
            lcd.print("Heeeeere's");        
            lcd.setCursor(4,1);
            lcd.print("LAM");
            delay(5000);
        }
        else if(member == 4 || member == 5){
            setColor(0,255,0);
            ServoknobCCW();
            lcd.clear();
            lcd.setCursor(6,0);
            lcd.print("Welcome Home");        
            lcd.setCursor(4,1);
            lcd.print("Papi");
            delay(5000);
        }
        else if(member == 6){
            setColor(128,0,128);
            ServoknobCCW();
            lcd.clear();
            lcd.setCursor(1,0);
            lcd.print("What's Cracka'lackin");        
            lcd.setCursor(4,1);
            lcd.print("Brian");
            delay(5000);
        }
                  
        ServoknobCW();
        lcd.clear();
        lcd.setCursor(6,0);
        lcd.print("Door");
        lcd.setCursor(5,1);
        lcd.print("LOCKED");
        setColor(255,0,0);
    }
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;


  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID; 
}

/** Sets color of LED **/
void setColor(int red, int green, int blue){
  analogWrite(redPin,red);
  analogWrite(greenPin,green);
  analogWrite(bluePin,blue);
}

/** Changes the orientation of Servo CCW **/
void ServoknobCCW(){
  servo.write(45);
  delay(5);
}

/** Changes the orientation of Servo CW **/
void ServoknobCW(){
  servo.write(0);
  delay(5);
}
