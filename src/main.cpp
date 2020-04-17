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


unsigned long time;
unsigned long lastPulseTime;
unsigned long lastCode=0;
byte motorDir = motorDirStop;
byte mode = modeStateSwitch;

void loop() {
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
            motorDir = motorDirUp;
            Serial.println("UP !");
            break;
          case motorDirDown:                       //Le moteur est en descente, on stoppe
            motorDir = motorDirStop;
            Serial.println("STOP !");
            break;
          case motorDirUp:                        //Le moteur est déjà en montée, on stoppe
            motorDir = motorDirStop;
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
            motorDir = motorDirDown;
            Serial.println("DOWN !");
            break;
          case motorDirDown:                       //Le moteur est déjà en descente, on stoppe
            motorDir = motorDirStop;
            Serial.println("STOP !");
            break;
          case motorDirUp:                        //Le moteur est en montée, on stoppe
            motorDir = motorDirStop;
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
  Serial.begin(115200);
  Serial.println("Init OK !");
  // Initialize receiver on interrupt 0 (= digital pin 2), calls the callback "showCode"
  // after 3 identical codes have been received in a row. (thus, keep the button pressed
  // for a moment)
  //
  // See the interrupt-parameter of attachInterrupt for possible values (and pins)
  // to connect the receiver.
  RemoteReceiver::init(0, 3, showCode);
}