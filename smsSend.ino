// voltage read and push button sent sms


//GSM TX=7 , RX= 8 , push_button=09 , Analog PIN = A0




#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#define PUSH_BUTTON 9
#define SENSOR_PIN A0

// Configure software serial port
SoftwareSerial SIM900(7, 8); // TX,RX gsm

void setup() {
  pinMode(SENSOR_PIN, INPUT);
  pinMode(PUSH_BUTTON, INPUT_PULLUP);
  lcd.begin(16, 2);
  Serial.begin(9600);
  SIM900.begin(9600);

  // Give time to your GSM shield log on to network
  delay(10000);
  Serial.print("SIM900 ready...");
   lcd.print("SIM900 ready...");
  // AT command to set SIM900 to SMS mode
  SIM900.print("AT+CMGF=1\r"); 
  delay(100);
  // Set module to send SMS data to serial out upon receipt 
  SIM900.print("AT+CNMI=2,2,0,0,0\r");
  delay(100);
}


void sendSMS()
{
      int R = analogRead(A0);
      double V = 5.0*(R/1023.0);
      // REPLACE WITH YOUR OWN SMS MESSAGE CONTENT
      String dataMessage = (" Voltage: " + String(V) + "");
      Serial.println(dataMessage);
      lcd.print(dataMessage);
     

      //return;
      
      // REPLACE THE X's WITH THE RECIPIENT'S MOBILE NUMBER
      // USE INTERNATIONAL FORMAT CODE FOR MOBILE NUMBERS
      SIM900.println("AT + CMGS = \"+919641892607\" ");
      delay(1000);
      
      // Send the SMS text message
      SIM900.print(dataMessage);
      delay(1000);
      // End AT command with a ^Z, ASCII code 26
      SIM900.println((char)26); 
      
      // Give module time to send SMS
      delay(5000);
}

unsigned long lastSMSSentTime = 0;

void loop(){


  
  if (!digitalRead(PUSH_BUTTON) && millis() - lastSMSSentTime > 1000){
    lastSMSSentTime = millis();
    sendSMS();
  }
}
