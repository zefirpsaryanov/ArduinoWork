
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <Wire.h>

#define RxD 10
#define TxD 11

LiquidCrystal_I2C lcd(0x3F, 16, 2);
SoftwareSerial SoftSerial(RxD,TxD);

byte inData;
char inChar;
String BuildINString="";
String DisplayString="";
String WorkingString="";
long DisplayValue;
long A;

void setup() {
  lcd.init();                    
  lcd.backlight();
  lcd.home();
  lcd.setCursor(0, 0);
  lcd.print("Honda Civic VII");
  delay(3000);
  lcd.clear();

//********************************************************************
  //Check if ELM327 responded
  Retry:
  lcd.setCursor(0, 0);
  lcd.print("                    ");
  lcd.setCursor(0, 1);
  lcd.print("Connecting......    ");
  
  //Set up Serial communication at 9600 baud
  SoftSerial.begin(9600);   //initialize Serial  

  SoftSerial.println("ATZ");
  lcd.setCursor(0, 0);
  lcd.print("OBD2");
  delay(2000);
  ReadData();
                                             // If used substring(1,4)=="ATZ" needed a space before ATZ in Serial Monitor and it did not work
  if (BuildINString.substring(1,3)=="TZ")    // MIATA RESPONSE TO ATZ IS ATZ[[[ELM327 V1.5  OR AT LEAST THAT IS WHAT ARDUINO HAS IN THE BUFFER
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Welcome");
      lcd.setCursor(9, 0);
      //lcd.print(BuildINString);   //Echo response to screen "Welcome  ELM327"
      lcd.setCursor(0, 1);
      lcd.print("Connection OK         ");
      delay(1500);
      lcd.clear();
    }
    else
    {
      lcd.setCursor(0, 0);
      lcd.print("Error             ");
      lcd.setCursor(0, 1);
      lcd.print("No Connection!         ");
      delay(1500);
      lcd.clear();
      goto Retry;
    }

  //*****************************************************************  
  //Added this from the test Code
  SoftSerial.println("0100");          // Works with This  only
  lcd.setCursor(0, 0);
  lcd.print("Initialzing.....");          //Initialize BUS  //lcd.print("0100 Sent"); 
  delay(4000);
  ReadData();
  lcd.setCursor(0, 0);            //Added 12-10-2016
  lcd.print("Initialized.....");  //Added 12-10-2016
  delay(1000);
  lcd.clear();
}

void loop() {
  // Added to display Coolant Temp
  lcd.setCursor(0, 0);
  lcd.print("Coolant Temp    "); 
  //resets the received string to NULL  Without it it repeated last string.
  BuildINString = "";  
  SoftSerial.println("0105");  // Send Coolant PID request 0105
  //SoftSerial.flush();         //Not sure if it's needed*********************************************
  delay(1000);
  // Receive complete string from the serial buffer
  ReadData();  //replaced below code
  // Parse the received string, retrieve only the hex value for temperature  Example: 32 is the 11 and 12th character
  // 0105[41 05 32   Correct value reading back  10 degrees. (11,13 or even 11,14 works)
  WorkingString = BuildINString.substring(11,13);   
  A = strtol(WorkingString.c_str(),NULL,16);  //convert hex to decimnal
  DisplayValue = A;
  DisplayString = String(DisplayValue - 40) + " C            ";  // Subtract 40 from decimal to get the right temperature
  lcd.setCursor(0, 1);
  lcd.print(DisplayString); 
  delay(500);
  //Check if if over 100 C ******************************************************
  int B;
  B = DisplayString.toInt();  //Convert String to Integer  .toInt()
  if (B >= 100){     //Check if if over 100 C
    // ------- Quick 3 blinks of backlight  -------------
    for(int i = 0; i< 3; i++)
    {
      lcd.display();   //For I2C use lcd.backlight
      delay(250);
      lcd.noDisplay(); //For I2C use lcd.noBacklight
      delay(250);
    }
    lcd.display(); // finish with backlight on   //For I2C use lcd.backlight
  }
}

//Read Data Function ***********************************************************
void ReadData()
{
  BuildINString="";  
  while(SoftSerial.available() > 0)
  {
    inData=0;
    inChar=0;
    inData = SoftSerial.read();
    inChar=char(inData);
    BuildINString = BuildINString + inChar;
  }
}



/*
at+reset

at+cmode=0

at+inqm=0,5,9

at+init

at+inq (now the led on the hc-05 starts to blink rapidly also turn on the car and make sure the elm327 is powered before you do this step)
You should get a response with the address of the elm327 device.

Now type 
at+rname?0019,6d,3292EF 
to verify if the address you've typed is correct (its easy to screw up the placement of the comma "," also for this step alone, use the entire address i.e 0019,6d,3292EF)

once you get the name its time to connect, note that the name thing is not relevant

at+pair=196D,32,92EF,9 (you'll have to remove the initial zeroes in this step)
at+bind=196D,32,92EF 
at+cmode=1
at+link=196D,32,92EF 

Turn off the arduino board, disconnect pin34 from any kind of input. You wont be needing it until you try pairing the hc-05 with another bt device.

Turn on the arduino board and open the serial monitor
Now you should notice the led light on the hc-05 blink in a different pattern, it indicates that it has connected to the elm327
And now you can type the PIDs in the serial monitor(like 010D) and you should get a response.

Now if you've gotten it to work this far then let me know and I'll tell you what to do with the display.

Cheers.
*/
