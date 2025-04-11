#include <FastLED.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define NUM_LEDS 12      // Number of LEDs in your strip
#define DATA_PIN 0         // Data pin connected to the LED strip
#define LED 12

CRGB leds[NUM_LEDS];

LiquidCrystal_I2C lcd(0x27, 20, 4); // Set the LCD address to 0x27 for a 20 chars and 4 line display

int defaultBpm = 100;
const int speakerPin = 7;
//const int upButtonPin = 6;
//const int downButtonPin = 4;
//const int selectButtonPin = 5;
double bpmConst, bpm;
int upButtonState, downButtonState, selectButtonState, selectedSong;
boolean songSelected = false;
boolean tempoSelected = false;
String songs[] = {"Mary Had a Little Lamb", "Twinkle Twinkle Little Star", "Runaway"};
int mappedValue;  

void setup() {
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  pinMode(LED,OUTPUT);
 
  lcd.init();                      // Initialize the LCD
  lcd.backlight();                 // Turn on the backlight
  lcd.setCursor(0, 0);             // Set cursor to column 0, row 0
  lcd.print("Welcome to NoteLight");      // Print text

  delay(3000);

  //pinMode(speakerPin, OUTPUT);

  // Button Pins
 // pinMode(upButtonPin, INPUT);
 // pinMode(downButtonPin, INPUT);
 // pinMode(selectButtonPin, INPUT);

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Red;
    delay(300);
  }
  FastLED.show();
}

void loop() {
 //

 
  // State is LOW when pressed and HIGH when not
 // upButtonState = digitalRead(upButtonPin);
 // downButtonState = digitalRead(downButtonPin);
 // selectButtonState = digitalRead(selectButtonPin);

  //if(upButtonState == true){
  //  leds[0] = CRGB::Blue;
 // }
 // if(downButtonState == true){
 //   leds[1] = CRGB::Blue;
 // }
 // if(selectButtonState == true){
  //  leds[2] = CRGB::Blue;
//  }

  // Print out all the songs
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Choose a song:");


  if(songSelected == false){
     // Display all songs
    for (int i = 0; i < 3; i++) {
      lcd.setCursor(0, i + 1);  // Move to the next line for each song
      lcd.print(songs[i]);  // Display the song name
    }
  }
}
   /*
    // Button functionality for scrolling through songs
    if (upButtonState == LOW) {
      if (selectedSong > 0) {
        selectedSong--;  // Move up the list
        delay(200);
      }
    }
   
    if (downButtonState == LOW) {
      if (selectedSong < sizeof(songs) - 1) {  // Don't go past the last song
        selectedSong++;  // Move down the list
        delay(200);  
      }
    }
   
    if (selectButtonState == LOW) {
      songSelected = true;
      lcd.clear();
      lcd.setCursor(0, 0);  
      lcd.print("Choose a tempo: ");
 
     }    
  }
*/

  /*if(tempoSelected == false && songSelected == true){
   lcd.setCursor(0, 1);
   
    // Control with pot
    double bpmConst = 60.0 / potPosition * 1000.0;
    double bpm = (double)millis() / bpmConst;  
    bpm = fmod(bpm - 1, 8.0) + 1;  // Wrap bpm between 1 and 8

    lcd.print(String(bpm) + " bpm");
   
    if (selectButtonState == LOW) {
      tempoSelected = true;
      lcd.clear();
      lcd.setCursor(0, 0);  
     }    
  }

   if(tempoSelected == true && songSelected == true){
      lcd.print("Song: ");
     
      lcd.setCursor(0, 1);
      lcd.print("Metronome: ");
      if ((int)bpm == bpm) {
        lcd.print((int)bpm);  
        tone(speakerPin, 1200, 100);
    }
}*/
