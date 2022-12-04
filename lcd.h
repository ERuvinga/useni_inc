
// FICHIER POUR AFFICHAGE DES DONNEES.
#ifndef lcd
#define lcd
#include "memory.h"

char tableau[2];
//int compte = 5;
unsigned long temp_compt = millis();

void compteur(){

  if((millis()-temp_compt) >=1000){
    if(flag_lcd == 2){
      compte--;
    }
    temp_compt= millis();
  }
  
  if(compte < 0) {
    flag_save = 1;
    flag_lcd = 0;
    flag_lcd_clear = 1;
    compte=5;
  }
}

void affichage(){
  
  sprintf(tableau,"%cC ", char(0b11011111));
  lcd_tempe.print("");
  lcd_tempe.setCursor(4, 0);
  lcd_tempe.print("T");
  lcd_tempe.print(char(0b11011111));
  lcd_tempe.setCursor(15, 0);
  lcd_tempe.print("H");
    
  lcd_tempe.setCursor(2, 1);
  lcd_tempe.print(tempe, 1);
  lcd_tempe.print(tableau);
  
  lcd_tempe.setCursor(13, 1);
  lcd_tempe.print(humidy, 1);
  lcd_tempe.print('%');

//affichage du temps et de la date   
  
  lcd_tempe.setCursor(5, 2);
  if (dateTime.hours < 10)
  lcd_tempe.print("0");
  
  lcd_tempe.print(dateTime.hours);
  lcd_tempe.print(":");
 
  if (dateTime.mins < 10){
  lcd_tempe.print("0");
  }
  lcd_tempe.print(dateTime.mins);  
  lcd_tempe.print("'");
  lcd_tempe.print(":");

  if (dateTime.sec < 10){
  lcd_tempe.print("0");
  }
  lcd_tempe.print(dateTime.sec);
  lcd_tempe.print('"');
    
  lcd_tempe.setCursor(2, 3);
  lcd_tempe.print(decode_dow(dateTime.dow));
  lcd_tempe.print(" ");
  if (dateTime.dom < 10)
  lcd_tempe.print("0");
  
  lcd_tempe.print(dateTime.dom);
  lcd_tempe.print(" ");
  lcd_tempe.print(decode_month(dateTime.month));
  lcd_tempe.print(" ");
  lcd_tempe.print(dateTime.year + 2000);
}


void initialisation()
{
  int j,i;
  
 for(i =0; i<2; i++){
  lcd_tempe.clear();
  lcd_tempe.setCursor(3, 2);
  lcd_tempe.print("chargement");
      
      delay(250);    
      for(j=0; j<=2; j++){
        lcd_tempe.print(".");
        delay(250);    
      }
    }   
        lcd_tempe.clear();
}


void displayRetournement()
{
  int j,i;
  
 for(i =0; i< 10; i++){
  lcd_tempe.clear();
  lcd_tempe.setCursor(2, 2);
  lcd_tempe.print("Retournement.");
      
      delay(1000);    
      for(j=0; j<=3; j++){
        lcd_tempe.print(".");
        delay(500);    
      }
    }   
    lcd_tempe.clear();
}

void day(){
  lcd_tempe.setCursor(1, 0);
  lcd_tempe.print("JOURS D'INCUBATION");
  
  lcd_tempe.setCursor(4, 2);
  lcd_tempe.print(EEPROM.read(ADRESS_DAY));
  lcd_tempe.print(" ");
  lcd_tempe.print(decode_month(memoryRead(ADRESS_MONTH)));
  lcd_tempe.print(" ");
  lcd_tempe.print(EEPROM.read(ADRESS_YEAR)+2000);
 
  lcd_tempe.setCursor(4, 3);
  lcd_tempe.print("Jour  ");
  lcd_tempe.print(calcul_day());  
}

void question(){
  
  lcd_tempe.setCursor(4, 1);
  lcd_tempe.print("Initialisation ");
  
  lcd_tempe.setCursor(6, 3);
  lcd_tempe.print("dans (");
  lcd_tempe.print(compte);
  lcd_tempe.print(')');
}

void menu(char choi){
    
  switch(choi){
    case 0:
         affichage(); 
    break;

    case 1:
        day();
   break;

    case 2:
      question();
  break;  

  }
  
}

void lcd_clear(bool yes_no){
  if(yes_no){
    lcd_tempe.clear();
    flag_lcd_clear =0;
  }
}

#endif //lcd
