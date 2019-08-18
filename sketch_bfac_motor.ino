#include <IRLib.h>
int ir_pin = 11;
//int button_pin = 4
int motor_pin1  = 5;
int motor_pin2  = 6;
int running = 0;

IRrecv My_Receiver(ir_pin);
IRdecode My_Decoder;
IRdecodeHash My_Hash_Decoder;

void setup() {
   Serial.begin(9600);
   Serial.println("Entering setup...");
   My_Receiver.enableIRIn(); // Start the receiver

   // use this if using momentary button pinMode(button_pin,INPUT_PULLUP);
   pinMode(motor_pin1,OUTPUT);
   pinMode(motor_pin2,OUTPUT);
 
   Serial.println("...exiting setup");
}

void loop() {
  Serial.println("enter loop");

  //int buttonValue = digitalRead(button_pin);
  //Serial.print("button state: [");Serial.print(buttonValue);Serial.println("]");
  
  if (My_Receiver.GetResults(&My_Decoder)){
    // turn on for 5 seconds
    Serial.println("*****button/ir detected, turning on motor...");
    digitalWrite(motor_pin1,LOW);
    digitalWrite(motor_pin2,HIGH);
    delay(1000);
    digitalWrite(motor_pin1,HIGH);
    digitalWrite(motor_pin2,LOW);
    delay(1000);
  }
 My_Receiver.resume(); 

  // turn off!
  {
    Serial.println("...turning off motor");
    digitalWrite(motor_pin1,LOW);
    digitalWrite(motor_pin2,LOW);
        //delay(5000);

    
  }

}
