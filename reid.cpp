// led_music_player.cpp (PlatformIO version using SPIFFS)

#include <LiquidCrystal.h>
#include <FastLED.h>
#include <FS.h>
#include <SPIFFS.h>

#define NUM_LEDS 88
#define DATA_PIN 0
#define MAX_STEPS 128

CRGB leds[NUM_LEDS];

const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

struct NoteStep {
    bool isRest;
    float noteIndex;
    float duration;
};

NoteStep sequence[MAX_STEPS];
int sequenceLength = 0;

void loadMusicFile() {
    File file = SPIFFS.open("/music.txt", FILE_READ);
    if (!file) {
        lcd.setCursor(0, 0);
        lcd.print("File error");
        return;
    }

    while (file.available() && sequenceLength < MAX_STEPS) {
        String line = file.readStringUntil('\n');
        line.trim();
        if (line.length() == 0) continue;

        line.replace("[", "");
        line.replace("]", "");
        line.replace("\"", "");
        line.replace("'", "");

        int sep = line.indexOf(',');
        if (sep == -1) continue;

        String noteStr = line.substring(0, sep);
        String durStr = line.substring(sep + 1);

        noteStr.trim();
        durStr.trim();

        NoteStep step;
        step.isRest = noteStr.equalsIgnoreCase("rest");
        step.noteIndex = step.isRest ? 0.0 : noteStr.toFloat();
        step.duration = durStr.toFloat();

        sequence[sequenceLength++] = step;
    }
    file.close();
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

        if (step.isRest) {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Rest");
        } else {
            int ledIndex = constrain(int(step.noteIndex) - 21, 0, NUM_LEDS - 1);
            leds[ledIndex] = CRGB::Red;

            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Note: ");
            lcd.print(step.noteIndex);
        }

        FastLED.show();
        delay(step.duration * 1000);

        fill_solid(leds, NUM_LEDS, CRGB::Black);
        FastLED.show();
        delay(100);
    }
    delay(2000);
}
