#include "Servo.h"

// Read three IR inputs (passenger, center, driver side) and set
// position of two servos (passenger, driver side) accordingly
// also respond to passenger/driver side interrupts to move servo by 45 degrees (for use by
// remote control)
//
int   g_pIR,    g_dIR,     g_cIR   ; // ir values
Servo g_pServo, g_dServo           ; // servos
int   g_pCurPos, g_dCurPos          ; // servo currentPositions
//int   g_pNext,  g_dNext              ; // next positions
int g_next;
int   IR_RAW [11] = {90, 100,110,118,147,188,230,302,360,505,510} ; //main/primary IR raw reading values
int   IR_DIST[11] = {90,  80, 70, 60, 50, 40, 30, 25, 20, 15, 10} ; //IR distance in cm corresponding to raw IR read vals
int   NUM_IR_VALS=11;
int   MAX_IR_IDX=NUM_IR_VALS-1;

int   D_SRV_P = 5;                  // passenger side servo pin
int   D_SRV_D = 4;                  // driver side servo pin

int   MAX_P   = 50;//130; //120;//180; //0;
int   MAX_D   = 130;//50;//180;

int   P_MOVEMENT = 10;            // servos are installed backwards so
int   D_MOVEMENT = 10;            // bleah, degrees go backwards

void moveServo(Servo s, int pin, int d)
{
  Serial.print("MOVING TO ");Serial.println(d);
  if( pin == D_SRV_P )
  {
    if(g_pCurPos == d)
    {
        Serial.print("P already at ");Serial.println(d);
        return;
    }
    g_pCurPos = d;
  }
  else
  {
    if(g_dCurPos == d)
    {
        Serial.print("D already at ");Serial.println(d);
        return;
    }
    g_dCurPos = d;

  }
  if( d ==0 )
  {
      d=10;
  }
  s.write(d);
}
void setup()
{
     Serial.begin(9600);

     // set up servo pins
     g_pServo.attach(D_SRV_P);
     g_dServo.attach(D_SRV_D);
     delay(500);
     
     moveToPSide();
     moveToDSide();
     crossEye();
     // move servos to 90 degrees
     moveServo(g_dServo, D_SRV_P,90);
     moveServo(g_pServo, D_SRV_D,90);
     g_pCurPos = 90;
     g_dCurPos = 90;
     delay(1000);
     Serial.println("done setup");
}


unsigned long loopCount = 0;
void loop()
{
     loopCount++;
     // otherwise, use IR's to use move logic..
     read_IRs();
     logic();

     moveServo(g_pServo,D_SRV_P,g_next);
     moveServo(g_dServo,D_SRV_D,g_next);
     delay(1000);
  
}

void moveToDSide()
{
  // do alt for 3 second
            moveServo(g_pServo,D_SRV_P,MAX_D);
            g_pCurPos =  (MAX_D);
             moveServo(g_dServo,D_SRV_D,MAX_D);
            g_dCurPos = (MAX_D);
            delay(1000);
}
void moveToPSide()
{
   // do alt for 3 second
            moveServo(g_pServo,D_SRV_P,MAX_P);
            g_pCurPos =  (MAX_P);
             moveServo(g_dServo,D_SRV_D,MAX_P);
            g_dCurPos = (MAX_P);
            delay(1000);
}
void crossEye()
{
   // do cross eye for 3 second
            moveServo(g_pServo,D_SRV_D,MAX_D);
            g_pCurPos =  (MAX_D);
             moveServo(g_dServo,D_SRV_P,MAX_P);
            g_pCurPos = (MAX_P);
            delay(2000);

            // then back to center
            moveServo(g_pServo,D_SRV_P,90);
            g_pCurPos = 90;
             moveServo(g_dServo,D_SRV_D,90);
            g_dCurPos = (90);
            delay(1000);
}
//
// Based on IR distances, determine which IR sensor has the closest thing in front of it
// or if two IR's have the same values
//

void logic()
{
     int pnxt=0;
     int cnxt=0;
     int dnxt=0; //,cnxt,dnxt;
     // out("logic g_pIR, g_cIR, g_dIR: " + g_pIR  + "," + g_cIR + "," + g_dIR);
     if (g_pIR < g_dIR)
     {
         
              pnxt=1;
              Serial.println("P");
              
              g_next = 0;
     }
         else if (g_pIR == g_dIR)
         {
              pnxt=1;
              dnxt=1;
              Serial.println("P and D???");
              
              g_next = 90;
         }
         else    
         {
              dnxt=1;
              Serial.println("D");
              
              g_next=135;
         }              
     
     Serial.print("DEG   VALS:: ");
          Serial.print(g_next);
          Serial.print("/");
          Serial.print(10000);
          Serial.print("/");
          Serial.println(g_next);
     
}
     
void read_IRs()
{
     // read raw values from IR devices
     g_pIR = analogRead(2);
     g_cIR = 10000;//analogRead(1);
     g_dIR = analogRead(0);
     // note: the larger the raw value, the closer an object is to the sensor
      Serial.print("RAW IR VALS: g_pIR/g_cIR/g_dIR: "); 
          Serial.print(g_pIR);
          Serial.print("/");
          Serial.print(10000);
          Serial.print("/");
          Serial.println(g_dIR);
     //if(true) return;
     // convert raw IR values into approx distance in centimeters
     int foundP;
     int foundC;
     int foundD;
     //Serial.print("Comparing Passenger val of ");
     //Serial.print(g_pIR);
     //Serial.print(" to ");
     for( int i = 0; i < NUM_IR_VALS; i++ )
     {
      //Serial.print(i);
      //Serial.print(" ");
      //Serial.print(g_pIR);
      //Serial.print(" ");
      //Serial.print(IR_RAW[i]);
      //Serial.print(" ");
      //Serial.print (IR_RAW[i+1]);
      //Serial.print( " " );
          if (g_pIR <= IR_RAW[0])
          {
               //Serial.print("P < min P, so using min P of ");
               //Serial.println(IR_DIST[0]);
               g_pIR = IR_DIST[0];
               break;
          }
          else if (g_pIR >= IR_RAW[MAX_IR_IDX])
          {
               g_pIR = IR_DIST[MAX_IR_IDX];
               //Serial.print("P > max P so using max P of ");
               //Serial.println(IR_DIST[MAX_IR_IDX]);
               break;
          }
          
          else if (g_pIR >= IR_RAW[i] && g_pIR < IR_RAW[i+1])
          {
               g_pIR = IR_DIST[i];
               //Serial.print("P between so using P of ");
               //Serial.println(IR_DIST[i]);
               break;
          }
          //else //Serial.println("no match, compare again");
     }
     
     /*
     Serial.print("Converting Center val of");
     Serial.print(g_cIR);
     Serial.print("...");

     for( int i = 0; i < NUM_IR_VALS; i++ )
     {
      Serial.print(i);
      Serial.print(" ");
      Serial.print(g_cIR);
      Serial.print(" ");
      Serial.print(IR_RAW[i]);
      Serial.print(" ");
      Serial.print (IR_RAW[i+1]);
      Serial.print( " " );
          if (g_cIR <= IR_RAW[0])
          {
               Serial.println("min c");
               g_cIR = IR_DIST[0];
               break;
          }
          else if (g_cIR >= IR_RAW[MAX_IR_IDX])
          {
               g_cIR = IR_DIST[MAX_IR_IDX];
               Serial.println("max c");
               break;
          }
          
          else if (g_cIR >= IR_RAW[i] && g_cIR < IR_RAW[i+1])
          {
               g_cIR = IR_DIST[i];
               Serial.println("med c");
               break;
          }
          else Serial.println("no match");
     }
     */
     //Serial.print("Converting Driver val of ");
     //Serial.print(g_dIR);
     //Serial.print("...");
     for( int i = 0; i < NUM_IR_VALS; i++ )
     {
      /*Serial.print(i);
      Serial.print(" ");
      Serial.print(g_dIR);
      Serial.print(" ");
      Serial.print(IR_RAW[i]);
      Serial.print(" ");
      Serial.print (IR_RAW[i+1]);
      Serial.print( " " );*/
          if (g_dIR <= IR_RAW[0])
          {
               //Serial.println("min d");
               g_dIR = IR_DIST[0];
               break;
          }
          else if (g_dIR >= IR_RAW[MAX_IR_IDX])
          {
               g_dIR = IR_DIST[MAX_IR_IDX];
               //Serial.println("max d");
               break;
          }
          
          else if (g_dIR >= IR_RAW[i] && g_dIR < IR_RAW[i+1])
          {
               g_dIR = IR_DIST[i];
               //Serial.println("med d");
               break;
          }
          //else Serial.println("no match");
     }
     Serial.print("DST IR VALS: g_pIR/g_cIR/g_dIR: "); 
          Serial.print(g_pIR);
          Serial.print("/");
          Serial.print(10000);
          Serial.print("/");
          Serial.print (g_dIR);
          Serial.println(" cm");
}
     

