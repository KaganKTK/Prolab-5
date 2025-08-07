#include <LiquidCrystal.h>
#define StartEngine 24
#define Seatbelt 23
#define Engine 22
#define RedLed 31
#define Sicaklik A0
#define Klima 37
#define IsikOlcer A1
#define MaviLed 32
#define YakitGostergesi A15
#define SariLed 33
#define RGBRED 4
#define RGBBLUE 2
#define RGBGREEN 3
#define ArabaKapisi 35




LiquidCrystal lcd(30,29,28,27,26,25);
bool isSeatbeltOn = false;
int loopTime = -5;
bool isCarRunning = false;
bool FarInfo = false;


void setup() {
  lcd.begin(16, 2);
  pinMode(RedLed,OUTPUT);              
  pinMode(Engine,OUTPUT);
  pinMode(Seatbelt,INPUT_PULLUP);
  pinMode(StartEngine,INPUT_PULLUP);
  pinMode(Sicaklik,INPUT);
  Serial.begin(9600);
  pinMode(Klima,OUTPUT);
  pinMode(IsikOlcer,INPUT);
  pinMode(MaviLed,OUTPUT);
  pinMode(YakitGostergesi,INPUT);
  pinMode(SariLed,OUTPUT);
  pinMode(RGBBLUE,INPUT);
  pinMode(RGBRED,INPUT);
  pinMode(RGBGREEN,INPUT);
  pinMode(ArabaKapisi,INPUT_PULLUP);
}

void loop() {
  bool ProblemPresent = false;
  int KapiDegeri = digitalRead(ArabaKapisi);

  loopTime++;
  if(loopTime > 40){
    loopTime = -5;
  }
  delay(100);
  lcd.scrollDisplayLeft();
  double sicaklik = analogRead(Sicaklik);
  int RealSicaklik = sicaklik * 0.488;

  if(digitalRead(Seatbelt) == LOW){
    isSeatbeltOn = true;
    digitalWrite(RedLed,LOW);
    lcd.clear();
  }

  if(!isSeatbeltOn && digitalRead(StartEngine) == LOW){
    lcd.print("Emniyet Kemeri Takili Degil!"); digitalWrite(RedLed,HIGH); delay(200);
  }

  if(KapiDegeri && isSeatbeltOn){
    
    lcd.clear();
    lcd.setCursor(-loopTime,0);
    lcd.print("Uyari: Kapi Acik - Motor Calismaz");
    digitalWrite(Engine,LOW);
    isCarRunning = false;
    analogWrite(RGBBLUE,255-147);
    analogWrite(RGBRED,255-255);
    analogWrite(RGBGREEN,255-20);
    delay(100);
  }
  if(!KapiDegeri && isSeatbeltOn){
    lcd.clear();
    analogWrite(RGBBLUE,255);
    analogWrite(RGBRED,255);
    analogWrite(RGBGREEN,255);
  }

  if(isSeatbeltOn && digitalRead(StartEngine) == LOW && !KapiDegeri){
    digitalWrite(Engine,HIGH);
    lcd.clear();
    digitalWrite(RedLed,LOW);
    isCarRunning = true;
  }

  if(isCarRunning){
    if(RealSicaklik > 25 && !ProblemPresent){
    char mesaj[50];
    delay(100);
    lcd.clear();
    lcd.setCursor(-loopTime,0);
    sprintf(mesaj, "Sicaklik: %d%cC - Klima Acik", RealSicaklik, 223);
    lcd.print(mesaj);
    digitalWrite(Klima,HIGH);
    ProblemPresent = true;
    }
    else{
     lcd.clear();
     digitalWrite(Klima,LOW);
    }
    if(analogRead(IsikOlcer) <= 250 && !ProblemPresent){
      digitalWrite(MaviLed,HIGH);
      lcd.print("Farlar Yaniyor !");
      delay(1000);
      FarInfo = true;
      ProblemPresent = true;
    }
    else if(FarInfo){
      digitalWrite(MaviLed,LOW);
      lcd.print("Farlar Kapandi !");
      delay(1000);
      lcd.clear();
      FarInfo = false;
    }
    int yakitYuzdesi = analogRead(YakitGostergesi)/10.23;
    if(yakitYuzdesi > 5 && yakitYuzdesi < 10 && !ProblemPresent){
      
      digitalWrite(SariLed,HIGH);
      char metin[50];
      lcd.clear();
      lcd.setCursor(-loopTime,0);
      sprintf(metin,"Uyari: Yakit Seviyesi Dusuk - %d",yakitYuzdesi);
      lcd.print(metin);
      delay(100);
      
      ProblemPresent = true;
    }
    else if(yakitYuzdesi != 0 && yakitYuzdesi <= 5 && !ProblemPresent){
      digitalWrite(SariLed,LOW);
      delay(100);  
      digitalWrite(SariLed,HIGH);
      char metin[50];
      lcd.clear();
      lcd.setCursor(-loopTime,0);
      sprintf(metin,"Kritik: Yakit Cok Az - %d",yakitYuzdesi);
      lcd.print(metin);
      ProblemPresent = true;
    }
    else if (yakitYuzdesi == 0 && !ProblemPresent){
      isCarRunning = 0;
      lcd.print("Yakit bitti - Motor durdu !");
      digitalWrite(Engine,LOW);
      digitalWrite(RedLed,LOW);
      digitalWrite(MaviLed,LOW);
      digitalWrite(SariLed,LOW);
      ProblemPresent = true;
    }
    else if(!ProblemPresent){
      lcd.clear();
      digitalWrite(SariLed,LOW);
    }
  }
}
