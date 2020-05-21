/*
 * ____                     _      ______ _____    _____
  / __ \                   | |    |  ____|  __ \  |  __ \
 | |  | |_ __   ___ _ __   | |    | |__  | |  | | | |__) |__ _  ___ ___
 | |  | | '_ \ / _ \ '_ \  | |    |  __| | |  | | |  _  // _` |/ __/ _ \
 | |__| | |_) |  __/ | | | | |____| |____| |__| | | | \ \ (_| | (_|  __/
  \____/| .__/ \___|_| |_| |______|______|_____/  |_|  \_\__,_|\___\___|
        | |
        |_|
 Open LED Race Project 
 An minimalist game race for LED strip
 
 Experimental version phisics dinamic based on Wolfram Cellular Automata 1D 

 https://writings.stephenwolfram.com/2019/10/announcing-the-rule-30-prizes/
 
 NOT PLAYABLE, it's just a concept test

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 3 of the License, or
 (at your option) any later version.

 by gbarbarov@singulardevices.com   
 https://github.com/gbarbarov/led-race 
*/

#include <Adafruit_NeoPixel.h>
#define PIN_LED        2
#define PIN_PLAYER1    A2
#define PIN_PLAYER2    A0
#define NUMPIXELS      300
#define R_LEVEL        10
#define G_LEVEL        10
#define B_LEVEL        0

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN_LED, NEO_GRB + NEO_KHZ800);

struct flags {
      unsigned WRed:1;
      unsigned WGreen:1;
      unsigned SRed:1;
      unsigned SGreen:1;      
} __attribute__((packed));

struct flags WAC[NUMPIXELS];

boolean  rule[] = {0,0,0,0,1,0,0,0}; // Rule 16 AC Wolfram 1D / OPEN LED RACE LEFT !!!!
                  
int dt = 20; // delay 

boolean run_rule (boolean cell_L, boolean cell, boolean cell_R) {
return(rule[cell_L<<2 | cell<<1 | cell_R]); 
}

void tick_R(){
  for (int n=1;n<NUMPIXELS-1;n++)
  {WAC[n].WRed=run_rule(WAC[n-1].SRed,WAC[n].SRed,WAC[n+1].SRed);   
  }    
  WAC[0].WRed=run_rule(WAC[NUMPIXELS-1].SRed,WAC[0].SRed,WAC[1].SRed);  
  WAC[NUMPIXELS-1].WRed=run_rule(WAC[NUMPIXELS-2].SRed,WAC[NUMPIXELS-1].SRed,WAC[0].SRed);   
}

void tick_G(){
  for (int n=1;n<NUMPIXELS-1;n++)
  {WAC[n].WGreen=run_rule(WAC[n-1].SGreen,WAC[n].SGreen,WAC[n+1].SGreen);   
  }    
  WAC[0].WGreen=run_rule(WAC[NUMPIXELS-1].SGreen,WAC[0].SGreen,WAC[1].SGreen);  
  WAC[NUMPIXELS-1].WGreen=run_rule(WAC[NUMPIXELS-2].SGreen,WAC[NUMPIXELS-1].SGreen,WAC[0].SGreen);   
}

void setup() {  
  pixels.begin();      
  pinMode( PIN_PLAYER1, INPUT_PULLUP);    //button P1 
  pinMode( PIN_PLAYER2, INPUT_PULLUP);    //button P2
  for (int n=0;n<NUMPIXELS;n++)
  {WAC[n].WRed=0;
   WAC[n].SRed=0;   
   WAC[n].WGreen=0; 
   WAC[n].SGreen=0;  
  };

  WAC[NUMPIXELS/2].WRed=1;                //seed start position
  WAC[1].WGreen=1;                        //seed start position            
}

void loop() {     
  WAC[0].WRed=!digitalRead(PIN_PLAYER1) | WAC[NUMPIXELS-1].WRed;        //push buttons for add seeds 
  WAC[0].WGreen=!digitalRead(PIN_PLAYER2) | WAC[NUMPIXELS-1].WGreen;;
  
  for(int i=0;i<NUMPIXELS;i++){   
      pixels.setPixelColor(i, pixels.Color(R_LEVEL*WAC[i].WRed,G_LEVEL*WAC[i].WGreen,0));     
      WAC[i].SRed=WAC[i].WRed;
      WAC[i].SGreen=WAC[i].WGreen;       
      }      
  pixels.show(); // Update LEDs to the hardware.    

  tick_R();
  tick_G();        
  delay(dt);
}
