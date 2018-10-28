#include <Arduino.h>
#include <NewTone.h>
//#include <boarddefs.h>
#include <IRremote.h>
#include <IRremoteInt.h>
#include <LiquidCrystal.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
#define NEC_ZERO 0xFF6897

#define DHT_PIN 2
#define IR_PIN 13
#define PB_PIN 3


DHT dht(DHT_PIN, DHT11);
IRrecv ir(IR_PIN);
enum { WEATHER_DISP_MODE, IR_DISP_MODE};
int lcd_display_mode = WEATHER_DISP_MODE;


const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
int melody[] = { 262, 196, 196, 220, 196, 0, 247, 262 };
int noteDurations[] = { 4, 8, 8, 4, 4, 4, 4, 4 };


//LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
LiquidCrystal lcd(7,8,9,10,11,12);
int dht_t_elapsed =0;



void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600);
  lcd.print("hello, world!");
  lcd.setCursor(0,1);
  lcd.print("~~~~~~");
  dht.begin();
  ir.enableIRIn();
  pinMode(PB_PIN, OUTPUT);
  digitalWrite(PB_PIN, LOW); 
  
}
void loop() {
  decode_results dr;


  if (ir.decode(&dr)) {
    if (dr.decode_type == NEC) {
      Serial.print("NEC: ");
      if(dr.value == 0xFF6897) Serial.println("WEATHER_DISP_MODE");
      if(dr.value == 0xFF30CF) {
        Serial.println("TOGGLING PIN 3");
          NewTone(PB_PIN, 200);
          delay(500);
          noNewTone(PB_PIN);
          delay(500);
            for (int thisNote = 0; thisNote < 8; thisNote++) { // Loop through the notes in the array.
    int noteDuration = 1000/noteDurations[thisNote];
    NewTone(PB_PIN, melody[thisNote], noteDuration); // Play thisNote for noteDuration.
    delay(noteDuration * 4 / 3); // Wait while the tone plays in the background, plus another 33% delay between notes.
  }

          
      }
    } else if (dr.decode_type == SONY) {
      Serial.print("SONY: ");
    } else if (dr.decode_type == RC5) {
      Serial.print("RC5: ");
    } else if (dr.decode_type == RC6) {
      Serial.print("RC6: ");
    } else if (dr.decode_type == UNKNOWN) {
      Serial.print("UNKNOWN: ");
    }
    Serial.println(dr.value, HEX);
    ir.resume(); // Receive the next value
  }

  
 // int chk = dht.read(DHT_PIN);
 if(lcd_display_mode == WEATHER_DISP_MODE && millis() - dht_t_elapsed > 2000){
  dht_t_elapsed = millis();
  lcd.setCursor(0,0); 
  lcd.print("Temp: ");
  lcd.print(dht.readTemperature());
  lcd.print((char)223);
  lcd.print("C");
  lcd.setCursor(0,1);
  lcd.print("Humidity: ");
  lcd.print(dht.readHumidity());
  lcd.print("%");
 }
}