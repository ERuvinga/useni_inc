#include <LiquidCrystal_I2C.h>
#include<Wire.h>
#include "dht.h"
#include "rtc.h"
#include "moteur.h"

  // Composants connectes sur l'arduino
#define DHT_PIN 12
// leds pour le control de la temperature
#define RED_LED_T 11
#define GREEN_LED_T 10

//leds pour le control des jours
#define RED_LED_D 9
#define GREEN_LED_D 8

//AUTRES
#define BUZZER 7
#define RELAIS_RES 6
#define RELAIS_BRIS 5 

#define BT_2 2
#define BT_3 3
#define BT_18 18
#define BT_19 19


unsigned long temp_lcd, temp_b1, temp_b2;
bool memo_2 = 0, memo_18= 0, memo_19 = 0; // pour la memorisation d'etats des boutons

void beginer(){
  
  lcd_tempe.init(); // initialisation de l'afficheur
  Serial.begin(9600);
  Wire.begin(); //initialisation de la voie i2c
  
  pinMode(RED_LED_T , OUTPUT);
  pinMode(GREEN_LED_T, OUTPUT);
  pinMode(RED_LED_D  , OUTPUT);
  pinMode(GREEN_LED_D, OUTPUT);
  
  pinMode(BUZZER, OUTPUT);
  pinMode(RELAIS_RES , OUTPUT);
  pinMode(RELAIS_BRIS, OUTPUT);

  //pins du moteur
  pinMode(PIN_MOTEUR_a, OUTPUT);
  pinMode(PIN_MOTEUR_d, OUTPUT);

  pinMode(BT_2, INPUT);
  pinMode(BT_3, INPUT);
  pinMode(BT_18, INPUT);
  pinMode(BT_19, INPUT);
}

void setup() {
  
    if (flag_retournement != (EEPROM.read(RETOURNEMENT))){
      flag_retournement = (EEPROM.read(RETOURNEMENT));
    }
    
    beginer();
    temp_lcd = millis();
    temp_b1=millis();
    temp_b2=millis();
    lcd_tempe.backlight();
    
  //(sec, mins, heur, dow, dom, moi, an) mise a jours de l'heure et de la date
  //update_time(0, 35, 17, 5, 18, 3, 22);

  download_time( &dateTime ); // telecharge l'heure et la date
  heure_actu = dateTime.hours;  //on sauvegarde l'heure aussi 
 }

void loop() {

appel_fonction();
 
  if(( millis() - temp_lcd) >= 1000){
   temp_lcd = millis(); 
   lcd_clear(flag_lcd_clear);
   menu(flag_lcd);
   }

  if(( millis() - temp_b1) >= 500){
   temp_b1= millis(); 
   control_buzzer();
   }

   retournement();
   compteur(); //active lorsqu'on veut initialiser

}

void appel_fonction(){
  
  memorySave( &flag_save );
  download_time( &dateTime );
  readDHT( DHT_PIN, &tempe, &humidy ); // mise ajour de la temperature 
  control_temperature();

  //lecture des booutons
  lecture_bt2();
  lecture_bt18();
  lecture_bt19();
}


void control_temperature(){
  
  if(tempe <= 37.75){
   digitalWrite(RELAIS_RES, HIGH);
  }

  else if(tempe > 37.8){
   digitalWrite(RELAIS_RES, LOW);
  }

   control_humidity();
   control_leds_T();
   control_leds_D(calcul_day());
}


void control_humidity(){
  
  if(humidy < 45){
    digitalWrite(RELAIS_BRIS, HIGH);
  }

  else if(humidy > 55){
    digitalWrite(RELAIS_BRIS, LOW);
  }  
}


void control_leds_T(){
  
  if(tempe < 36.1){
    digitalWrite(RED_LED_T, HIGH);
    digitalWrite(GREEN_LED_T, LOW);
  }

  else if((tempe > 36) && (tempe < 38.9)){
    digitalWrite(RED_LED_T, LOW);
    digitalWrite(GREEN_LED_T, HIGH);
  }

  else{
    digitalWrite(RED_LED_T, HIGH);
    digitalWrite(GREEN_LED_T, LOW);
  }
}


void control_leds_D(byte day){
  if (day > 18){
    digitalWrite(RED_LED_D, HIGH);
    digitalWrite(GREEN_LED_D, LOW);
  }

  else{
    digitalWrite(RED_LED_D, LOW);
    digitalWrite(GREEN_LED_D, HIGH);
  }
  
}


void control_buzzer(){
 
  if((tempe >= 39.5) || (humidy < 35)){
   
     if(( millis() - temp_b2) >= 500){
     temp_b2 = millis(); 
     
     digitalWrite(BUZZER, etat_buz);
     etat_buz = 0;
      }
    }
  else 
    digitalWrite(BUZZER, LOW);
}


void lecture_bt2(){
  bool valeur_lis;
  valeur_lis= digitalRead(BT_2);
  
  if((valeur_lis== HIGH) && (memo_2 != valeur_lis)){
      result_bt(BT_2, 1);
     }
  memo_2 = valeur_lis;
}


void lecture_bt19(){
  bool valeur_lis;
  valeur_lis= digitalRead(BT_19);
  
  if((valeur_lis== HIGH) && (memo_19 != valeur_lis)){
      result_bt(BT_19, 1);
      EEPROM.write(RETOURNEMENT, flag_retournement);
     }
  memo_19 = valeur_lis;
}


void lecture_bt18(){

    unsigned long temp_button_init = millis();
    bool sortie =1; 
    
  while(digitalRead(BT_18) && sortie ){
    
    if(millis()-temp_button_init >= 1500){
      sortie= 0;
     }   
            result_bt(BT_18, sortie);
   }
}


void result_bt(char numeroBt, bool confirmation){
  switch(numeroBt){
    case 2:
        isrDay();
    break;

    case 19:
        isrRetournement();
    break;

    case 18:
    if(!confirmation){
            isrConfirmation();      
    }
    
    break;
  }
}
