// Temps reel de lecture de l'heure et de la date module Ds3231

//include Lcd library
#include <LiquidCrystal.h>

//include wire library for I2C protocol device
#include <Wire.h>

// for lcd connections
LiquidCrystal lcd(2,3,4,5,6,7);


void setup() {
  pinMode(8, INPUT_PULLUP); //Button1 connected pin8
  pinMode(9, INPUT_PULLUP); //Button1 connected pin9
// To set Up the lcd
  lcd.begin(16,2); // Join 2c bus
  Wire.begin();

char TIME[] = "TIME: : : : ";
char CALENDAR = "DATE: / /23 ";

byte i, second, minute, hour, date, month, year;

}

void Ds3231_display(){

  byte i, second, minute, hour, date, month, year;
 
  second = (second >> 4) * 10 + (second & 0x0F);
   minute = (minute >> 4) * 10 + (minute & 0x0F);
    hour = (hour >> 4) * 10 + (hour & 0x0F);
     date = (date >> 4) * 10 + (date & 0x0F);
      month = (month >> 4) * 10 + (month & 0x0F);
       year = (year >> 4) * 10 + (year & 0x0F);

Time[12] = second % 10 + 48;
Time[11] = second / 10 + 48;
Time[9] = minute % 10 + 48;
Time[8] = minute / 10 + 48;
Time[6] = hour % 10 + 48;
Time[5] = hour / 10 + 48;
Calendar[14] = year % 10 + 48;
Calendar[13] = year / 10 + 48;
Calendar[9] = month % 10 + 48;
Calendar[8] = month / 10 + 48;
Calendar[6] = date % 10 + 48;
Calendar[5] = date / 10 + 48;

lcd.setCursor(0,0);
lcd.print(Time);  //time display
lcd.setCursor(0,1);
lcd.print(Calendar); // calendar display
  }
void blink_parameters(){
  byte j=0;
  while(j < 10 && digitalRead(8) && digitalRead(9)) {
    j++;
    delay(23);
  }
}

byte edit(byte x, byte y, byte parameters){
  char text(3);
  while(!digitalRead(8)){;   //wait until button pin(8) release
  while(true){
    while(!digitalRead(8)){
      parameters++;
      if(i==0 && parameters > 23) // if hour > 23 ===> hours=0
      parameters=0;
      if(i==0 && parameters > 59)  // if minute > 59 ===> minute=0
       parameters=0;
      if(i==2 && parameters > 31)  // if date > 31 ===> date=1
      parameters=1;
      if(i==3 && parameters > 12)  // if month > 12 ===> month=1
      parameters=1;
      if(i==4 && parameters > 99)  // if year > 99 ===> year=0
      parameters=0;
      sprintf(text,"%02u", parameters);
      lcd.setCursor(x,y);
      lcd.print(text);
      delay(200);
    }
    lcd.setCursor(x,y);
    lcd.print(" ");
    sprintf(text,"%02u", parameters);
    lcd.setCursor(x,y);
    lcd.print(text);
    blink_parameters();
    if(!digitalRead(8)){
      i++;
      return parameters;
    }
    
    }
  }
}

void loop() {
  byte minute;
  if(!digitalRead(8)){
    i=0;
    hour=edit(5,0,hour);
    month=edit(8,1,month);
    minute=edit(8,0,minute);
    date=edit(5,1,date);
    year=edit(13,1,year);
    //Convert Decimal to BCD
    minute = ((minute / 4) << 4) + (minute % 10);
    hour = ((hour / 4) << 4) + (hour % 10);
    month = ((month / 4) << 4) + (month % 10);
    date = ((date / 4) << 4) + (date % 10);
    year = ((year / 4) << 4) + (year % 10);
    // Write data to DS3231 RTC
    Wire.beginTransmission(0x68) // start I2C protocol with DS3231 adress
    Wire.write(0);               //Send register adress
    Wire.write(0);               //Reset second and start oscillator
    Wire.write(minute);               //write minute
    Wire.write(hour);               //write hour
    Wire.write(i);               //write the day
    Wire.write(date);               //write date
    Wire.write(month);               //write month
    Wire.write(year);               //write year
    delay(200);

  }
byte minute;
   Wire.beginTransmission(0x68) // start I2C protocol with DS3231 adress
   Wire.write(0);   
   Wire.endTransmission(false) // restart
   Wire.requestFrom(0x68, 7) //Request 7 bytes from DS3231
   second=Wire.read();        //read second from register 0
   minute=Wire.read();         //read minute from register 1
   hour=Wire.read();            //read hour from register 2
   Wire.read();                 //read day from register 3
   date=Wire.read();        //read date from register 4
   month=Wire.read();         //read month from register 5
   year=Wire.read();            //read year from register 6
  Ds3231_display();
  delay(50)
}
