/*****************************************************************************************************
**          Project overview: Code broken into two systems: monitoring(L), and reactive(R).         **
**   Each system is defined as its own function to allow for a modular design in the final version. **
**                                                                                                  **
**                                    LAST MODIFIED: Mar. 31,2018                                   **
*****************************************************************************************************/

const int chipSelect = 4;
#include <SPI.h> //This is here for convience sake.
#include <SD.h> //Required to write or do anything with SD.
#include <LiquidCrystal.h> //LCD library
#include <DHT.h> //DHT library (temp and humidity)

LiquidCrystal lcd(13,12,4,5,6,7); //LCD Setup (physical pins)

#define DHTPIN 2 //Where the DHT pin is declared (digital)
#define DHTTYPE DHT11 //Telling the arduino we're using DHT11

DHT dht(DHTPIN, DHTTYPE); //insitializing DHT? 3/31

/*
Light
*/
int photo; // Value for photoresistor
byte PhotoC[8] = {
        0b00000,
        0b10101,
        0b01110,
        0b11111,
        0b01110,
        0b10101,
        0b00000,
        0b00000
}; // Custom character for sun

/*
Temperature
*/
float temp; // Temperature stored as float
byte TempC[8] = {
        0b00100,
        0b01010,
        0b01010,
        0b01010,
        0b01010,
        0b10001,
        0b10001,
        0b01110
}; // Custom character for thermometer

/*
Moisture
*/
int moist; // moisture sensor val
byte MoistC[8] = {
        0b00000,
        0b00100,
        0b00100,
        0b01110,
        0b01010,
        0b11101,
        0b11111,
        0b01110
}; // Custom character for soil moisture

/*
Humidity
*/
float humid; //Humidity sensor val
byte HumidC[8] = {
        0b00000,
        0b00000,
        0b01000,
        0b00000,
        0b00111,
        0b11000,
        0b00011,
        0b11100
}; // Custom character for humidity, symbol needs to be changed

int x; // THE "X"s NEED TO BE CHANGED IN THE REACTIVE LOOPS!

void setup()
{
Serial.begin(9600); //Runs the program at 9600 bauds

lcd.createChar(1, TempC); //assigns value to custom characters, used in the LCD library when adressing custom chars
lcd.createChar(2, PhotoC);
lcd.createChar(3, MoistC);
lcd.createChar(4, HumidC);
lcd.begin(16,2); // Defines LCD size

while (!Serial) //while serial does not exist
{
  lcd.setCursor(1,0); //untested code 3/31
  lcd.print("Serial monitor not running"); //print error to lcd if serial doesn't work
}

Serial.print("Testing"); //Says Testing in Serial monitor
if ( !SD.begin(chipSelect))
 { //If the chip cannot begin, then it prints Can not do.
 Serial.println("Can not do, homie"); //Error message
 lcd.setCursor(1,0);
 lcd.print("Unable to find");  //write error to LCD
 lcd.setCursor(2,0);
 lcd.print("SD card.");
 delay(3000); //wait three seconds
 lcd.clear(); //clears LCD screen before taking measurements.
 return;
 }

Serial.println("card good to go"); //Prints its good to go.
} //ends setup function


void loop() //run all of the individual functions, allows for a modular design of final project
{
PhotoL();
TempL();
MoistureL();
HumidL();
LCDL();
PhotoR();
TempR();
MoistureR();
HumidR();
} //ends loop funtion


void PhotoL()
{
String photoString = "";
int photoPin = 1;  //For now we are using pin 1 for the photoreceptor. Feel free to change
photo = analogRead(photoPin);  //Reads the sensor
photoString += String(photo);  // Adds to the string

File photoFile = SD.open("lightlog.txt", FILE_WRITE);  //Opens(or creates) a file named datalog.txt

if (photoFile)
{
  photoFile.print(photoString); //Adds the analog value to SD Card
  photoFile.println(","); //Adds a comma.
  photoFile.close(); // Closes the File
  Serial.println(photoString); //Prints value in serial
}

else
{
  Serial.println("Not working fam"); //change this
}
} //ends PhotoL function

void TempL()
{
String tempString = ""; //placeholder string for temperature to be inserted into 3/31
int tempPin = 2; //physical pin on arduino
temp = analogRead(tempPin);
temp = temp * .48828125; //conversion to degrees F
tempString += String(temp); //converting the temp val into a string

File tempFile = SD.open("templog.txt", FILE_WRITE); //Opens(or creates) a file named datalog.txt

if (tempFile)
{ //if the file can be found
  tempFile.print(tempString); //Adds the analog value to SD Card
  tempFile.println(","); //Adds a comma.
  tempFile.close(); // Closes the File
  Serial.println(tempString); //Prints value in serial
}

else
{ //should have this create file and write to it
  Serial.println("Not working fam"); //If it cant open or make dataFile, will display this.
}
} //end tempL function

void MoistureL()
{
String moistString = ""; //String stuff? Dont totally get this *same as TempL 3/31
int moistPin = 3;  //For now we are using pin 1 for the photoreceptor. Feel free to change
moist = analogRead(moistPin);  //Reads the sensor
moist = map(moist, 400, 1023, 100, 0); // Maps analog inputs to meaningful output (percentage 1-100%) starting at 400 allows for variance
moistString += String(moist);  // Adds to the string

File moistFile = SD.open("moistlog.txt", FILE_WRITE);  //Opens(or creates) a file named datalog.txt

if (moistFile)
{
  moistFile.print(moistString); //Adds the analog value to SD Card
  moistFile.println(","); //Adds a comma.
  moistFile.close(); // Closes the File
  Serial.println(moistString); //Prints value in serial monitor11
}

else
{
  Serial.println("Not working fam"); //change this
}
} //end MoistureL function

void HumidL()
{
String humidString = "";

// int humidPin = 4;  //For now we are using pin 1 for the photoreceptor. Feel free to change
//humid = analogRead(humidPin);  //Reads the sensor

humid = dht.readHumidity(); //Humidity read using DHT library
humidString += String(humid);  // Adds to the string

File humidFile = SD.open("humidlog.txt", FILE_WRITE);  //Opens(or creates) a file named datalog.txt

if (humidFile)
{
  humidFile.print(humidString); //Adds the analog value to SD Card
  humidFile.println(","); //Adds a comma.
  humidFile.close(); // Closes the File
  Serial.println(humidString); //Prints value in serial monitor
}

else
{
Serial.println("Not working fam");
}
} //ends HumidL function

void LCDL() //prints all read values to LCD
{
if(1 == 1) // Eventually changed for toggling "On" "Off"
{
  lcd.setCursor(1,0); // (in one, row zero) Top left
  lcd.write(1);  // Calls custom char ""
  lcd.print(":");
  lcd.print(temp); // CORRECT VARIABLE NEEDED

  lcd.setCursor(1,1);
  lcd.write(2);
  lcd.print(":");
  lcd.print(photo); // CORRECT VAR

  lcd.setCursor(11,0);
  lcd.write(3);
  lcd.print(":");
  lcd.print(moist); // CORRECT VAR
  lcd.print("%");

  lcd.setCursor(11,1);
  lcd.write(4);
  lcd.print(":");
  lcd.print(humid); // CORRECT VAR
  lcd.print("%");
}

else
{
}
} //ends LCDL function

void PhotoR()
/* Have to declare "photo" as global variable to use in this function.
come up with some function to relate the current light level to teh desired light level. function will probably look like photo+write == constant
*/
{
if (photo > x)
{
  //Make dimmer
}

if (photo < x)
{
  analogWrite(photo, )
  //Make brighter
}

if (photo == x)
{
  //Do nothing
}

else
{
}
} //ends PhotoR

void TempR()
/*
  Declare temp as global variable, write function lie temp+write= constant, where C is dependant on the time of day we want to simulate (maybe dependant on light level?)
*/
{
if (temp > x)
{
  //Lower temp
}

if (temp < x)
{
  // Raise Temp
}
  //dont need following checks, will only add time to program runtime
if (temp == x)
{
  //Do nothing
}

else
{
}
} //ends TempR

void MoistureR()
 /* define moist as global, write function for ideal Moisture
 */
 {
if (moist > x)
{
   //Command to turn off some misting
}

if ( moist < x)
{
   //Command to start Misting
}

   //dont need these statements
if (moist == x)
{
    //command for doing nothing
}

else
{
    //do nothing
}
} //ends MoistureR


void HumidR()
//humid => global, function for ideal humidity.
{
if (humid > x)
{
    //Command
}

if (humid < x)
{
    //Command
}

if (humid == x)
{
    //Command
}

else
{
    //Command
}

} //ends HumidR
