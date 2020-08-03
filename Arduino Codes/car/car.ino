#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <LiquidCrystal.h>
const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
uint8_t check[8] = {0x0,0x1,0x3,0x16,0x1c,0x8,0x0};
uint8_t cross[8] = {0x0,0x1b,0xe,0x4,0xe,0x1b,0x0};
const int in1 = 12; //22 7
const int in2 = 11; //23 6
const int in3 = 2; //25 4
const int in4 = 3; //24 5
RF24 radio(38, 53); // CE, CSN
const byte address[6] = "00001";
int speedd = 0;
int speedr = 0;
char speeds[4] = {'0' + '0' + '0' + '\0'};
int selection = 0;

//Sensor pins
const int front_trig = 49;
const int front_echo = 48;
const int left_trig = 43;
const int left_echo = 42;
const int right_trig = 47;
const int right_echo = 46;
const int back_trig = 45;
const int back_echo = 44;

int datacnt = 0;

void boot() {
  delay(200);
  lcd.begin(16, 2);
  lcd.createChar(0, check);
  lcd.createChar(1, cross);
  lcd.setCursor(4,0);
  lcd.print("SEEEKISH");
  lcd.setCursor(4,1);
  lcd.print("ROBOTICS");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.blink();
  delay(1500);
  lcd.noBlink();
  lcd.setCursor(0,0);
  lcd.print("Seeekish Remote");
  lcd.setCursor(0,1);
  lcd.print("Car V1.0");
  delay(2000);
}

int button(){
  int x;
  x = analogRead (A0);
  if (x < 60) {
    return 1; //Right
  }
  else if (x < 200) {
    return 2; //Up
  }
  else if (x < 400){
    return 3; //Down
  }
  else if (x < 600){
    return 4; //Left
  }
  else if (x < 800){
    return 5; //Select
  }else{
    return 0;
  }
}

void setup() {
  boot();
  pinMode(A0, INPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  
  pinMode(front_trig, OUTPUT);
  pinMode(front_echo,INPUT);
  pinMode(left_trig, OUTPUT);
  pinMode(left_echo,INPUT);
  pinMode(right_trig, OUTPUT);
  pinMode(right_echo,INPUT);
  pinMode(back_trig, OUTPUT);
  pinMode(back_echo,INPUT);
  
  Serial1.begin(115200);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();
  
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("RC Mode");
  lcd.setCursor(1,1);
  lcd.print("Wi-Fi Mode");
  lcd.setCursor(0,0);
  lcd.print(">");
  while(true){
    if(button() == 3){
      selection = 1;
    }else if(button() == 2){
      selection = 0;
    }else if(button() == 5){
      break;
    }
    if (selection == 1){
      lcd.setCursor(0,0);
      lcd.print(" ");
      lcd.setCursor(0,1);
      lcd.print(">");
    }else if(selection == 0){
      lcd.setCursor(0,1);
      lcd.print(" ");
      lcd.setCursor(0,0);
      lcd.print(">");
    }
    delay(100);
  }
  lcd.clear();
  delay(500);
  lcd.setCursor(0,0);
  lcd.print("Initialising");
  if(selection == 0){
    lcd.setCursor(0,1);
    lcd.print("RC Mode");
    delay(2000);
    lcd.setCursor(0,0);
    lcd.print("Running:      ");
    delay(500);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Signal: ");
    lcd.setCursor(0,1);
    lcd.print("Data: ");
  }else if(selection == 1){
    lcd.setCursor(0,1);
    lcd.print("Wi-Fi Mode");
    delay(2000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Wi-Fi Mode is");
    lcd.setCursor(0,1);
    lcd.print("not Implemented");
    while(true){
      delay(10000);
    }
    //lcd.setCursor(0,0);
    //lcd.print("Running:      ");
  }
}//  22 23 24 25

String sensor(char dir){
  long timee, dist;
  int trigPin, echoPin;
  String data = "000";
  if (dir == 'f'){
    trigPin = front_trig;
    echoPin = front_echo;
  }else if (dir == 'l'){
    trigPin = left_trig;
    echoPin = left_echo;
  }else if (dir == 'r'){
    trigPin = right_trig;
    echoPin = right_echo;
  }else if (dir == 'b'){
    trigPin = back_trig;
    echoPin = back_echo;
  }
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  timee = pulseIn(echoPin, HIGH);
  dist = timee /29.1/2;
  if (dist > 200){
    dist = 200;
  }
  if (dist < 10){
    data = "00" + String(dist);
  }else if (dist < 100){
    data = "0" + String(dist);
  }else{
    data = String(dist);
  }

  return data;
}

void mode_RC(){
  if (radio.available()) {
    lcd.setCursor(8,0);
    lcd.write(byte(0));
    char text[8] = "";
    radio.read(&text, sizeof(text));
    if(datacnt >= 3){
      Serial1.println(sensor('f') + " " + sensor('l') + " " + sensor('r') + " " + sensor('b'));
      datacnt = 0;
    }
    lcd.setCursor(8,1);
    lcd.print(text);
    speeds[0] = text[4];
    speeds[1] = text[5];
    speeds[2] = text[6];
    speedd = atoi(speeds);
    speedr = map(speedd, 0, 255, 0, 100);
    if(text[0] == '1' && text[2] == '1'){
      analogWrite(in1, speedd);
      analogWrite(in2, 0);
      analogWrite(in3, speedr);
      analogWrite(in4, 0);
    }else if(text[0] == '1' && text[3] == '1'){
      analogWrite(in1, speedr);
      analogWrite(in2, 0);
      analogWrite(in3, speedd);
      analogWrite(in4, 0);
    }else if(text[1] == '1' && text[3] == '1'){
      analogWrite(in1, 0);
      analogWrite(in2, speedd);
      analogWrite(in3, 0);
      analogWrite(in4, speedr);
    }else if(text[1] == '1' && text[2] == '1'){
      analogWrite(in1, 0);
      analogWrite(in2, speedr);
      analogWrite(in3, 0);
      analogWrite(in4, speedd);
    }else if (text[0] == '1'){
      analogWrite(in1, speedd);
      analogWrite(in2, 0);
      analogWrite(in3, speedd);
      analogWrite(in4, 0);
    }else if (text[1] == '1'){
      analogWrite(in1, 0);
      analogWrite(in2, speedd);
      analogWrite(in3, 0);
      analogWrite(in4, speedd);
    }else if (text[2] == '1'){
      analogWrite(in1, speedd);
      analogWrite(in2, speedd);
      analogWrite(in3, 0);
      analogWrite(in4, 0);
    }else if (text[3] == '1'){
      analogWrite(in1, 0);
      analogWrite(in2, 0);
      analogWrite(in3, speedd);
      analogWrite(in4, speedd);
    }else{
      analogWrite(in1, 0);
      analogWrite(in2, 0);
      analogWrite(in3, 0);
      analogWrite(in4, 0);
    }
    datacnt++;
  }else{
    //lcd.setCursor(8,0);
    //lcd.write(byte(1));
  }
}

void loop() {
  if(selection == 0){
    mode_RC();
  }
}  
