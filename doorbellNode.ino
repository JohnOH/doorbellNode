#include <Tone.h> 
#include <rtttl.h>

#include <JeeLib.h>
#include <EEPROM.h>
#include "avr/pgmspace.h"

#define DEBUG 1
#define BAND RF12_868MHZ    // wireless frequency band
#define GROUP 5             // wireless net group
#define NODEID 18           // node id on wireless to which this sketch responds
#define buttonPin   5   

const int pinSpeaker = 7;
const int octave = 0;

int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers



const char song_0[]  ="GirlFromIpane:d=4,o=5,b=160:g.,8e,8e,d,g.,8e,e,8e,8d,g.,e,e,8d,g,8g,8e,e,8e,8d,f,d,d,8d,8c,e,c,c,8c,a#4,2c";
const char song_1[]  ="The Simpsons:d=4,o=5,b=160:c.6,e6,f#6,8a6,g.6,e6,c6,8a,8f#,8f#,8f#,2g,8p,8p,8f#,8f#,8f#,8g,a#.,8c6,8c6,8c6,c6";
const char song_2[]  ="Indiana:d=4,o=5,b=250:e,8p,8f,8g,8p,1c6,8p.,d,8p,8e,1f,p.,g,8p,8a,8b,8p,1f6,p,a,8p,8b,2c6,2d6,2e6,e,8p,8f,8g,8p,1c6,p,d6,8p,8e6,1f.6,g,8p,8g,e.6,8p,d6,8p,8g,e.6,8p,d6,8p,8g,f.6,8p,e6,8p,8d6,2c6";
const char song_3[]  ="TakeOnMe:d=4,o=4,b=160:8f#5,8f#5,8f#5,8d5,8p,8b,8p,8e5,8p,8e5,8p,8e5,8g#5,8g#5,8a5,8b5,8a5,8a5,8a5,8e5,8p,8d5,8p,8f#5,8p,8f#5,8p,8f#5,8e5,8e5,8f#5,8e5,8f#5,8f#5,8f#5,8d5,8p,8b,8p,8e5,8p,8e5,8p,8e5,8g#5,8g#5,8a5,8b5,8a5,8a5,8a5,8e5,8p,8d5,8p,8f#5,8p,8f#5,8p,8f#5,8e5,8e5";
const char song_4[]  ="Entertainer:d=4,o=5,b=140:8d,8d#,8e,c6,8e,c6,8e,2c.6,8c6,8d6,8d#6,8e6,8c6,8d6,e6,8b,d6,2c6,p,8d,8d#,8e,c6,8e,c6,8e,2c.6,8p,8a,8g,8f#,8a,8c6,e6,8d6,8c6,8a,2d6";
const char song_5[]  ="Muppets:d=4,o=5,b=250:c6,c6,a,b,8a,b,g,p,c6,c6,a,8b,8a,8p,g.,p,e,e,g,f,8e,f,8c6,8c,8d,e,8e,8e,8p,8e,g,2p,c6,c6,a,b,8a,b,g,p,c6,c6,a,8b,a,g.,p,e,e,g,f,8e,f,8c6,8c,8d,e,8e,d,8d,c";
const char song_6[]  ="Xfiles:d=4,o=5,b=125:e,b,a,b,d6,2b.,1p,e,b,a,b,e6,2b.,1p,g6,f#6,e6,d6,e6,2b.,1p,g6,f#6,e6,d6,f#6,2b.,1p,e,b,a,b,d6,2b.,1p,e,b,a,b,e6,2b.,1p,e6,2b.";
const char song_7[]  ="Looney:d=4,o=5,b=140:32p,c6,8f6,8e6,8d6,8c6,a.,8c6,8f6,8e6,8d6,8d#6,e.6,8e6,8e6,8c6,8d6,8c6,8e6,8c6,8d6,8a,8c6,8g,8a#,8a,8f";
const char song_8[]  ="20thCenFox:d=16,o=5,b=140:b,8p,b,b,2b,p,c6,32p,b,32p,c6,32p,b,32p,c6,32p,b,8p,b,b,b,32p,b,32p,b,32p,b,32p,b,32p,b,32p,b,32p,g#,32p,a,32p,b,8p,b,b,2b,4p,8e,8g#,8b,1c#6,8f#,8a,8c#6,1e6,8a,8c#6,8e6,1e6,8b,8g#,8a,2b";
const char song_9[]  ="Bond:d=4,o=5,b=80:32p,16c#6,32d#6,32d#6,16d#6,8d#6,16c#6,16c#6,16c#6,16c#6,32e6,32e6,16e6,8e6,16d#6,16d#6,16d#6,16c#6,32d#6,32d#6,16d#6,8d#6,16c#6,16c#6,16c#6,16c#6,32e6,32e6,16e6,8e6,16d#6,16d6,16c#6,16c#7,c.7,16g#6,16f#6,g#.6";
const char song_10[]  ="MASH:d=8,o=5,b=140:4a,4g,f#,g,p,f#,p,g,p,f#,p,2e.,p,f#,e,4f#,e,f#,p,e,p,4d.,p,f#,4e,d,e,p,d,p,e,p,d,p,2c#.,p,d,c#,4d,c#,d,p,e,p,4f#,p,a,p,4b,a,b,p,a,p,b,p,2a.,4p,a,b,a,4b,a,b,p,2a.,a,4f#,a,b,p,d6,p,4e.6,d6,b,p,a,p,2b";

const char song_200[]  = "smbdeath:d=4,o=5,b=90:32c6,32c6,32c6,8p";

char *songs[] = {song_1, song_2, song_3};

Rtttl player;


struct {
  byte ringtone; 
  byte ring; 
} payload;




void setup () {
  Serial.begin(57600); 
  Serial.println("\n[doorbelNode]");

  player.begin(pinSpeaker);
  
 // digitalWrite(buttonPin, 1); // pull-up
  pinMode(buttonPin, INPUT);
    
  loadSettings();
  rf12_initialize(NODEID, BAND, GROUP);
}

static void doReport(){
        #if DEBUG
           Serial.println(" doReport ");
        #endif
        
      rf12_sendStart(0, &payload, sizeof payload);  
}

static void doTrigger() {
    #if DEBUG
        Serial.println(" doTrigger ");  
        Serial.println(payload.ringtone);         
    #endif
    
    doReport();
 
    player.play_P(songs[payload.ringtone], octave); 
}

static void loadSettings() {
    payload.ringtone = (EEPROM.read(0x00));
    payload.ring     = (EEPROM.read(0x01));
    
    #if DEBUG
        Serial.println(" loadSettings ");  
        Serial.print(" RingTone ");
        Serial.println(payload.ringtone);    
        Serial.print(" Rings ");
        Serial.println(payload.ring);    
    #endif
}

void recvSettings(){
    if(rf12_recvDone()  && rf12_hdr == (RF12_HDR_DST | NODEID) && rf12_crc == 0) {       
      if(rf12_len == 2) {
        
        #if DEBUG
           Serial.println(" packetReceived ");
        #endif
                
         EEPROM.write(0x00, (rf12_data[0]));         
         EEPROM.write(0x01, (rf12_data[1])); 
         
      loadSettings();     
      player.play_P(song_200, octave); 
      }       
    }         
}

void loop () {
  
   recvSettings();
   
 
  int reading = digitalRead(buttonPin);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  } 
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      if (reading == HIGH) {
        
        doTrigger();
        
      }
    }
  }
  lastButtonState = reading;
}
