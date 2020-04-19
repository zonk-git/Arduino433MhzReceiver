#include <Arduino.h>
#include <RemoteReceiver.h>

#define ButtonUp 531363
#define ButtonDown 531369
#define motorDirStop 0
#define motorDirUp 1
#define motorDirDown 2
#define debounceTime 250
#define modeMomentarySwitch 0
#define modeStateSwitch 1
#define pinRelay1 3
#define pinRelay2 4
#define pinButton1 5
#define pinButton2 6


unsigned long time;
unsigned long lastPulseTime;
unsigned long lastCode=0;
byte motorDir = motorDirStop;
byte mode = modeStateSwitch; 



void setMotorDir(byte motorDirToGo) {
   switch(motorDirToGo){
     case motorDirStop:
        digitalWrite(pinRelay1,0);
        digitalWrite(pinRelay2,0);
        motorDir = motorDirStop;
        Serial.println("SetMotorDir Stop");
     break;
     case motorDirUp:
        digitalWrite(pinRelay1,1);
        digitalWrite(pinRelay2,0);
        motorDir = motorDirUp;
        Serial.println("SetMotorDir Up");
     break;
     case motorDirDown:
        digitalWrite(pinRelay1,0);
        digitalWrite(pinRelay2,1);
        motorDir = motorDirDown;
        Serial.println("SetMotorDir Down");
     break;
     default:
        Serial.println("SetMotorDir Anomalie");
   }
}


// Callback function is called only when a valid code is received.
void showCode(unsigned long receivedCode, unsigned int period) {
  // Note: interrupts are disabled. You can re-enable them if needed.
  // Print the received code.
  //Serial.print("Code: ");
  //Serial.print(receivedCode);
  time = millis();
  if (receivedCode==lastCode && time-lastPulseTime<debounceTime) lastPulseTime = time;
  if (receivedCode!=lastCode || time-lastPulseTime>debounceTime) { 
    lastCode = receivedCode;
    lastPulseTime = time;
    if (receivedCode == ButtonUp) {                // Reception du code Bouton "B" pour monter
      switch (motorDir) {
          case motorDirStop:                       //Le moteur est à l'arret donc on monte
            setMotorDir(motorDirUp);
            Serial.println("UP !");
            break;
          case motorDirDown:                       //Le moteur est en descente, on stoppe
            setMotorDir(motorDirStop);
            Serial.println("STOP !");
            break;
          case motorDirUp:                        //Le moteur est déjà en montée, on stoppe
            setMotorDir(motorDirStop);
            Serial.println("STOP !");
            break;
          default:                                 //On ne devrait pas arriver ici si tout va bien
            Serial.println("Anomalie en montée ! Le moteur est dans un état inconnu !");
            break;
          }
    }
    if (receivedCode == ButtonDown) {
      switch (motorDir) {
          case motorDirStop:                       //Le moteur est à l'arret donc on descend
            setMotorDir(motorDirDown);
            Serial.println("DOWN !");
            break;
          case motorDirDown:                       //Le moteur est déjà en descente, on stoppe
            setMotorDir(motorDirStop);
            Serial.println("STOP !");
            break;
          case motorDirUp:                        //Le moteur est en montée, on stoppe
            setMotorDir(motorDirStop);
            Serial.println("STOP !");
            break;
          default:                                 //On ne devrait pas arriver ici si tout va bien
            Serial.println("Anomalie en descente ! Le moteur est dans un état inconnu !");
            break;
          }
    }
  }
}


// Callback function is called only when a valid code is received.
void fixedSwitchPressed(byte buttonPressed) {

}

void setup() {
  pinMode(pinRelay1,OUTPUT);
  pinMode(pinRelay2,OUTPUT);
  pinMode(pinButton1,INPUT_PULLUP);
  pinMode(pinButton2,INPUT_PULLUP);
  // Initialize receiver on interrupt 0 (= digital pin 2), calls the callback "showCode"
  // after 3 identical codes have been received in a row. (thus, keep the button pressed
  // for a moment)
  //
  // See the interrupt-parameter of attachInterrupt for possible values (and pins)
  // to connect the receiver.
  RemoteReceiver::init(0, 3, showCode);
  Serial.begin(115200);
  Serial.println("Init OK !");

}

void loop() {
  if (digitalRead(pinButton1) == 0) showCode(ButtonDown,0);
  if (digitalRead(pinButton2) == 0) showCode(ButtonUp,0);
  delay(50);
}