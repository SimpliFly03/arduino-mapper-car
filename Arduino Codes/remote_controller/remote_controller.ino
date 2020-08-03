#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h> 
LiquidCrystal_I2C lcd(0x3f, 16, 2);
uint8_t check[8] = {0x0,0x1,0x3,0x16,0x1c,0x8,0x0};

RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";

int p2 = 0;
int p3 = 0;
int p6 = 0;
int p7 = 0;
int pot = 0;
String potx = "000";
String data_str = "0000000";
char data_nrf[8] = {'0', '0', '0', '0', '0', '0', '0', '\0'};
void setup() {
  delay(200);
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, check);
  lcd.setCursor(4,0);
  lcd.print("SEEEKISH");
  lcd.setCursor(4,1);
  lcd.print("ROBOTICS");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Seeekish Remote");
  lcd.setCursor(0,1);
  lcd.print("Controller V1.0");
  // put your setup code here, to run once:
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  pinMode(A0, INPUT);
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MAX);
  radio.stopListening();
}

void loop() {
  // put your main code here, to run repeatedly:
  p2 = digitalRead(2);
  p3 = digitalRead(3);
  p6 = digitalRead(6);
  p7 = digitalRead(7);
  pot = map(analogRead(A0), 0, 1023, 0, 255);

  if (pot < 10){
    potx = "00" + String(pot);
  }else if(pot < 100){
    potx = "0" + String(pot);
  }else{
    potx = String(pot);
  }
  
  data_str = String(p2) + String(p3) + String(p6) + String(p7) + potx;
  data_str.toCharArray(data_nrf, 8);

  radio.write(&data_nrf, sizeof(data_nrf));
  delay(100);
}
