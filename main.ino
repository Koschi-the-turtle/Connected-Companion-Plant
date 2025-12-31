#define SOIL_PIN A0
#define LIGHT_PIN A1
#define SPEAKER_PIN 8

#include <LiquidCrystal_I2C.h>

enum PlantState {
    THIRSTY, 
    OKAY,
    DROWNING,
    DARK
};


PlantState currentState;
PlantState previousState;

#define DRY_THRESHOLD 300
#define DROWNING_THRESHOLD 700
#define DARK_THRESHOLD 300

LiquidCrystal_I2C lcd (0x27, 16, 2);

void playHappy() {
    tone(SPEAKER_PIN, 1200, 150);
}
void playSad() {
    tone(SPEAKER_PIN, 300, 300);
}
void playAlarm() {
    for (int i=0; i<3; i++) {
        tone(SPEAKER_PIN, 800, 100);
        delay(150);
    }
}

void showMessage(PlantState state) {
    lcd.clear();
    switch (state) {
        case THIRSTY:
            lcd.setCursor(0, 0);
            lcd.print("HELP! I NEED WATER");
            lcd.setCursor(0, 1);
            lcd.print("(x_x)");
            break;
        case OKAY:
            lcd.setCursor(0, 0);
            lcd.print("Feeling good");
            lcd.setCursor(0, 1);
            lcd.print("(^-^)");
            break;
        case DROWNING:
            lcd.setCursor(0, 0);
            lcd.print("BRO STOP I'M");
            lcd.setCursor(0, 1);
            lcd.print("DROWNING! (x_x)");
            break;
        case DARK:
            lcd.setCursor(0, 0);
            lcd.print("WHERE DID THE");
            lcd.setCursor(0, 1);
            lcd.print("SUN GO ?! (*-*)");
    }
}

PlantState determineState(int soilValue) {
    if (soilValue <= DRY_THRESHOLD) return THIRSTY;
    if (soilValue <= DROWNING_THRESHOLD) return OKAY;
    return DROWNING;
}
PlantState determineLightState(int lightValue) {
    if (lightValue <= DARK_THRESHOLD) return DARK;
    return OKAY;
}

PlantState combineStates(PlantState water, PlantState light) {
    if (water == THIRSTY)
        return THIRSTY;
    if (water == DROWNING)
        return DROWNING;
    if (light == DARK)
        return DARK;
    return OKAY;

}
void onStateChange() {
    showMessage(currentState);
    switch (currentState) {
        case THIRSTY:
        case DARK:
            playSad();
            break;
        case OKAY:
            playHappy();
            break;
        case DROWNING:
            playAlarm();
            break;
    }
}

void setup() {
    pinMode(SPEAKER_PIN, OUTPUT);
    pinMode(SOIL_PIN ,INPUT);
    pinMode(LIGHT_PIN, INPUT);
    lcd.init();
    lcd.blacklight();
    currentState = OKAY;
    previousState = OKAY;
    showMessage(currentState);
}




void loop() {
    int soilValue = analogRead(SOIL_PIN);
    int lightValue = analogRead(LIGHT_PIN);

    PlantState waterState = determineState(soilValue);
    PlantState lightState = determineLightState(lightValue);
    
    previousState = currentState;
    currentState = combineStates(waterState, lightState);

    if (currentState != previousState) {
        onStateChange();
    }
    delay(500);
}