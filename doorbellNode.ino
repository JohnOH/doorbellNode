// #include <Tone.h> 
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


char *songs[] = {
  "song_0",
  "song_1", 
  "song_2",
  "song_3",
  "song_4",
  "song_5",
  "song_6",
  "song_7",
  "song_8",
  "song_9", 
  "song_10",
  "song_11",
  "song_12",
  "song_13",
  "song_14",
  "song_15",
  "song_16",
  "song_17", 
  "song_18",
  "song_19",
  "song_20",
  "song_21",
  "song_22",
  "song_23",
  "song_24",
  "song_25", 
  "song_26",
  "song_27",
  "song_28",
  "song_29",
  "song_30",
  "song_31",
  "song_32",
  "song_33", 
  "song_34",
  "song_35",
  "song_36",
  "song_37",
  "song_38",
  "song_39",
  "song_40",  
  "song_41", 
  "song_42",
  "song_43",
  "song_44",
  "song_45",
  "song_46",
  "song_47",
  "song_48",
  "song_49", 
  "song_50",
  "song_51",
  "song_52",
  "song_53",
  "song_54",
  "song_55",
  "song_56",
  "song_57", 
  "song_58",
  "song_59",
  "song_60",
  "song_61",
  "song_62",
  "song_63",
  "song_64",
  "song_65", 
  "song_66",
  "song_67",
  "song_68",
  "song_69",
  "song_70",
  "song_71",
  "song_72",
  "song_73", 
  "song_74",
  "song_75",
  "song_76",
  "song_77",
  "song_78",
  "song_79",
  "song_80",
  "song_81", 
  "song_82",
  "song_83",
  "song_84",
  "song_85",
  "song_86",
  "song_87",
  "song_88",
  "song_89", 
  "song_90",
  "song_91",
  "song_92",
  "song_93",
  "song_94",
  "song_95",
  "song_96",
  "song_97", 
  "song_98",
  "song_99",
  "song_100",
  "song_101",
  "song_102",
  "song_103",
  "song_104", 
  "song_105", 
  "song_106",
  "song_107",
  "song_108",
  "song_109",
  "song_110",
  "song_111",
  "song_112",
  "song_113", 
  "song_114",
  "song_115",
  "song_116",
  "song_117",
  "song_118",
  "song_119",
  "song_120",
  "song_121",
  "song_122",
  "song_123",
  "song_200",
};

const char song_0[] PROGMEM ="GirlFromIpane:d=4,o=5,b=160:g.,8e,8e,d,g.,8e,e,8e,8d,g.,e,e,8d,g,8g,8e,e,8e,8d,f,d,d,8d,8c,e,c,c,8c,a#4,2c";
const char song_1[] PROGMEM ="The Simpsons:d=4,o=5,b=160:c.6,e6,f#6,8a6,g.6,e6,c6,8a,8f#,8f#,8f#,2g,8p,8p,8f#,8f#,8f#,8g,a#.,8c6,8c6,8c6,c6";
const char song_2[] PROGMEM ="Indiana:d=4,o=5,b=250:e,8p,8f,8g,8p,1c6,8p.,d,8p,8e,1f,p.,g,8p,8a,8b,8p,1f6,p,a,8p,8b,2c6,2d6,2e6,e,8p,8f,8g,8p,1c6,p,d6,8p,8e6,1f.6,g,8p,8g,e.6,8p,d6,8p,8g,e.6,8p,d6,8p,8g,f.6,8p,e6,8p,8d6,2c6";
const char song_3[] PROGMEM ="TakeOnMe:d=4,o=4,b=160:8f#5,8f#5,8f#5,8d5,8p,8b,8p,8e5,8p,8e5,8p,8e5,8g#5,8g#5,8a5,8b5,8a5,8a5,8a5,8e5,8p,8d5,8p,8f#5,8p,8f#5,8p,8f#5,8e5,8e5,8f#5,8e5,8f#5,8f#5,8f#5,8d5,8p,8b,8p,8e5,8p,8e5,8p,8e5,8g#5,8g#5,8a5,8b5,8a5,8a5,8a5,8e5,8p,8d5,8p,8f#5,8p,8f#5,8p,8f#5,8e5,8e5";
const char song_4[] PROGMEM ="Entertainer:d=4,o=5,b=140:8d,8d#,8e,c6,8e,c6,8e,2c.6,8c6,8d6,8d#6,8e6,8c6,8d6,e6,8b,d6,2c6,p,8d,8d#,8e,c6,8e,c6,8e,2c.6,8p,8a,8g,8f#,8a,8c6,e6,8d6,8c6,8a,2d6";
const char song_5[] PROGMEM ="Muppets:d=4,o=5,b=250:c6,c6,a,b,8a,b,g,p,c6,c6,a,8b,8a,8p,g.,p,e,e,g,f,8e,f,8c6,8c,8d,e,8e,8e,8p,8e,g,2p,c6,c6,a,b,8a,b,g,p,c6,c6,a,8b,a,g.,p,e,e,g,f,8e,f,8c6,8c,8d,e,8e,d,8d,c";
const char song_6[] PROGMEM ="Xfiles:d=4,o=5,b=125:e,b,a,b,d6,2b.,1p,e,b,a,b,e6,2b.,1p,g6,f#6,e6,d6,e6,2b.,1p,g6,f#6,e6,d6,f#6,2b.,1p,e,b,a,b,d6,2b.,1p,e,b,a,b,e6,2b.,1p,e6,2b.";
const char song_7[] PROGMEM ="Looney:d=4,o=5,b=140:32p,c6,8f6,8e6,8d6,8c6,a.,8c6,8f6,8e6,8d6,8d#6,e.6,8e6,8e6,8c6,8d6,8c6,8e6,8c6,8d6,8a,8c6,8g,8a#,8a,8f";
const char song_8[] PROGMEM ="20thCenFox:d=16,o=5,b=140:b,8p,b,b,2b,p,c6,32p,b,32p,c6,32p,b,32p,c6,32p,b,8p,b,b,b,32p,b,32p,b,32p,b,32p,b,32p,b,32p,b,32p,g#,32p,a,32p,b,8p,b,b,2b,4p,8e,8g#,8b,1c#6,8f#,8a,8c#6,1e6,8a,8c#6,8e6,1e6,8b,8g#,8a,2b";
const char song_9[] PROGMEM ="Bond:d=4,o=5,b=80:32p,16c#6,32d#6,32d#6,16d#6,8d#6,16c#6,16c#6,16c#6,16c#6,32e6,32e6,16e6,8e6,16d#6,16d#6,16d#6,16c#6,32d#6,32d#6,16d#6,8d#6,16c#6,16c#6,16c#6,16c#6,32e6,32e6,16e6,8e6,16d#6,16d6,16c#6,16c#7,c.7,16g#6,16f#6,g#.6";
const char song_10[] PROGMEM ="MASH:d=8,o=5,b=140:4a,4g,f#,g,p,f#,p,g,p,f#,p,2e.,p,f#,e,4f#,e,f#,p,e,p,4d.,p,f#,4e,d,e,p,d,p,e,p,d,p,2c#.,p,d,c#,4d,c#,d,p,e,p,4f#,p,a,p,4b,a,b,p,a,p,b,p,2a.,4p,a,b,a,4b,a,b,p,2a.,a,4f#,a,b,p,d6,p,4e.6,d6,b,p,a,p,2b";
const char song_11[] PROGMEM ="StarWars:d=4,o=5,b=45:32p,32f#,32f#,32f#,8b.,8f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32e6,8c#.6,32f#,32f#,32f#,8b.,8f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32e6,8c#6";
const char song_12[] PROGMEM ="GoodBad:d=4,o=5,b=56:32p,32a#,32d#6,32a#,32d#6,8a#.,16f#.,16g#.,d#,32a#,32d#6,32a#,32d#6,8a#.,16f#.,16g#.,c#6,32a#,32d#6,32a#,32d#6,8a#.,16f#.,32f.,32d#.,c#,32a#,32d#6,32a#,32d#6,8a#.,16g#.,d#";
const char song_13[] PROGMEM ="TopGun:d=4,o=4,b=31:32p,16c#,16g#,16g#,32f#,32f,32f#,32f,16d#,16d#,32c#,32d#,16f,32d#,32f,16f#,32f,32c#,16f,d#,16c#,16g#,16g#,32f#,32f,32f#,32f,16d#,16d#,32c#,32d#,16f,32d#,32f,16f#,32f,32c#,g#";
const char song_14[] PROGMEM ="A-Team:d=8,o=5,b=125:4d#6,a#,2d#6,16p,g#,4a#,4d#.,p,16g,16a#,d#6,a#,f6,2d#6,16p,c#.6,16c6,16a#,g#.,2a#";
const char song_15[] PROGMEM ="Flinstones:d=4,o=5,b=40:32p,16f6,16a#,16a#6,32g6,16f6,16a#.,16f6,32d#6,32d6,32d6,32d#6,32f6,16a#,16c6,d6,16f6,16a#.,16a#6,32g6,16f6,16a#.,32f6,32f6,32d#6,32d6,32d6,32d#6,32f6,16a#,16c6,a#,16a6,16d.6,16a#6,32a6,32a6,32g6,32f#6,32a6,8g6,16g6,16c.6,32a6,32a6,32g6,32g6,32f6,32e6,32g6,8f6,16f6,16a#.,16a#6,32g6,16f6,16a#.,16f6,32d#6,32d6,32d6,32d#6,32f6,16a#,16c.6,32d6,32d#6,32f6,16a#,16c.6,32d6,32d#6,32f6,16a#6,16c7,8a#.6";
const char song_16[] PROGMEM ="Jeopardy:d=4,o=6,b=125:c,f,c,f5,c,f,2c,c,f,c,f,a.,8g,8f,8e,8d,8c#,c,f,c,f5,c,f,2c,f.,8d,c,a#5,a5,g5,f5,p,d#,g#,d#,g#5,d#,g#,2d#,d#,g#,d#,g#,c.7,8a#,8g#,8g,8f,8e,d#,g#,d#,g#5,d#,g#,2d#,g#.,8f,d#,c#,c,p,a#5,p,g#.5,d#,g#";
const char song_17[] PROGMEM ="Gadget:d=16,o=5,b=50:32d#,32f,32f#,32g#,a#,f#,a,f,g#,f#,32d#,32f,32f#,32g#,a#,d#6,4d6,32d#,32f,32f#,32g#,a#,f#,a,f,g#,f#,8d#";
const char song_18[] PROGMEM ="Smurfs:d=32,o=5,b=200:4c#6,16p,4f#6,p,16c#6,p,8d#6,p,8b,p,4g#,16p,4c#6,p,16a#,p,8f#,p,8a#,p,4g#,4p,g#,p,a#,p,b,p,c6,p,4c#6,16p,4f#6,p,16c#6,p,8d#6,p,8b,p,4g#,16p,4c#6,p,16a#,p,8b,p,8f,p,4f#";
const char song_19[] PROGMEM ="MahnaMahna:d=16,o=6,b=125:c#,c.,b5,8a#.5,8f.,4g#,a#,g.,4d#,8p,c#,c.,b5,8a#.5,8f.,g#.,8a#.,4g,8p,c#,c.,b5,8a#.5,8f.,4g#,f,g.,8d#.,f,g.,8d#.,f,8g,8d#.,f,8g,d#,8c,a#5,8d#.,8d#.,4d#,8d#.";
const char song_20[] PROGMEM ="LeisureSuit:d=16,o=6,b=56:f.5,f#.5,g.5,g#5,32a#5,f5,g#.5,a#.5,32f5,g#5,32a#5,g#5,8c#.,a#5,32c#,a5,a#.5,c#.,32a5,a#5,32c#,d#,8e,c#.,f.,f.,f.,f.,f,32e,d#,8d,a#.5,e,32f,e,32f,c#,d#.,c#";
const char song_21[] PROGMEM ="MissionImp:d=16,o=6,b=95:32d,32d#,32d,32d#,32d,32d#,32d,32d#,32d,32d,32d#,32e,32f,32f#,32g,g,8p,g,8p,a#,p,c7,p,g,8p,g,8p,f,p,f#,p,g,8p,g,8p,a#,p,c7,p,g,8p,g,8p,f,p,f#,p,a#,g,2d,32p,a#,g,2c#,32p,a#,g,2c,a#5,8c,2p,32p,a#5,g5,2f#,32p,a#5,g5,2f,32p,a#5,g5,2e,d#,8d";
const char song_22[] PROGMEM ="TakeOnMe:d=4,o=4,b=160:8f#5,8f#5,8f#5,8d5,8p,8b,8p,8e5,8p,8e5,8p,8e5,8g#5,8g#5,8a5,8b5,8a5,8a5,8a5,8e5,8p,8d5,8p,8f#5,8p,8f#5,8p,8f#5,8e5,8e5,8f#5,8e5,8f#5,8f#5,8f#5,8d5,8p,8b,8p,8e5,8p,8e5,8p,8e5,8g#5,8g#5,8a5,8b5,8a5,8a5,8a5,8e5,8p,8d5,8p,8f#5,8p,8f#5,8p,8f#5,8e5,8e5";
const char song_23[] PROGMEM ="90210:d=4,o=5,b=140:8f,8a#,8c6,d.6,2d6,p,8f,8a#,8c6,8d6,8d#6,f6,f.6,2a#.,8f,8a#,8c6,8d6,8d#6,8f6,8g6,f6,8d#6,d#6,d6,2c.6,8a#,a,a#.,g6,8f6,8d#6,8d6,8d#6,8d6,8a#,f";
const char song_24[] PROGMEM ="Abdelazer:d=4,o=5,b=160:2d,2f,2a,d6,8e6,8f6,8g6,8f6,8e6,8d6,2c#6,a6,8d6,8f6,8a6,8f6,d6,2a6,g6,8c6,8e6,8g6,8e6,c6,2a6,f6,8b,8d6,8f6,8d6,b,2g6,e6,8a,8c#6,8e6,8c6,a,2f6,8e6,8f6,8e6,8d6,c#6,f6,8e6,8f6,8e6,8d6,a,d6,8c#6,8d6,8e6,8d6,2d6";
const char song_25[] PROGMEM ="aadams:d=4,o=5,b=160:8c,f,8a,f,8c,b4,2g,8f,e,8g,e,8e4,a4,2f,8c,f,8a,f,8c,b4,2g,8f,e,8c,d,8e,1f,8c,8d,8e,8f,1p,8d,8e,8f#,8g,1p,8d,8e,8f#,8g,p,8d,8e,8f#,8g,p,8c,8d,8e,8f";
const char song_26[] PROGMEM ="Smoke:d=4,o=5,b=112:c,d#,f.,c,d#,8f#,f,p,c,d#,f.,d#,c,2p,8p,c,d#,f.,c,d#,8f#,f,p,c,d#,f.,d#,c,p";
const char song_27[] PROGMEM ="smb:d=4,o=5,b=100:16e6,16e6,32p,8e6,16c6,8e6,8g6,8p,8g,8p,8c6,16p,8g,16p,8e,16p,8a,8b,16a#,8a,16g.,16e6,16g6,8a6,16f6,8g6,8e6,16c6,16d6,8b,16p,8c6,16p,8g,16p,8e,16p,8a,8b,16a#,8a,16g.,16e6,16g6,8a6,16f6,8g6,8e6,16c6,16d6,8b,8p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16g#,16a,16c6,16p,16a,16c6,16d6,8p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16c7,16p,16c7,16c7,p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16g#,16a,16c6,16p,16a,16c6,16d6,8p,16d#6,8p,16d6,8p,16c6";
const char song_28[] PROGMEM ="smb_under:d=4,o=6,b=100:32c,32p,32c7,32p,32a5,32p,32a,32p,32a#5,32p,32a#,2p,32c,32p,32c7,32p,32a5,32p,32a,32p,32a#5,32p,32a#,2p,32f5,32p,32f,32p,32d5,32p,32d,32p,32d#5,32p,32d#,2p,32f5,32p,32f,32p,32d5,32p,32d,32p,32d#5,32p,32d#";
const char song_29[] PROGMEM ="smbdeath:d=4,o=5,b=90:32c6,32c6,32c6,8p,16b,16f6,16p,16f6,16f.6,16e.6,16d6,16c6,16p,16e,16p,16c";
const char song_30[] PROGMEM ="ducktales:d=4,o=5,b=112:8e6,8e6,16p,16g6,8b6,g#6,p,8e6,8d6,8c6,8d6,8e6,8d6,8c6,8d6,8e6,8e6,16p,16g6,8b6,g#6,p,8e6,8d6,8c6,8d6,8e6,8d6,8c6,8g6,8e6,8e6";
const char song_31[] PROGMEM ="Zelda1:d=4,o=5,b=125:a#,f.,8a#,16a#,16c6,16d6,16d#6,2f6,8p,8f6,16f.6,16f#6,16g#.6,2a#.6,16a#.6,16g#6,16f#.6,8g#.6,16f#.6,2f6,f6,8d#6,16d#6,16f6,2f#6,8f6,8d#6,8c#6,16c#6,16d#6,2f6,8d#6,8c#6,8c6,16c6,16d6,2e6,g6,8f6,16f,16f,8f,16f,16f,8f,16f,16f,8f,8f,a#,f.,8a#,16a#,16c6,16d6,16d#6,2f6,8p,8f6,16f.6,16f#6,16g#.6,2a#.6,c#7,c7,2a6,f6,2f#.6,a#6,a6,2f6,f6,2f#.6,a#6,a6,2f6,d6,2d#.6,f#6,f6,2c#6,a#,c6,16d6,2e6,g6,8f6,16f,16f,8f,16f,16f,8f,16f,16f,8f,8f";
const char song_32[] PROGMEM ="smario2:d=4,o=5,b=125:8g,16c,8e,8g.,16c,8e,16g,16c,16e,16g,8b,a,8p,16c,8g,16c,8e,8g.,16c,8e,16g,16c#,16e,16g,8b,a,8p,16b,8c6,16b,8c6,8a.,16c6,8b,16a,8g,16f#,8g,8e.,16c,8d,16e,8f,16e,8f,8b.4,16e,8d.,c";
const char song_33[] PROGMEM ="smb3lvl1:d=4,o=5,b=80:16g,32c,16g.,16a,32c,16a.,16b,32c,16b,16a.,32g#,16a.,16g,32c,16g.,16a,32c,16a,4b.,32p,16c6,32f,16c.6,16d6,32f,16d.6,16e6,32f,16e6,16d.6,32c#6,16d.6,16c6,32f,16c.6,16d6,32f,16d6,4e.6,32p,16g,32c,16g.,16a,32c,16a.,16b,32c,16b,16a.,32g#,16a.,16c6,8c.6,32p,16c6,4c.6";
const char song_34[] PROGMEM ="shinobi:d=4,o=5,b=140:b,f#6,d6,b,g,f#,e,2f#.,a,1f#,p,b,f#6,d6,b,g,f#,e,1f#.,8a,1b.,8a,1f#.,8a,1b.,8a,1f#.";
const char song_35[] PROGMEM ="outrun_magic:d=4,o=5,b=160:f6,d#6,8g#.6,f6,d#6,8c#.6,d#6,c6,2g#.,c#6,c6,8d#.6,c#6,c6,8f.,a#,16c.6,1a#,f6,d#6,8g#.6,f6,d#6,8c#.6,d#6,c6,2g#.,c#6,c6,8d#.6,c#6,c6,16f.,16g#.,c6,2a#.";
const char song_36[] PROGMEM ="Popeye:d=4,o=5,b=140:16g.,16f.,16g.,16p,32p,16c.,16p,32p,16c.,16p,32p,16e.,16d.,16c.,16d.,16e.,16f.,g,8p,16a,16f,16a,16c6,16b,16a,16g,16a,16g,8e,16g,16g,16g,16g,8a,16b,32c6,32b,32c6,32b,32c6,32b,8c6";
const char song_37[] PROGMEM ="Wonderboy:d=4,o=5,b=225:f6,d6,f6,8d6,f6,32p,8f6,d6,f6,d6,e6,c6,e6,8c6,e6,32p,8e6,c6,e6,c6";
const char song_38[] PROGMEM ="smwwd1:d=4,o=5,b=125:a,8f.,16c,16d,16f,16p,f,16d,16c,16p,16f,16p,16f,16p,8c6,8a.,g,16c,a,8f.,16c,16d,16f,16p,f,16d,16c,16p,16f,16p,16a#,16a,16g,2f,16p,8a.,8f.,8c,8a.,f,16g#,16f,16c,16p,8g#.,2g,8a.,8f.,8c,8a.,f,16g#,16f,8c,2c6";
const char song_39[] PROGMEM ="dkong:d=4,o=5,b=160:2c,8d.,d#.,c.,16b,16c6,16b,16c6,16b,16c6,16b,16c6,16b,16c6,16b,16c6,16b,2c6";
const char song_40[] PROGMEM ="BarbieGirl:d=4,o=5,b=125:8g#,8e,8g#,8c#6,a,p,8f#,8d#,8f#,8b,g#,8f#,8e,p,8e,8c#,f#,c#,p,8f#,8e,g#,f#";
const char song_41[] PROGMEM ="Coca-cola:d=4,o=5,b=125:8f#6,8f#6,8f#6,8f#6,g6,8f#6,e6,8e6,8a6,f#6,d6,2p";
const char song_42[] PROGMEM ="90210:d=4,o=5,b=140:8f,8a#,8c6,d.6,2d6,p,8f,8a#,8c6,8d6,8d#6,f6,f.6,2a#.,8f,8a#,8c6,8d6,8d#6,8f6,8g6,f6,8d#6,d#6,d6,2c.6,8a#,a,a#.,g6,8f6,8d#6,8d6,8d#6,8d6,8a#,f";
const char song_43[] PROGMEM ="Abdelazer:d=4,o=5,b=160:2d,2f,2a,d6,8e6,8f6,8g6,8f6,8e6,8d6,2c#6,a6,8d6,8f6,8a6,8f6,d6,2a6,g6,8c6,8e6,8g6,8e6,c6,2a6,f6,8b,8d6,8f6,8d6,b,2g6,e6,8a,8c#6,8e6,8c6,a,2f6,8e6,8f6,8e6,8d6,c#6,f6,8e6,8f6,8e6,8d6,a,d6,8c#6,8d6,8e6,8d6,2d6";
const char song_44[] PROGMEM ="aadams:d=4,o=5,b=160:8c,f,8a,f,8c,b4,2g,8f,e,8g,e,8e4,a4,2f,8c,f,8a,f,8c,b4,2g,8f,e,8c,d,8e,1f,8c,8d,8e,8f,1p,8d,8e,8f#,8g,1p,8d,8e,8f#,8g,p,8d,8e,8f#,8g,p,8c,8d,8e,8f";
const char song_45[] PROGMEM ="aadams:d=4,o=5,b=160:8c,f,8a,f,8c,b4,2g,8f,e,8g,e,8e4,a4,2f,8c,f,8a,f,8c,b4,2g,8f,e,8c,d,8e,1f,8c,8d,8e,8f,1p,8d,8e,8f#,8g,1p,8d,8e,8f#,8g,p,8d,8e,8f#,8g,p,8c,8d,8e,8f";
const char song_46[] PROGMEM ="Agadoo:d=4,o=5,b=125:8b,8g#,e,8e,8e,e,8e,8e,8e,8e,8d#,8e,f#,8a,8f#,d#,8d#,8d#,d#,8d#,8d#,8d#,8d#,8c#,8d#,e";
const char song_47[] PROGMEM ="Argentina:d=4,o=5,b=70:8e.4,8e4,8e4,8e.4,8f4,8g4,8a4,g4,8p,8g4,8a4,8a4,8g4,c,g4,8f4,e.4,8p,8e4,8f4,8g4,8d4,d4,8d4,8e4,8f4,c4,16p,8c4,8d4,8c4,8e4,g4,16p,8g4,8g4,8a4,c,16p";
const char song_48[] PROGMEM ="Auld L S:d=4,o=5,b=100:g,c.6,8c6,c6,e6,d.6,8c6,d6,8e6,8d6,c.6,8c6,e6,g6,2a.6,a6,g.6,8e6,e6,c6,d.6,8c6,d6,8e6,8d6,c.6,8a,a,g,2c.6";
const char song_49[] PROGMEM =" :d=4,o=5,b=125:g,8a#.,16g,16p,16g,8c6,8g,8f,g,8d.6,16g,16p,16g,8d#6,8d6,8a#,8g,8d6,8g6,16g,16f,16p,16f,8d,8a#,2g,p,16f6,8d6,8c6,8a#,g,8a#.,16g,16p,16g,8c6,8g,8f,g,8d.6,16g,16p,16g,8d#6,8d6,8a#,8g,8d6,8g6,16g,16f,16p,16f,8d,8a#,2g";
const char song_50[] PROGMEM ="axelf:d=4,o=5,b=160:f#,8a.,8f#,16f#,8a#,8f#,8e,f#,8c.6,8f#,16f#,8d6,8c#6,8a,8f#,8c#6,8f#6,16f#,8e,16e,8c#,8g#,f#.";
const char song_51[] PROGMEM ="girl:d=4,o=5,b=125:8g#,8e,8g#,8c#6,a,p,8f#,8d#,8f#,8b,g#,8f#,8e,p,8e,8c#,f#,c#,p,8f#,8e,g#,f#";
const char song_52[] PROGMEM ="Black Bear:d=4,o=5,b=180:d#,d#,8g.,16d#,8a#.,16g,d#,d#,8g.,16d#,8a#.,16g,f,8c.,16b4,c,8f.,16d#,8d.,16d#,8c.,16d,8a#.4,16c,8d.,16a#4,d#,d#,8g.,16d#,8a#.,16g,d#,d#,8g.,16d#,8a#.,16g,f,f,f,8g.,16f,d#,g,2d#";
const char song_53[] PROGMEM ="Bebopalula:d=4,o=5,b=180:2p,2a,a,8a,8e,g,a,a,a,g,a,8p,8a,8a,8e,g,8a,8a,a,a,g,a";
const char song_54[] PROGMEM ="Be-Bop-A-Lula:d=4,o=5,b=180:2p,2a,a,8a,8e,g,a,a,a,g,a,8p,8a,8a,8e,g,8a,8a,a,a,g,a";
const char song_55[] PROGMEM ="Birdy S:d=4,o=5,b=100:16g,16g,16a,16a,16e,16e,8g,16g,16g,16a,16a,16e,16e,8g,16g,16g,16a,16a,16c6,16c6,8b,8b,8a,8g,8f,16f,16f,16g,16g,16d,16d,8f,16f,16f,16g,16g,16d,16d,8f,16f,16f,16g,16g,16a,16b,8c6,8a,8g,8e,c";
const char song_56[] PROGMEM ="Bogey:d=4,o=5,b=140:8g,8e,p,8p,8e,8f,8g,e6,e6,2c6,8g,8e,p,8p,8e,8f,8e,g,g,2f,8f,8d,p,8p,8d,8e,8f,8g,8e,p,8p,8e,8f#,8e,8d,8g,8p,8e,8f#,8d,8p,8a,8g.,16f#,8g,8a,8g,8f,8e,8d,8c";
const char song_57[] PROGMEM ="Bolero:d=4,o=5,b=80:c6,8c6,16b,16c6,16d6,16c6,16b,16a,8c6,16c6,16a,c6,8c6,16b,16c6,16a,16g,16e,16f,2g,16g,16f,16e,16d,16e,16f,16g,16a,g,g,16g,16a,16b,16a,16g,16f,16e,16d,16e,16d,8c,8c,16c,16d,8e,8f,d,2g";
const char song_58[] PROGMEM ="Bulletme:d=4,o=5,b=112:b.6,g.6,16f#6,16g6,16f#6,8d.6,8e6,p,16e6,16f#6,16g6,8f#.6,8g6,8a6,b.6,g.6,16f#6,16g6,16f#6,8d.6,8e6,p,16c6,16b,16a,16b";
const char song_59[] PROGMEM =" :d=4,o=5,b=80:8d,8f#,8a,8d6,8c#,8e,8a,8c#6,8d,8f#,8b,8d6,8a,8c#,8f#,8a,8b,8d,8g,8b,8a,8d,8f#,8a,8b,8f#,8g,8b,8c#,8e,8a,8c#6,f#6,8f#,8a,e6,8e,8a,d6,8f#,8a,c#6,8c#,8e,b,8d,8g,a,8f#,8d,b,8d,8g,c#.6";
const char song_60[] PROGMEM ="careaboutus:d=4,o=5,b=125:16f,16e,16f,16e,16f,16e,8d,16e,16d,16e,16d,16e,16d,16c,16d,d";
const char song_61[] PROGMEM ="Children:d=4,o=5,b=63:8p,f.6,1p,g#6,8g6,d#.6,1p,g#6,8g6,c.6,1p,g#6,8g6,g#.,1p,16f,16g,16g#,16c6,f.6,1p,g#6,8g6,d#.6,1p,16c#6,16c6,c#6,8c6,g#,2p,g.,g#,8c6,f.";
const char song_62[] PROGMEM =" :d=4,o=5,b=70:16e,16f,16g,16a,16b,16c6,16d6,16d6,16d6,c6,e6,8d6,8c6,16b,16c6,32g,32a,16e,f,f,8g,8a,8b,16c6,8b,16d6,16a,16b,16d6,16d6,16a,16b,16c6,16b,16f,16b,8a,f,e,8c6,d,8b,e,8a,8e,8f,8g,8a,8b";
const char song_63[] PROGMEM ="countdown:d=4,o=5,b=125:p,8p,16b,16a,b,e,p,8p,16c6,16b,8c6,8b,a,p,8p,16c6,16b,c6,e,p,8p,16a,16g,8a,8g,8f#,8a,g.,16f#,16g,a.,16g,16a,8b,8a,8g,8f#,e,c6,2b.,16b,16c6,16b,16a,1b";
const char song_64[] PROGMEM ="Crypt:d=4,o=5,b=160:d#,f#,a,8p,8b,a#,f#,d#,8p,8b4,a#4,d#,f#,a,2b4,8p,a#4,d,f,8p,8f#,g#,b,a#,8p,8g#,f#,f,d#,d,2d#,1p,1p,p.,f,g#,b,8p,8c#6,c6,g#,f,8p,8c#,c,f,g#,b,1c#,c,e,g,8p,8g#,a#,c#6,c6,8p,8a#,g#,g,f,e,2f,16p";
const char song_65[] PROGMEM ="Dallas:d=4,o=5,b=125:8e,a.,8e,e.6,8a,c#6,8b,8c#6,a,e,a,f#6,e6,8c#6,8d6,2e.6,8p,8e,a,f#6,e6,8c#6,8d6,e6,8b,8c#6,a,e,a,8c#6,8d6,b.,8a,2a";
const char song_66[] PROGMEM ="dark:d=4,o=5,b=140:8f#6,8e6,2f#6,16e6,16d#6,16d6,16b,a#,1b,8f#,8e,2f#,8c#,8d,8a#4,1b4,8f#,8e,2f#,16e,16d#,16d,16b4,a#4,1b4,8f#,8e,2f#,c#,2d,2e4,1b4";
const char song_67[] PROGMEM ="DasBoot:d=4,o=5,b=100:d#.4,8d4,8c4,8d4,8d#4,8g4,a#.4,8a4,8g4,8a4,8a#4,8d,2f.,p,f.4,8e4,8d4,8e4,8f4,8a4,c.,8b4,8a4,8b4,8c,8e,2g.,2p";
const char song_68[] PROGMEM ="DavyCrockett:d=4,o=5,b=160:f,8f.,16g,8a.,16g,8f.,16c,d,f,2c,f,g,a,8g.,16f,g,8g.,16a,2g,c,8c.,16c,f,8c.,16c,d,8d.,16d,2g,e,8e.,16e,e,8e.,16d,c,8d.,16e,2f,a,2c.6,d.6,8d6,8c6,a.,8c.,16c,8c.,16c,e,g,2f.,p,a,2c.6,d.6,8d6,8c6,a.,8c.,16c,8c.,16c,e,g,2f.";
const char song_69[] PROGMEM =" :d=4,o=5,b=100:c.,c,8c,c.,d#,8d,d,8c,c,8c,2c.";
const char song_70[] PROGMEM ="Deutschlandlied:d=4,o=5,b=160:2g,8a,b,a,c6,b,8a,8f#,g,e6,d6,c6,b,a,8b,8g,2d6,2g,8a,b,a,c6,b,8a,8f#,g,e6,d6,c6,b,a,8b,8g,2d6,a,b,8a,8f#,d,c6,b,8a,8f#,d,d6,c6,2b,8b,c#6,8c#6,8d6,2d6,2g6,8f#6,8f#6,8e6,d6,2e6,8d6,8d6,8c6,b,2a,16b,16c6,8d6,8e6,8c6,8a,2g,8b,8a,2g";
const char song_71[] PROGMEM ="Do you hear the people sing:d=4,o=5,b=140:8e.6,16d6,8c.6,16d6,8e.6,16f6,g6,8e6,8d6,8c6,8b.,16a,8b.,16c6,g,8a,8g,8f,8e.,16g,8c.6,16e6,8d.6,16c#6,8d.6,16a,8c.6,16b,8b.,16c6,d6";
const char song_72[] PROGMEM ="don'tcare:d=4,o=5,b=125:16f,16e,16f,16e,16f,16e,8d,16e,16d,16e,16d,16e,16d,16c,16d,d";
const char song_73[] PROGMEM ="don't wanna miss a thing:d=4,o=5,b=125:2p,16a,16p,16a,16p,8a.,16p,a,16g,16p,2g,16p,p,8p,16g,16p,16g,16p,16g,8g.,16p,c6,16a#,16p,a,8g,f,g,8d,8f.,16p,16f,16p,16c,8c,16p,a,8g,16f,16p,8f,16p,16c,16p,g,f";
const char song_74[] PROGMEM ="dualingbanjos:d=4,o=5,b=200:8c#,8d,e,c#,d,b4,c#,d#4,b4,p,16c#6,16p,16d6,16p,8e6,8p,8c#6,8p,8d6,8p,8b,8p,8c#6,8p,8a,8p,b,p,a4,a4,b4,c#,d#4,c#,b4,p,8a,8p,8a,8p,8b,8p,8c#6,8p,8a,8p,8c#6,8p,8b";
const char song_75[] PROGMEM ="Dustman:d=4,o=5,b=140:8a.,16a,16b,16p,16c6,16p,8c#6,p,8e6,16c#6,16p,16c#6,16p,16c#6,16p,16c#6,16p,16c#6,16p,c#6,16c#6,16p,16c#6,16p,16c#6,16p,16d6,16p,16c#6,16p,b,16b,16p,16b,16p,16b,16p,16b,16p,16b,16p,16b,16p,16b,16p,8b.,16p,16e6,16e6,16e6,16p,16d6,16p,16c#6,16p,16b,16p,a";
const char song_76[] PROGMEM ="Equidor:d=4,o=5,b=140:8g.,8d.,8a#,8a,8c6,8a,8f,8g.,8d.,8a#,8a,8c6,8a,8f,8a#.,8f.,8d6,8c6,8d6,8c6,8a,8a#.,8g.,8a#,8a,8a#,8a,8f";
const char song_77[] PROGMEM ="Eternally:d=4,o=5,b=112:b,8b,8a,8b,8c6,a,8a,8g,8a,8b,g,8g,8f#,8e,8d#,2e";
const char song_78[] PROGMEM ="Exodus:d=4,o=5,b=70:8c#,f#.,8c#6,b.,8f#,8a,8b,8g#.,16e,f#.,8c#6,e.6,8d#6,8e6,8f#6,8d#.6,16b,2c#6";
const char song_79[] PROGMEM ="Fawlty:d=4,o=5,b=125:8b,8c6,8d6,8c#6,8d6,8c#6,8d6,8g6,e.6,8d6,8c6,8b,8c6,8b,8c6,8b,8c6,8f#6,d.6,8c6,8b,8a,8g,8f#,8g,8f#,8g,8d6,8c6,8b,8c6,8b,8a,8g,8f#,8g,8e,8f#,d,8c6,8d6,8b,8c6,a";
const char song_80[] PROGMEM ="Flntstn:d=4,o=5,b=200:g#,c#,8p,c#6,8a#,g#,c#,8p,g#,8f#,8f,8f,8f#,8g#,c#,d#,2f,2p,g#,c#,8p,c#6,8a#,g#,c#,8p,g#,8f#,8f,8f,8f#,8g#,c#,d#,2c#";
const char song_81[] PROGMEM ="Friends:d=4,o=5,b=80:c,g,a#4,f,c,g,a#4,8a#,8e,c,g,a#4,f,c,g,a#4,8a#,8e";
const char song_82[] PROGMEM ="Fun2Remix:d=4,o=5,b=320:c6,8c6,g,8g,a,a#,a,g,a,c6,8c6,g,8g,a,a#,a,g,a,a#,8a#,f,8f,g,g#,g,f,g,c6,8c6,c6,8c6,8c6,8c6,c6,c6,c6,c6";
const char song_83[] PROGMEM ="FunkyTown:d=4,o=4,b=125:8c6,8c6,8a#5,8c6,8p,8g5,8p,8g5,8c6,8f6,8e6,8c6,2p,8c6,8c6,8a#5,8c6,8p,8g5,8p,8g5,8c6,8f6,8e6,8c6";
const char song_84[] PROGMEM ="song11:d=4,o=5,b=125:8g.,8g.,8g,8c,8c,8d,8d,8g.,8g.,8g,8a#,8a#,8c6,8c6,8g.,8g.,8g,8c,8c,8d,8d,8g.,8g.,8g,8a#,8a#,8c6,8d6";
const char song_85[] PROGMEM ="National Anthem:d=4,o=5,b=140:g6,g6,a6,f#.6,8g6,a6,b6,b6,c7,b.6,8a6,g6,a6,g6,f#6,g6";
const char song_86[] PROGMEM ="Greensleaves:d=4,o=5,b=140:g,2a#,c6,d.6,8d#6,d6,2c6,a,f.,8g,a,2a#,g,g.,8f,g,2a,f,2d,g,2a#,c6,d.6,8e6,d6,2c6,a,f.,8g,a,a#.,8a,g,f#.,8e,f#,2g";
const char song_87[] PROGMEM ="Halloween:d=4,o=5,b=180:8d6,8g,8g,8d6,8g,8g,8d6,8g,8d#6,8g,8d6,8g,8g,8d6,8g,8g,8d6,8g,8d#6,8g,8c#6,8f#,8f#,8c#6,8f#,8f#,8c#6,8f#,8d6,8f#,8c#6,8f#,8f#,8c#6,8f#,8f#,8c#6,8f#,8d6,8f#";
const char song_88[] PROGMEM ="HeyBaby:d=4,o=5,b=900:8a4,16a#4,16b4,16c,16c#,16d,16d#,16e,16f,16f#,16g,16g#,16a,16a#,16b,16c6,8c#6,16d6,16d#6,16e6,16f6,p,p,16a4,16a#4,16b4,16c,16c#,16d,16d#,16e,16f,16f#,16g,16g#,16a,16a#,16b,16a#,16a,16g#,16g,16f#,16f,16e,16d#,16d,16c#,16c,16b4,16a#4,16a4";
const char song_89[] PROGMEM ="Hitchcoc:d=4,o=5,b=200:16c,16p,16f4,8p,8f,32g,32p,16f,32p,16e,32p,16d,32p,16e,8p,16f,32p,16g,8p.,16c,16p,16f4,8p,8f,32g,32p,16f,32p,16e,32p,16d,32p,16e,8p,16f,32p,16g,8p.,16c,16p,16f4,8p,16g#,32p,8c6,16p,16a#,32p,16g#,8p,16c6,32p,8d#6,16p,16c#6,32p,16c6,8p,16d#6,32p,8g6,16p,16f6,32p,16e6,32p,16c#6,32p,16c6,32p,16a#,32p,16g#,32p,16g,32p,8f4";
const char song_90[] PROGMEM ="Ickley:d=4,o=5,b=100:8d,8g.,16g,8g,8d,g,8p,8a,8b.,16b,8b,8a,b,8p,8b,a,g,g,f#,2g";
const char song_91[] PROGMEM ="Indiana:d=4,o=5,b=250:e,8p,8f,8g,8p,1c6,8p.,d,8p,8e,1f,p.,g,8p,8a,8b,8p,1f6,p,a,8p,8b,2c6,2d6,2e6,e,8p,8f,8g,8p,1c6,p,d6,8p,8e6,1f.6,g,8p,8g,e.6,8p,d6,8p,8g,e.6,8p,d6,8p,8g,f.6,8p,e6,8p,8d6,2c6";
const char song_92[] PROGMEM ="GirlFromIpane:d=4,o=5,b=160:g.,8e,8e,d,g.,8e,e,8e,8d,g.,e,e,8d,g,8g,8e,e,8e,8d,f,d,d,8d,8c,e,c,c,8c,a#4,2c";
const char song_93[] PROGMEM ="I swear:d=4,o=5,b=125:2p,p,8b,8a.,16f#,8e,p,8p,8f#,8g#,a,8a,8a,a,8c#,8d,2e,8p,8f#,8g#,2e";
const char song_94[] PROGMEM ="Itchy:d=4,o=5,b=160:8c6,8a,p,8c6,8a6,p,8c6,8a,8c6,8a,8c6,8a6,p,8p,8c6,8d6,8e6,8p,8e6,8f6,8g6,p,8d6,8c6,d6,8f6,a#6,a6,2c7";
const char song_95[] PROGMEM ="Jesus:d=4,o=5,b=100:f,8d,2a#4,g,8d#,2a#4,g#,8f,8g#,g,8f,8d#,f,8d,2a#4";
const char song_96[] PROGMEM ="killing me softly:d=4,o=5,b=90:p,8e,f,g,8a,a,8g,d,g.,p,8p,8a,g,8f,8e,8e,8f,2c,p,8e,f,g,8a,a,8g,a,b,8b,8c6,8b,16a,8g,16a,2a,2a.";
const char song_97[] PROGMEM ="KnightRider:d=4,o=5,b=125:16e,16p,16f,16e,16e,16p,16e,16e,16f,16e,16e,16e,16d#,16e,16e,16e,16e,16p,16f,16e,16e,16p,16f,16e,16f,16e,16e,16e,16d#,16e,16e,16e,16d,16p,16e,16d,16d,16p,16e,16d,16e,16d,16d,16d,16c,16d,16d,16d,16d,16p,16e,16d,16d,16p,16e,16d,16e,16d,16d,16d,16c,16d,16d,16d";
const char song_98[] PROGMEM ="Lazy:d=4,o=5,b=160:8d.4,8f4,16d4,8g4,16f4,8d.4,8f4,16d4,8g4,16f4,8d4,8p,8p";
const char song_99[] PROGMEM ="Walk of Life:d=4,o=5,b=160:b.,b.,p,8p,8f#,8g,b,8g,8f,e.,e.,p,2p,p,8f,8g,b.,b.,p,8p,8f,8g,b,8g,f,e.,e.,p,8p,8f,8g,b,8g,8f,8e";
const char song_100[] PROGMEM ="Little Wing:d=4,o=5,b=63:2p,p,8e,8g,8a,a.,p,8a,8g,8g,e.,p,8d,8c,8d,16e,8d.,8p,8d,8d,8c,2a";
const char song_101[] PROGMEM ="Looney:d=4,o=5,b=140:c6,8f6,8e6,8d6,8c6,a.,8c6,8f6,8e6,8d6,8d#6,e.6,8e6,8e6,8c6,8d6,8c6,8e6,8c6,8d6,8a,8c6,8g,8a#,8a,8f";
const char song_102[] PROGMEM ="losing:d=4,o=5,b=63:2p,8b,8c#6,8b,8f#,a.,8a,8a,a,a,a.,8b,8c#6,8b,8f#,a.,8a,8a,a,a.,8b,8c#6,8b,8f#,a.,8a,8a,a,a.,8b,8c#6,8b,8f#,a,a,8a,a,8g#,2g#";
const char song_103[] PROGMEM ="Lulay Lula:d=4,o=4,b=100:d6,d6,c#6,2d6,f6,8e6,8e6,e6,d6,2c#.6,d6,e6,f6,g6,2e6,2d6,a6,2g6,f6,2e6,f6,8e6,8e6,e6,d6,2c#.6,d6,e6,f6,g6,2e6,2f#6";
const char song_104[] PROGMEM ="Macarena:d=4,o=5,b=180:f,8f,8f,f,8f,8f,8f,8f,8f,8f,8f,8a,8c,8c,f,8f,8f,f,8f,8f,8f,8f,8f,8f,8d,8c,p,f,8f,8f,f,8f,8f,8f,8f,8f,8f,8f,8a,p,2c.6,a,8c6,8a,8f,p,2p";
const char song_105[] PROGMEM ="Barbie girl:d=4,o=5,b=125:8g#,8e,8g#,8c#6,a,p,8f#,8d#,8f#,8b,g#,8f#,8e,p,8e,8c#,f#,c#,p,8f#,8e,g#,f#,8g#,8e,8g#,8c#6,a,p,8f#,8d#,8f#,8b,g#,8f#,8e,p,8e,8c#,f#,c#,p,8f#,8e,g#,f#,8g#,8e,8g#,8c#6,a,p,8f#,8d#,8f#,8b,g#,8f#,8e,p,8e,8c#,f#,c#,p,8f#,8e,g#,f#";
const char song_106[] PROGMEM ="Match of the day:d=4,o=5,b=100:8c,8f,8a,8c.6,16a,8a,8a,8a,a,8a#,8c.6,16a,8g,8a,8a#,8c,8e,8g,8a#.,16g,8g,8g,8g,g,8a,8a#.,16g,8f,8g,8a,8c,8f,8a,8c.6,16a,8a,8a,8a,a,8a#,8c.6,16a,8a#,8c6,d6,8d6,8e6,8f6,16f6,8e6,16e6,8d6,8f6,8c6,8c6,8d6,8c6,16a#,8a,16a,8g,f";
const char song_107[] PROGMEM ="missathing:d=4,o=5,b=125:2p,16a,16p,16a,16p,8a.,16p,a,16g,16p,2g,16p,p,8p,16g,16p,16g,16p,16g,8g.,16p,c6,16a#,16p,a,8g,f,g,8d,8f.,16p,16f,16p,16c,8c,16p,a,8g,16f,16p,8f,16p,16c,16p,g,f";
const char song_108[] PROGMEM ="Mission:d=4,o=6,b=100:32d,32d#,32d,32d#,32d,32d#,32d,32d#,32d,32d,32d#,32e,32f,32f#,32g,16g,8p,16g,8p,16a#,16p,16c,16p,16g,8p,16g,8p,16f,16p,16f#,16p,16g,8p,16g,8p,16a#,16p,16c,16p,16g,8p,16g,8p,16f,16p,16f#,16p,16a#,16g,2d,32p,16a#,16g,2c#,32p,16a#,16g,2c,16p,16a#5,16c";
const char song_109[] PROGMEM ="songs12:d=4,o=5,b=112:8e6,8e6,8e6,8e6,8e6,8e6,16e,16a,16c6,16e6,8d#6,8d#6,8d#6,8d#6,8d#6,8d#6,16f,16a,16c6,16d#6,d6,8c6,8a,8c6,c6,2a,32a,32c6,32e6,8a6";
const char song_110[] PROGMEM ="Monty P:d=4,o=5,b=200:f6,8e6,d6,8c#6,c6,8b,a#,8a,8g,8a,8a#,a,8g,2c6,8p,8c6,8a,8p,8a,8a,8g#,8a,8f6,8p,8c6,8c6,8p,8a,8a#,8p,8a#,8a#,8p,8c6,2d6,8p,8a#,8g,8p,8g,8g,8f#,8g,8e6,8p,8d6,8d6,8p,8a#,8a,8p,8a,8a,8p,8a#,2c6,8p,8c6";
const char song_111[] PROGMEM ="munsters:d=4,o=5,b=160:d,8f,8d,8g#,8a,d6,8a#,8a,2g,8f,8g,a,8a4,8d#4,8a4,8b4,c#,8d,p,c,c6,c6,2c6,8a#,8a,8a#,8g,8a,f,p,g,g,2g,8f,8e,8f,8d,8e,2c#,p,d,8f,8d,8g#,8a,d6,8a#,8a,2g,8f,8g,a,8d#4,8a4,8d#4,8b4,c#,2d";
const char song_112[] PROGMEM ="LightMyFire:d=4,o=5,b=140:8b,16g,16a,8b,8d6,8c6,8b,8a,8g,8a,16f,16a,8c6,8f6,16d6,16c6,16a#,16g,8g#,8g,8g#,16g,16a,8b,8c#6,16b,16a,16g,16f,8e,8f,1a,a";
const char song_113[] PROGMEM ="Newyear:d=4,o=5,b=125:a4,d.,8d,d,f#,e.,8d,e,8f#,8e,d.,8d,f#,a,2b.,b,a.,8f#,f#,d,e.,8d,e,8f#,8e,d.,8b4,b4,a4,2d,16p";
const char song_114[] PROGMEM ="PinkPanther:d=4,o=5,b=160:8d#,8e,2p,8f#,8g,2p,8d#,8e,16p,8f#,8g,16p,8c6,8b,16p,8d#,8e,16p,8b,2a#,2p,16a,16g,16e,16d,2e";
const char song_115[] PROGMEM ="peanuts:d=4,o=5,b=160:f,8g,a,8a,8g,f,2g,f,p,f,8g,a,1a,2p,f,8g,a,8a,8g,f,2g,2f,2f,8g,1g";
const char song_116[] PROGMEM ="piccolo:d=4,o=5,b=320:d6,g6,g,g6,8d6,8e6,8d6,8b,g,d,8g,8a,8b,8c6,d6,g6,1d6,d6,g6,g,g6,8d6,8e6,8b,g,d,8f,8g,8a,8b,c6,f6,1c6";
const char song_117[] PROGMEM ="Pilipom:d=4,o=5,b=160:16e,16p,16e,16p,16g,16p,16g,16p,16b4,16c#,16d,16p,16g,16p,16g,16p,16e,16p,16e,16p,16g,16p,16g,16p,16b,16g,16b,16e6,8d#6,8p,16d#6,16d6,16b,16a#,16d#6,16d6,16b,16a#,16d#6,16d6,16b,16a#,16b,16c6,16d6,16d#6,16b,16a#,16g,16f#,16e,16d#,16c,16b4,16e,16f#,16d#,16b4,8e,16p";
const char song_118[] PROGMEM ="Poison:d=4,o=5,b=112:8d,8d,8a,8d,8e6,8d,8d6,8d,8f#,8g,8c6,8f#,8g,8c6,8e,8d,8d,8d,8a,8d,8e6,8d,8d6,8d,8f#,8g,8c6,8f#,8g,8c6,8e,8d,8c,8d,8a,8d,8e6,8d,8d6,8d,8f#,8g,8c6,8f#,8g,8c6,8e,8d,8c,8d,8a,8d,8e6,8d,8d6,8d,8a,8d,8e6,8d,8d6,8d,2a,8d";
const char song_119[] PROGMEM ="polkka:d=4,o=5,b=140:16d,16c#,16d,16e,16f,16e,16f,16f#,16g,16f#,16g,16a,16a#,16a,16g,16a#,16a,16a4,16c#,16e,16a,16g,16f,16e,16f,16e,16d,16c#,16d,16a4,16b4,16c#,16d,16c#,16d,16e,16f,16e,16f,16f#,16g,16f#,16g,16a,16a#,16a,16g,16a#,16a,16a4,16c#,16e,16a,16g,16f,16e,16d,p,2c#,8d,8a4,8d";
const char song_120[] PROGMEM ="Popcorn:d=4,o=5,b=160:8c6,8a#,8c6,8g,8d#,8g,c,8c6,8a#,8c6,8g,8d#,8g,c,8c6,8d6,8d#6,16c6,8d#6,16c6,8d#6,8d6,16a#,8d6,16a#,8d6,8c6,8a#,8g,8a#,c6";
const char song_121[] PROGMEM ="2 Unlimited - No Limits:d=8,o=5,b=180:4e,4e,p,g,g,4e,4e,p,g,g,e,4e,p,g,e,a,a,b,4b,4e,4e,p,g,g,4e,4e,p,g,g,4e,4e,p,g,e,a,4a,4b,4b#";
const char song_122[] PROGMEM = "JNGLBLS:d=4,o=5,b=125:8g,8e6,8d6,8c6,2g,8g,8e6,8d6,8c6,2a,8a,8f6,8e6,8d6,8b,8g,8b,8d6,8g.6,16g6,8f6,8d6,2e6,8g,8e6,8d6,8c6,2g,16f#,8g,8e6,8d6,8c6,2a,8a,8f6,8e6,8d6,8g6,16g6,16f#6,16g6,16f#6,16g6,16g#6,8a.6,16g6,8e6,8d6,c6,g6,8e6,8e6,8e.6,16d#6,8e6,8e6,8e.6,16d#6,8e6,8g6,8c.6,16d6,2e6,8f6,8f6,8f.6,16f6,8f6,8e6,8e6,16e6,16e6,8e6,8d6,8d6,8e6,2d6";
const char song_123[] PROGMEM = "frosty:d=4,o=5,b=140:2g,e,8p,8f,g,2c6,8b,8c6,d6,c6,b,a,2g,p,8b,8c6,d6,c6,b,8a,8a,8g,c6,e.,8f,8a,g,f,e,f,2g,2p,2g,e,8p,8f,g,2c6,8b,8c6,d6,c6,b,a,2g,p,8b,8c6,d6,c6,b,8a,8a,8g,c6,e.,8g,8a,g,f,e,d,2c,p,c,a,a,c6,c6,b,a,g,e,f,a,g,f,2e,p,e,d,d,g,g,b,b,d6,8d6,8b,d6,c6,b,a,g,p,2g";
const char song_200[] PROGMEM = "smbdeath:d=4,o=5,b=90:32c6,32c6,32c6,8p";


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
