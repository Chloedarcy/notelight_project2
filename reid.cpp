// led_music_player.cpp (PlatformIO version using SPIFFS)

#include <LiquidCrystal.h>
#include <FastLED.h>
#include <FS.h>
#include <SPIFFS.h>

#define NUM_LEDS 12
#define DATA_PIN 0
#define MAX_STEPS 128

CRGB leds[NUM_LEDS];

const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

struct NoteStep {
    bool isRest;
    float noteIndex;
    float duration; // in beats
};

NoteStep sequence[MAX_STEPS];
int sequenceLength = 0;

// Configurable BPM variable:
int bpm = 60; // Beats per minute

void loadMusicFile() {
    File file = SPIFFS.open("/music.txt", FILE_READ);
    if (!file) {
        lcd.setCursor(0, 0);
        lcd.print("File error");
        return;
    }

    // Read the entire file into a single string.
    String rawData = file.readString();
    file.close();
    rawData.trim();

    // Remove spaces that occur right after commas.
    rawData.replace(", ", ",");

    // Remove the outer square brackets if they exist.
    if (rawData.startsWith("[")) {
        rawData = rawData.substring(1);
    }
    if (rawData.endsWith("]")) {
        rawData = rawData.substring(0, rawData.length() - 1);
    }

    // Now rawData should be something like:
    // "['Rest',2.0],['Rest',2.0],[4,1.0],[4,0.5],..."
    int startIndex = 0;
    while (sequenceLength < MAX_STEPS && startIndex < rawData.length()) {
        // Find the delimiter "],"
        int endIndex = rawData.indexOf("],", startIndex);
        String entry;
        if (endIndex == -1) {
            // No more delimiters, so take the rest of the string.
            entry = rawData.substring(startIndex);
        } else {
            entry = rawData.substring(startIndex, endIndex);
        }

        entry.trim();

        // Remove any stray brackets from the beginning or ending of the entry.
        if (entry.startsWith("[")) {
            entry = entry.substring(1);
        }
        if (entry.endsWith("]")) {
            entry = entry.substring(0, entry.length() - 1);
        }

        // Now entry should be like "'Rest',2.0" or "4,1.0"
        if (entry.length() > 0) {
            int sep = entry.indexOf(',');
            if (sep != -1) {
                // Separate note and duration strings.
                String noteStr = entry.substring(0, sep);
                String durStr = entry.substring(sep + 1);
                noteStr.trim();
                durStr.trim();

                NoteStep step;
                // Check for different formats of the word "Rest"
                step.isRest = noteStr.equalsIgnoreCase("rest") 
                           || noteStr.equalsIgnoreCase("'Rest'") 
                           || noteStr.equalsIgnoreCase("\"Rest\"");
                step.noteIndex = step.isRest ? 0.0 : noteStr.toFloat();
                step.duration = durStr.toFloat(); // Here, duration is in beats

                sequence[sequenceLength++] = step;
            }
        }

        // If there was no delimiter found, break after processing the last entry.
        if (endIndex == -1) {
            break;
        }
        startIndex = endIndex + 2; // Skip past "],"
    }
}

void setup() {
    FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
    lcd.begin(16, 2);
    lcd.clear();

    if (!SPIFFS.begin(true)) {
        lcd.setCursor(0, 0);
        lcd.print("SPIFFS Failed");
        while (true);
    }

    loadMusicFile();
}

void loop() {
    for (int i = 0; i < sequenceLength; ++i) {
        NoteStep step = sequence[i];

        fill_solid(leds, NUM_LEDS, CRGB::Black);
        lcd.clear();
        lcd.setCursor(0, 0);

        if (step.isRest) {
            lcd.print("Rest");
        } else {
            // Use the noteIndex directly as the LED index.
            int ledIndex = constrain(int(step.noteIndex), 0, NUM_LEDS - 1);
            leds[ledIndex] = CRGB::Red;
            lcd.print("Note: ");
            lcd.print(step.noteIndex);
        }

        FastLED.show();
        // Calculate the delay in milliseconds based on BPM.
        // seconds_per_beat = 60 / bpm, then multiplied by the number of beats.
        int delayMs = step.duration * (60.0 / bpm) * 1000;
        delay(delayMs);

        fill_solid(leds, NUM_LEDS, CRGB::Black);
        FastLED.show();
        delay(100);
    }
    delay(2000);
}