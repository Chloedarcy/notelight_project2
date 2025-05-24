#include <LiquidCrystal_I2C.h>
#include <FastLED.h>
#include <FS.h>
#include <SPIFFS.h>

#define NUM_LEDS 12
#define DATA_PIN 0
#define MAX_STEPS 128

#define CENTER_BUTTON 4
#define DOWN_BUTTON 2
#define UP_BUTTON 15

CRGB leds[NUM_LEDS];
LiquidCrystal_I2C lcd(0x27, 20, 4); // LCD: 20x4

struct NoteStep {
    bool isRest;
    float noteIndex;
    float duration; // in beats
};

NoteStep sequence[MAX_STEPS];
int sequenceLength = 0;

// Configurable BPM
int bpm = 60;

String filePaths[] = {"/maryHadALittleLamb.txt", "/twinkleTwinkle.txt", "/runaway.txt"};
String songNames[] = {"Mary Had a Little Lamb", "Twinkle Twinkle Little Star", "Runaway"};
int songIndex = 0;
String selectedSong;

void flashLEDsTwice() {
    for (int i = 0; i < 3; i++) {
        delay(300);
        fill_solid(leds, NUM_LEDS, CRGB::Blue);
        FastLED.show();
        delay(300);
        fill_solid(leds, NUM_LEDS, CRGB::Black);
        FastLED.show();
    }
}

void showWelcomeScreen() {
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Welcome to NoteLight");
    flashLEDsTwice();
    delay(1500);
}

void selectSong() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Select a song:");

    int scrollPos = 0;
    unsigned long lastScrollTime = 0;
    const int scrollSpeed = 500; // milliseconds
    const int displayWidth = 20; // LCD width

    while (true) {
        // Prepare the full display string with prefix
        String name = "> " + songNames[songIndex];
        int fullLength = name.length();

        // Scroll the song name if it's too long
        if (millis() - lastScrollTime >= scrollSpeed) {
            lcd.setCursor(0, 1);
            String toDisplay = name.substring(scrollPos, scrollPos + displayWidth);
            while (toDisplay.length() < displayWidth) toDisplay += " "; // pad if short
            lcd.print(toDisplay);

            scrollPos++;
            if (scrollPos + displayWidth > fullLength) scrollPos = 0;

            lastScrollTime = millis();
        }

        // Button navigation
        if (digitalRead(UP_BUTTON) == LOW) {
            songIndex = (songIndex + 1) % (sizeof(songNames) / sizeof(songNames[0]));
            scrollPos = 0;
            delay(300);
        }

        if (digitalRead(DOWN_BUTTON) == LOW) {
            songIndex = (songIndex - 1 + (sizeof(songNames) / sizeof(songNames[0]))) % (sizeof(songNames) / sizeof(songNames[0]));
            scrollPos = 0;
            delay(300);
        }

        if (digitalRead(CENTER_BUTTON) == LOW) {
            selectedSong = filePaths[songIndex];
            lcd.clear();
            lcd.setCursor(0, 1);
            lcd.print("Selected!");
            delay(1000);
            return;
        }
    }
}


void selectTempo() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Set Tempo (BPM):");

    while (true) {
        lcd.setCursor(0, 1);
        lcd.print("                ");
        lcd.setCursor(0, 1);
        lcd.print("BPM: ");
        lcd.print(bpm);

        if (digitalRead(UP_BUTTON) == LOW) {
            bpm += 5;
            delay(200);
        }
        if (digitalRead(DOWN_BUTTON) == LOW) {
            bpm -= 5;
            if (bpm < 20) bpm = 20;
            delay(200);
        }
        if (digitalRead(CENTER_BUTTON) == LOW) {
            lcd.setCursor(0, 2);
            lcd.print("Tempo set!");
            delay(1000);
            return;
        }
    }
}

void waitToStart() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Press CENTER to");
    lcd.setCursor(0, 1);
    lcd.print("Start Playback");
    while (digitalRead(CENTER_BUTTON) != LOW) {
        delay(100);
    }
    lcd.clear();
    lcd.print("Starting...");
    delay(1000);
}

void loadMusicFile() {
    File file = SPIFFS.open(selectedSong, FILE_READ);
    if (!file) {
        lcd.clear();
        lcd.print("File error");
        delay(2000);
        return;
    }

    String rawData = file.readString();
    file.close();

    if (rawData.length() == 0) {
        lcd.clear();
        lcd.print("File empty");
        delay(2000);
        return;
    }

    rawData.replace(", ", ",");
    if (rawData.startsWith("[")) rawData = rawData.substring(1);
    if (rawData.endsWith("]")) rawData = rawData.substring(0, rawData.length() - 1);

    sequenceLength = 0;
    int startIndex = 0;
    while (sequenceLength < MAX_STEPS && startIndex < rawData.length()) {
        int endIndex = rawData.indexOf("],", startIndex);
        String entry = (endIndex == -1) ? rawData.substring(startIndex)
                                        : rawData.substring(startIndex, endIndex);
        entry.trim();
        if (entry.startsWith("[")) entry = entry.substring(1);
        if (entry.endsWith("]")) entry = entry.substring(0, entry.length() - 1);

        int sep = entry.indexOf(',');
        if (sep != -1) {
            String noteStr = entry.substring(0, sep);
            String durStr = entry.substring(sep + 1);
            noteStr.trim();
            durStr.trim();

            NoteStep step;
            step.isRest = noteStr.equalsIgnoreCase("rest") 
                       || noteStr.equalsIgnoreCase("'Rest'")
                       || noteStr.equalsIgnoreCase("\"Rest\"");
            step.noteIndex = step.isRest ? 0.0 : noteStr.toFloat();
            step.duration = durStr.toFloat();

            sequence[sequenceLength++] = step;
        }

        if (endIndex == -1) break;
        startIndex = endIndex + 2;
    }


    delay(2000);
}

String noteToLetter(int noteIndex) {
    switch (noteIndex) {
        case 0: return "C";
        case 1: return "D";
        case 2: return "E";
        case 3: return "F";
        case 4: return "G";
        case 5: return "A";
        case 6: return "B";
        case 7: return "C#"; // or high C or whatever your mapping is
        default: return "?";
    }
}

void playSequence() {
    for (int i = 0; i < sequenceLength; ++i) {
        NoteStep step = sequence[i];
        fill_solid(leds, NUM_LEDS, CRGB::Black);
        lcd.clear();

        if (step.isRest) {
            lcd.setCursor(0, 0);
            lcd.print("Rest");
        } else {
            int ledIndex = constrain(int(step.noteIndex), 0, NUM_LEDS - 1);
            leds[ledIndex] = CRGB::Red;
            lcd.setCursor(0, 0);
            lcd.print("Note: ");
            lcd.print(noteToLetter(int(step.noteIndex)));
        }

        FastLED.show();
        int delayMs = step.duration * (60.0 / bpm) * 1000;
        delay(delayMs);
        fill_solid(leds, NUM_LEDS, CRGB::Black);
        FastLED.show();
        delay(100);
    }
}

bool askToPlayAgain() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Play again?");
    lcd.setCursor(0, 1);
    lcd.print("UP: Yes  DOWN: No");

    while (true) {
        if (digitalRead(UP_BUTTON) == LOW) {
            delay(500);
            return true;
        }
        if (digitalRead(DOWN_BUTTON) == LOW) {
            delay(500);
            return false;
        }
    }
}

void setup() {
    FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
    pinMode(UP_BUTTON, INPUT_PULLUP);
    pinMode(DOWN_BUTTON, INPUT_PULLUP);
    pinMode(CENTER_BUTTON, INPUT_PULLUP);

    lcd.init();
    lcd.backlight();

    if (!SPIFFS.begin(true)) {
        lcd.print("SPIFFS Failed");
        while (true);
    }

    showWelcomeScreen();
}

void loop() {
    selectSong();
    selectTempo();
    waitToStart();
    loadMusicFile();
    playSequence();

    if (!askToPlayAgain()) {
        return; // Go back to song selection
    }
}
