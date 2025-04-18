#include <FastLED.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define NUM_LEDS 12      // Number of LEDs in your strip
#define DATA_PIN 0         // Data pin connected to the LED strip
#define CENTER_BUTTON 15
#define DOWN_BUTTON 2
#define UP_BUTTON 4

CRGB leds[NUM_LEDS];

LiquidCrystal_I2C lcd(0x27, 20, 4); // Set the LCD address to 0x27 for a 20 chars and 4 line display

int defaultBpm = 100;
const int speakerPin = 7;
double bpmConst, bpm;
int upButtonState, downButtonState, selectButtonState, selectedSong;
boolean songSelected = false;
boolean tempoSelected = false;
String songs[] = {"Mary Had a Little Lamb", "Twinkle Twinkle Little Star", "Runaway"};
int mappedValue;

int lastStateUP = HIGH;
int currentStateUP;
int lastStateDOWN = HIGH;
int currentStateDOWN;
int lastStateCENTER = HIGH;
int currentStateCENTER;

CRGB color;

void updateLEDs() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = color; 
  }
  FastLED.show();
}


void setup() {
  Serial.begin(9600);  // Initialize Serial communication at 9600 baud
  Serial.println("Hello, world!");

  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  pinMode(UP_BUTTON, INPUT_PULLUP);
  pinMode(DOWN_BUTTON, INPUT_PULLUP);
  pinMode(CENTER_BUTTON, INPUT_PULLUP);

  lcd.init(); // Initialize the LCD
  lcd.backlight(); 
  lcd.setCursor(0, 0); 
  lcd.print("Welcome to NoteLight"); 
 // delay(3000);

  // Turn on all the LEDs initially
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Red;
    delay(100);
  }
  FastLED.show();
}

void loop() {
  // Read button states
  int readingUP = digitalRead(UP_BUTTON);
  int readingDOWN = digitalRead(DOWN_BUTTON);
  int readingCENTER = digitalRead(CENTER_BUTTON);

  // Check if the UP button is pressed
  if (readingUP == LOW && lastStateUP == HIGH) {
    color = CRGB::Green; 

    updateLEDs();
  }

  // Check if the DOWN button is pressed
  if (readingDOWN == LOW && lastStateDOWN == HIGH) {
    color = CRGB::Purple; 
    updateLEDs();
  }

  // Check if the CENTER button is pressed
 /* if (readingCENTER == LOW && lastStateCENTER == HIGH) {
    color = CRGB::Blue; 
    updateLEDs();
  }*/

  // Save the current button state for next loop iteration
  lastStateUP = readingUP;
  lastStateDOWN = readingDOWN;
  lastStateCENTER = readingCENTER;

  // Display songs on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Choose a song:");

  if (!songSelected) {
    for (int i = 0; i < 3; i++) {
      lcd.setCursor(0, i + 1);
      lcd.print(songs[i]);
    }
  }
  delay(100); 

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
