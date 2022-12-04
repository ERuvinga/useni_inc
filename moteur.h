#ifndef moteur
#define moteur

#define PIN_MOTEUR_a 36
#define PIN_MOTEUR_d 38
#define RETOURNEMENT 8
  //ce fichier contient les fonction permettant le retournement des oeufs grace au moteur
int temp_retour = 1;
int heure_actu;

void control_hour(){
  if(dateTime.hours != heure_actu){
   heure_actu = dateTime.hours;
    flag_retournement = !flag_retournement;
    EEPROM.write(RETOURNEMENT, flag_retournement);
    valider = 1;
  }
}


void retournement(){
control_hour(); //on verifie l'heure enfin d'activer les flag pour le retournement si 1heure est passee
  
  if(valider){
      if(flag_retournement){
        digitalWrite(PIN_MOTEUR_a, LOW);
        digitalWrite(PIN_MOTEUR_d, HIGH); 
      }
      
       else{
        digitalWrite(PIN_MOTEUR_d, LOW);
        digitalWrite(PIN_MOTEUR_a, HIGH);
      }  
      displayRetournement();
    }
 

  digitalWrite(PIN_MOTEUR_d, LOW);
  digitalWrite(PIN_MOTEUR_a, LOW);
  valider = 0;
  }
 
#endif //moteur
