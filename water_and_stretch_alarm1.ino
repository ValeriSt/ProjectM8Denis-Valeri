#define LED_PIN     13

#define TRIGGER_PIN_WATER  7
#define ECHO_PIN_WATER     6
#define TRIGGER_PIN_STRETCH 8
#define ECHO_PIN_STRETCH 9

#define DECREASE_TIME_BUTTON A1
#define INCREASE_TIME_BUTTON A2
#define SET_TIME_BUTTON A3
#define SWAP_MODE_BUTTON A4

#define BUZZER_PIN      A0
#define BUZZER_NOTE     330
#define BUZZER_DURATION 1000

#define DISTANCE 50

#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

signed short waterHours, waterMinutes, waterSeconds;
signed short customWaterHours = 01, customWaterMinutes, customWaterSeconds;
signed short stretchHours, stretchMinutes, stretchSeconds;
signed short customStretchHours = 01, customStretchMinutes = 30, customStretchSeconds;
char timeline[16];
int count;

void startTimer()
{
  if(waterHours < 01 && waterMinutes < 30){
    waterHours = customWaterHours;
    waterMinutes = customWaterMinutes;
    waterSeconds = 00;
  }
  if(stretchHours < 01 && stretchMinutes < 30){
    stretchHours = customStretchHours;
    stretchMinutes = customStretchMinutes;
    stretchSeconds = 00;
  }
  count = 0;
  while(readUltrasonicDistance(TRIGGER_PIN_WATER, ECHO_PIN_WATER) < DISTANCE
        && readUltrasonicDistance(TRIGGER_PIN_STRETCH, ECHO_PIN_STRETCH) < DISTANCE){
    if(waterSeconds %5 == 0){
      count++;
    }
    lcd.clear();
    if(count %2 == 0){
      lcd.setCursor(0, 0);
      lcd.print("Drink water in...");
      lcd.setCursor(0, 1);
      sprintf(timeline,"%0.2d:%0.2d:%0.2d", waterHours, waterMinutes, waterSeconds);
      lcd.print(timeline);
    }
    else{
      lcd.setCursor(0, 0);
      lcd.print("Stretch in...");
      lcd.setCursor(0, 1);
      sprintf(timeline,"%0.2d:%0.2d:%0.2d", stretchHours, stretchMinutes, stretchSeconds);
      lcd.print(timeline);
    }

    delay(1000);
    waterSeconds--;

    if (waterSeconds == -01){
      waterSeconds = 59;
      waterMinutes--;
    }
    if (waterMinutes == -01){
      waterMinutes = 59;
      waterHours--;
    }
    if(waterHours == 00 && waterMinutes == 00 && waterSeconds == 00)
    {
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("DRINK WATER!!!");
      lcd.setCursor(0, 0);
      lcd.print("DRINK WATER!!!");
      playAlarm(TRIGGER_PIN_WATER, ECHO_PIN_WATER);
    }
    
    stretchSeconds--;
    
    if (stretchSeconds == -01){
      stretchSeconds = 59;
      stretchMinutes--;
    }
    if (stretchMinutes == -01){
      stretchMinutes = 59;
      stretchHours--;
    }
    if(stretchHours == 00 && stretchMinutes == 00 && stretchSeconds == 00)
    {
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("DO STRETCHES!!!");
      lcd.setCursor(0, 0);
      lcd.print("GET UP!!!");
      playAlarm(TRIGGER_PIN_STRETCH, ECHO_PIN_STRETCH);
    }
  }
}

void SetTimer()
{
  bool settingTimer = 1;
  count = 0;
  delay(1000);
  while(settingTimer){
    lcd.clear();
    if(count %2 == 0){
      lcd.setCursor(0, 0);
      lcd.print("WaterAlarmTimer:");
      lcd.setCursor(0, 1);
      sprintf(timeline,"%0.2d:%0.2d:%0.2d"
              , customWaterHours, customWaterMinutes, customWaterSeconds);
      lcd.print(timeline);
    }
    else{
      lcd.setCursor(0, 0);
      lcd.print("Stretch Timer:");
      lcd.setCursor(0, 1);
      sprintf(timeline,"%0.2d:%0.2d:%0.2d"
              , customStretchHours, customStretchMinutes, customStretchSeconds);
      lcd.print(timeline);
    }
    
    if(digitalRead(SWAP_MODE_BUTTON)){
      count++;
    }
    
    if(digitalRead(INCREASE_TIME_BUTTON)){
      if(count % 2 == 0){
        customWaterMinutes += 15;
        if(customWaterMinutes == 60){
          customWaterHours++;
          customWaterMinutes = 00;
        }
      }
      else{
        customStretchMinutes += 15;
        if(customStretchMinutes == 60){
          customStretchHours++;
       	  customStretchMinutes = 00;
        }
      }
    }
    
    if(digitalRead(DECREASE_TIME_BUTTON)){
      if(count % 2 == 0){
        customWaterMinutes -= 15;
        if(customWaterMinutes == -15){
          customWaterHours--;
          customWaterMinutes = 45;
        }
        if(customWaterHours == -01){
          customWaterHours = 00;
        }
      }
      else{
        customStretchMinutes -= 15;
        if(customStretchMinutes == -15){
          customStretchHours--;
       	  customStretchMinutes = 45;
        }
        if(customStretchHours == -01){
          customStretchHours = 00;
        }
      }
    }
    
    if(digitalRead(SET_TIME_BUTTON)){
      settingTimer = 0;
    }
    
    delay(100);
  }
}

long readUltrasonicDistance(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT);
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);

  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);

  return (pulseIn(echoPin, HIGH)*0.01723);
}

void playAlarm(int triggerPin, int echoPin) 
{
    while(readUltrasonicDistance(triggerPin, echoPin) < DISTANCE) {
        digitalWrite(LED_PIN, HIGH);

        tone(BUZZER_PIN, BUZZER_NOTE, BUZZER_DURATION);
        delay(BUZZER_DURATION * 1.0);
        noTone(BUZZER_PIN);

        delay(100);
    }
    digitalWrite(LED_PIN, LOW);
}

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(DECREASE_TIME_BUTTON, INPUT);
  pinMode(INCREASE_TIME_BUTTON, INPUT);
  pinMode(SET_TIME_BUTTON, INPUT);
  pinMode(SWAP_MODE_BUTTON, INPUT);
  lcd.begin(16, 2);
}

void loop()
{
  if (readUltrasonicDistance(TRIGGER_PIN_WATER, ECHO_PIN_WATER) < DISTANCE
      && readUltrasonicDistance(TRIGGER_PIN_STRETCH, ECHO_PIN_STRETCH) < DISTANCE){
    startTimer();
  }
  if(digitalRead(SET_TIME_BUTTON)){
    SetTimer();
    waterHours = customWaterHours;
    waterMinutes = customWaterMinutes;
    waterSeconds = 00;
    
    stretchHours = customStretchHours;
    stretchMinutes = customStretchMinutes;
    stretchSeconds = 00;
    }
  
  delay(500);
}