#include <LiquidCrystal_I2C.h>
#include <Wire.h> 
#include <HX711.h>

#define DOUT 4
#define CLK  5

LiquidCrystal_I2C lcd(0x27,24,4);     //alebo 0x27 --> I2C adresy sa mozu roznit
float value = 0;
float rev = 0;
int rpm;
int oldtime = 0;
int time;

HX711 scale;
float calibration_factor = 650;
int GRAM;

void isr() {
  rev++;
}

void setup() {
  Serial.begin(9600);
  lcd.backlight();
  lcd.begin(24, 4); 
  lcd.init();
  attachInterrupt(digitalPinToInterrupt (2), isr, RISING); //interrupt pin
  
  scale.begin(DOUT, CLK); 
  //scale.set_scale(calibration_factor); //Adjust to this calibration factor
  //scale.tare();
}

void loop() {
  detachInterrupt(digitalPinToInterrupt(2));      
  time = millis() - oldtime;    //rozdiel casov
  
  //isr();
  rpm = (rev++ / time);   //vyrataj otacky/min
  oldtime = millis();           //uloz aktualny cas
  lcd.setCursor(0, 1);
  lcd.print(rpm);
  lcd.print(" ");
  lcd.print("RPM");
  
  Serial.print(rpm);
  Serial.print(" ");
  Serial.print("RPM");
  Serial.println();
  //attachInterrupt(digitalPinToInterrupt (2), isr, RISING);
  scale.set_scale();
  scale.tare();
  long zero_factor = scale.get_value();
  Serial.print("Weight : ");
  Serial.print(zero_factor);
  Serial.println(" Gram");
  
  lcd.setCursor(0, 0);
  lcd.print("Weight : ");
  lcd.print(zero_factor);
  lcd.println(" Gram");

  delay(100);
}
