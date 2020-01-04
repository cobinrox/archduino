/***************************************************
 Xmas Fish
 Play all mp3 files, check for fwd/rev/pause/continue
 SDCard has one folder, mp3, with files 0001.mp3, 0002.mp3, etc.
 
 Based on
 Durch Wolfgang Ewald modifizierte Version des FullFunction.ino Beispielsketches der DFRobotDFPlayerMini Bibliothek:
 
 DFPlayer - A Mini MP3 Player For Arduino
 <https://www.dfrobot.com/product-1121.html>
**/

#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
# define ACTIVATED LOW

SoftwareSerial mySoftwareSerial(10,11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
int buttonNext = 2;
int buttonPause = 3;
int buttonPrevious = 4;
boolean isPlaying = false;
void setup()
{
  // some arduino nano clones require this delay at start
  // up when running off of stand alone power supply or the
  // remainder of the program won't run unless the restart
  // button is hit
  // reference:
  // https://forum.arduino.cc/index.php?topic=515895.0
  delay(1000);
  
  mySoftwareSerial.begin(9600);
  Serial.begin(9600);
  pinMode(buttonPause, INPUT);
  digitalWrite(buttonPause,HIGH);
  pinMode(buttonNext, INPUT);
  digitalWrite(buttonNext,HIGH);
  pinMode(buttonPrevious, INPUT);
  digitalWrite(buttonPrevious,HIGH);

  Serial.println();
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  
  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  Serial.println(F("DFPlayer Mini online."));
  
  myDFPlayer.setTimeOut(500); //Set serial communictaion time out 500ms
  
  //----Set different EQ----
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
  //  myDFPlayer.EQ(DFPLAYER_EQ_POP);
  //  myDFPlayer.EQ(DFPLAYER_EQ_ROCK);
  //  myDFPlayer.EQ(DFPLAYER_EQ_JAZZ);
  //  myDFPlayer.EQ(DFPLAYER_EQ_CLASSIC);
  //  myDFPlayer.EQ(DFPLAYER_EQ_BASS);
  
  //----Set device we use SD as default----
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);

  // set volume
  myDFPlayer.volume(30);
  delay(1000);

  // start !
  myDFPlayer.start();
  delay(1000);

  isPlaying = true;

}

void loop(){  
  if (myDFPlayer.available()) {
    int fin = getStatus(myDFPlayer.readType(), myDFPlayer.read());
    if( fin == 666) {
      Serial.println('DONE DONE DONE');
      myDFPlayer.next();
    }
  } else
  {
    if (digitalRead(buttonPause) == ACTIVATED)
    {
      if(isPlaying)
      {
        myDFPlayer.pause();
        delay(1000);
        isPlaying = false;
      }else
      {
        isPlaying = true;
        myDFPlayer.start();  //start the mp3 from the pause
        delay(1000);
      }
    }
  
    if (digitalRead(buttonNext) == ACTIVATED)
    {
      if(isPlaying)
      {
         myDFPlayer.next();  //Play next mp3
        delay(1000);
      }
    }
  
     if (digitalRead(buttonPrevious) == ACTIVATED)
    {
      if(isPlaying)
      {
        myDFPlayer.previous();  //Play previous mp3
        delay(1000);
      }
    }
  }
}

/**
 * Return 666 if at end of song/file
 */
int getStatus(uint8_t type, int value){
  int ret = 0;
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
//    case DFPlayerCardOnline:
//      Serial.println(F("Card Online!"));
//      break;
//    case DFPlayerUSBInserted:
//      Serial.println("USB Inserted!");
//      break;
//    case DFPlayerUSBRemoved:
//      Serial.println("USB Removed!");
//      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      ret = 666;
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }  
  return ret;
}
