#ifndef memo
#define memo

#include <EEPROM.h>
#include "prototypes.h"
int compte = 5;
LiquidCrystal_I2C lcd_tempe(0x27, 20, 4);

#define ADRESS_DAY 2   //adresse eeprom du premier jour d'incubation  
#define ADRESS_MONTH 3  //adresse eeprom du moi d'incubation

#define EEPROM_DAY 4  //adresse eeprom du jour actuel
#define NUMBER_DAY 5  //adress eeprom des nombres des jours ecoules depuis le 1ier jour
#define ADRESS_YEAR 6

char flag_lcd = 0;
bool flag_lcd_clear = 0;
bool flag_save=0;          // drapeau pour la confimation de mise a jour de la date d'incubation en memoire
bool flag_retournement = 1; //drapeau pour la confirmation du retournemt
bool valider = 0;


 void isrMemory();
 void isrConfirmation();
 void isrRetournement();
 void isrDay();

  //les fonctions pour l'EEPROM
char memoryRead(char);
void memorySave();


byte calcul_day(){

  int nday = EEPROM.read(NUMBER_DAY);
 
  if(nday > 22){          
        EEPROM.write(NUMBER_DAY, 0);
        }
        
  if (EEPROM.read(EEPROM_DAY) !=  dateTime.dom){
        
        EEPROM.write(NUMBER_DAY, nday + 1);
        EEPROM.write(EEPROM_DAY, dateTime.dom); 
  }

  return EEPROM.read(NUMBER_DAY);
}


void memorySave(bool* save) {

  if (*save) {

    //initialisation des jours
    EEPROM.write(ADRESS_DAY, dateTime.dom);
    EEPROM.write(ADRESS_MONTH, dateTime.month);
    EEPROM.write(ADRESS_YEAR, dateTime.year);

    //initialisation  du compteur des jours
    EEPROM.write(NUMBER_DAY, 0);
    EEPROM.write(EEPROM_DAY, dateTime.dom);
    *save = 0;

    initialisation();
  }
}

char memoryRead(char adress) {
  return EEPROM.read(adress);
}


//les fonctions pour interruptions de sauvegarde


void isrConfirmation() {
    flag_lcd = 2;
    compte= 5;
    flag_lcd_clear = 1;
}

 void isrDay(){
  
  flag_lcd++;
  flag_lcd_clear = 1;
  
  if(flag_lcd >1)
    flag_lcd = 0;
 }

 void isrRetournement(){
   flag_retournement = !flag_retournement;
   valider = 1;
   flag_lcd_clear = 1;
  }

#endif
