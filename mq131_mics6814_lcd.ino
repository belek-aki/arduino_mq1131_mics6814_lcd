#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "MQ131.h"

// Настройка адреса I2C для LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);  // здесь 0x27 - адрес I2C, 16 колонок и 2 строки

// Аналоговые пины для подключения датчиков
const int pinQM131 = A0; // аналоговый вход для QM131
const int pinMICS6814CO = A1; // CO аналоговый вход для MICS-6814
const int pinMICS6814NH3 = A2; // NH3 аналоговый вход для MICS-6814
const int pinMICS6814NO2 = A3; // CO2 аналоговый вход для MICS-6814

int co,nh3;
float no2;
const int S_analog = 1023.0;

void setup() {
  // Инициализация портов ввода/вывода
  Serial.begin(115200); // начать серийную связь для отладки
  MQ131.begin(2,pinQM131, HIGH_CONCENTRATION, 1000000);
  MQ131.calibrate();
  pinMode(pinMICS6814CO, INPUT);
  pinMode(pinMICS6814NH3, INPUT);
  pinMode(pinMICS6814NO2, INPUT);
  pinMode(pinQM131, INPUT);
  // Инициализация MQ131
  Serial.println("Calibration done!");
  Serial.print("R0 = ");
  Serial.print(MQ131.getR0());
  Serial.println(" Ohms");
  Serial.print("Time to heat = ");
  Serial.print(MQ131.getTimeToRead());
  Serial.println(" s");
  // Инициализация LCD дисплей6
  lcd.init();
  lcd.backlight();
  lcd.setCursor(2,0);
  lcd.print("Home gas");
  lcd.setCursor(2,1);
  lcd.print("Analyzer");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Чтение значений с аналоговых входов
  co = map(analogRead(pinMICS6814CO), 0, S_analog, 1, 1000);
  nh3 = map(analogRead(pinMICS6814NH3), 0, S_analog, 1, 500);
  no2 = (map(analogRead(pinMICS6814NO2), 0, S_analog, 5, 1000)) / 100.0;
  MQ131.sample();
  int valueQM131 = analogRead(pinQM131);
  // Отображение значений на LCD
  lcd.setCursor(0, 0); // установить курсор в первую строку, первый символ
  lcd.print("CO:");
  lcd.print(co);
  lcd.print("ppm");
  lcd.setCursor(8, 0); // установить курсор в первую строку, первый символ
  lcd.print("CO:");
  lcd.print(nh3);
  lcd.print("ppm");
  lcd.setCursor(0, 1); // установить курсор во вторую строку, первый символ
  lcd.print("O3:");
  lcd.print(valueQM131);
  lcd.print("ppm");
  lcd.setCursor(8, 1); // установить курсор в первую строку, первый символ
  lcd.print("NO2:");
  lcd.print(MQ131.getO3(PPM));
  lcd.print("ppm");
  delay(1000); // задержка 1 секунда
}
