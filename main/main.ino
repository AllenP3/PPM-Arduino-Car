#include "omni_edit.h"

void setup();
void loop()
{ 
   readPPMData();
  mobilize();
  //stop_Stop();

 for (int i = 0; i < 6; i++)
  {
    Serial.print(" " + String(ch[i]) + " "); 
  }
  Serial.println();
  /*if(rChar == 'w'){
      Serial.println("got w");

    }

    else if(rChar == 's'){
      Serial.println("got s");

}*/
}