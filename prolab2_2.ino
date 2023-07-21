#include <Key.h>
#include <Keypad.h>
#include <LiquidCrystal.h>

int pir_sensor_pini = 13;
int role_pini = 12;
boolean pir_sensor_degeri = 0;

int flame_sensor_pini = 11;
int buzzer_pini = 10;
int flame_sensor_degeri;

int lm35_pini = A1;
float sensor_degeri = 0;
float gerilim_degeri = 0;
float sicaklik_degeri = 0;
LiquidCrystal lcd(9,8,7,6,5,4);

const byte satir_sayisi = 4;//sifre satir
const byte sutun_sayisi = 3; //sifre sutun
#define girilecek_sifre_uzunlugu 5
char veri[girilecek_sifre_uzunlugu];
char girilecek_sifre[girilecek_sifre_uzunlugu] = "1919";
byte veri_count = 0,sifre_count = 0;
int kirmizi_led = 3;
int yesil_led = 2;
char tus_takimi[satir_sayisi][sutun_sayisi]={
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
  
  };
byte satir_pinleri[satir_sayisi] = {17,18,19,20}; 
byte sutun_pinleri[sutun_sayisi] = {16,15,14}; 
Keypad pad = Keypad(makeKeymap(tus_takimi), satir_pinleri, sutun_pinleri, satir_sayisi ,sutun_sayisi);



void setup() {
  Serial.begin(9600);
  
  pinMode(pir_sensor_pini, INPUT);
  pinMode(role_pini, OUTPUT);
  
  pinMode(flame_sensor_pini, INPUT);
  pinMode(buzzer_pini, OUTPUT);
  
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  //analogReference (INTERNAL1V1); 
  
  pinMode(kirmizi_led,OUTPUT); 
  pinMode(yesil_led, OUTPUT); 
  digitalWrite(kirmizi_led, LOW);
  digitalWrite(yesil_led, LOW);
}
void clearData()
{
      
      while(veri_count!=0)
      {
        veri[veri_count--]=0;
       }
        return;
}


void loop(){
  
  pir_sensor_degeri = digitalRead(pir_sensor_pini);
  if (pir_sensor_degeri == 1)
  {
    digitalWrite(role_pini, HIGH);
  } 
  else 
  {
    digitalWrite(role_pini, LOW);
  }
  
  flame_sensor_degeri = digitalRead(flame_sensor_pini);
  if (flame_sensor_degeri == 1) 
  {
    digitalWrite(buzzer_pini, HIGH);
  } 
  else 
  {
    digitalWrite(buzzer_pini, LOW);
  }

  // LM35 sicaklik sensoru derece basina 10mV cikis verir.1V=1000mV
  sensor_degeri = analogRead(lm35_pini);
  gerilim_degeri = (sensor_degeri / 1023.0)*5000;
  sicaklik_degeri = (gerilim_degeri / 10.0);
 
  lcd.print(sicaklik_degeri);
  
  if(sicaklik_degeri < 20.0)
  {
    lcd.setCursor(0,1);
    lcd.print("Sicaklik AZALDI!");
    delay(100);
    lcd.clear();
    
  }
  else if(sicaklik_degeri > 30.0)
  {
    
    lcd.setCursor(0,1);
    lcd.print("Sicaklik ARTTI!");
    delay(100);
    lcd.clear();
    
  }
  else
  {
    lcd.setCursor(0,1);
    lcd.print("Normal Sicaklik");
    delay(100);
    lcd.clear();
  }
  char customKey=pad.getKey();
  if(customKey)
  {
    veri[veri_count]=customKey;
    veri_count++;
  }
    
  if(veri_count == girilecek_sifre_uzunlugu-1)
  {
      if(!strcmp(veri,girilecek_sifre))
      {
        digitalWrite(yesil_led,HIGH);
        digitalWrite(kirmizi_led,LOW);
      }
      else
      {
          digitalWrite(kirmizi_led,HIGH);
          digitalWrite(yesil_led,LOW);
      }
       clearData();
   }

 }
