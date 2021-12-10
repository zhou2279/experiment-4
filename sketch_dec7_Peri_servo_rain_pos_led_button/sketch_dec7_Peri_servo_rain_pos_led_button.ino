/*
  LED

  This example creates a BLE peripheral with service that contains a
  characteristic to control an LED.

  The circuit:
  - Arduino MKR WiFi 1010, Arduino Uno WiFi Rev2 board, Arduino Nano 33 IoT,
    Arduino Nano 33 BLE, or Arduino Nano 33 BLE Sense board.

  You can use a generic BLE central app, like LightBlue (iOS and Android) or
  nRF Connect (Android), to interact with the services and characteristics
  created in this sketch.

  This example code is in the public domain.
*/

//peripheral--listening constantly and when it gets the right value(1)
//it'll turn on the LED

#include <ArduinoBLE.h>
#include <Servo.h>



////////// LEDs GROUP/////////////

//LED2
BLEService led2Service("14478ec9-fe39-4f88-b53e-9bd4c405f3de");
BLEByteCharacteristic led2Chara("14478ec9-fe39-4f88-b53e-9bd4c405f3de", BLEWrite);
const int led2 = 2;

////LED3
//BLEService led3Service("902c7769-6750-411a-bd7a-bdbbe6f70a10");
//BLEByteCharacteristic led3Chara("902c7769-6750-411a-bd7a-bdbbe6f70a10", BLEWrite);
//const int led3 = 6;
//
////LED4
//BLEService led4Service("4e93a2dc-362f-4516-a1a9-c8c2785aa67b");
//BLEByteCharacteristic led4Chara("4e93a2dc-362f-4516-a1a9-c8c2785aa67b", BLEWrite);
//const int led4 = 4;

//LED5
BLEService led5Service("537f1528-4ae3-4613-a22f-8f121b58c832");
BLEByteCharacteristic led5Chara("537f1528-4ae3-4613-a22f-8f121b58c832", BLEWrite);
const int led5 = 5;

//LED button
BLEService ledButtonService("acd14ef6-0bc3-4d5f-b96e-dbc6e58b725f");
BLEByteCharacteristic ledButtonChara("acd14ef6-0bc3-4d5f-b96e-dbc6e58b725f", BLEWrite);
const int ledButton = A1;




////////// END of LED GROUP/////////////



//////////////////////SERVO
Servo servo1;
int servo1Pin = A6;
unsigned long lastUpdate1 = 0;
int pos1 = 0;
int sp1 = 3;
BLEService servo1Service("c74cf98c-5bf7-48da-a214-290466c81784");
BLEByteCharacteristic servo1Characteristic("c74cf98c-5bf7-48da-a214-290466c81784", BLERead | BLEWrite);


Servo servo2;
int servo2Pin = A0;
unsigned long lastUpdate2 = 0;
int pos2 = 0;
int sp2 = 1;
//BLEService servo2Service("4b189721-f170-4358-b9e5-5aa31517c161");
//BLEByteCharacteristic servo2Characteristic("4b189721-f170-4358-b9e5-5aa31517c161", BLERead | BLEWrite);
//////////////////////END OF SERVO




void setup() {
  Serial.begin(9600);
  //is the board connected to a computer (serial port)?
  //if not, it won't go pass here.
  //if you'll be using wall power, comment it out.
  //    while (!Serial);


  // begin initialization
  //turn on the radio.
  //also lets you know if it didn't turn on
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }

  // set advertised local name for entire peripheral and service UUID:
  //.setlocalName--the name that'll appear when your device shows up when scanning
  BLE.setLocalName("Peripheral");




  ///////////////////////////////////SERVO
  servo1.attach(servo1Pin);

  BLE.setAdvertisedService(servo1Service);
  servo1Service.addCharacteristic(servo1Characteristic);
  BLE.addService(servo1Service);
  servo1Characteristic.writeValue(0);

  //servo2!
  servo2.attach(servo2Pin);

  //  BLE.setAdvertisedService(servo2Service);
  //  servo2Service.addCharacteristic(servo2Characteristic);
  //  BLE.addService(servo2Service);
  //  servo2Characteristic.writeValue(0);
  ////////////////////////////END OF SERVO



  ///////////////////////////////LED Group/////
  //LED 2
  pinMode(led2, OUTPUT);

  BLE.setAdvertisedService(led2Service);
  led2Service.addCharacteristic(led2Chara);
  BLE.addService(led2Service);
  //  led2Chara.writeValue(0);


  //  //LED 3
  //  pinMode(led3, OUTPUT);
  //
  //  BLE.setAdvertisedService(led3Service);
  //  led3Service.addCharacteristic(led3Chara);
  //  BLE.addService(led3Service);
  //  //  led3Chara.writeValue(0);
  //
  //
  //  //LED 4
  //  pinMode(led4, OUTPUT);
  //
  //  BLE.setAdvertisedService(led4Service);
  //  led4Service.addCharacteristic(led4Chara);
  //  BLE.addService(led4Service);
  //  //  led4Chara.writeValue(0);


  //LED 5
  pinMode(led5, OUTPUT);

  BLE.setAdvertisedService(led5Service);
  led5Service.addCharacteristic(led5Chara);
  BLE.addService(led5Service);
  //  led5Chara.writeValue(0);


  ///////////////LED button
  pinMode(ledButton, OUTPUT);

  BLE.setAdvertisedService(ledButtonService);
  ledButtonService.addCharacteristic(ledButtonChara);
  BLE.addService(ledButtonService);
  ledButtonChara.writeValue(0);



  ////////////////////////END of LED





  // start advertising so a central can connect to it
  BLE.advertise();

  Serial.println("BLE SERVO Peripheral");

}

void loop() {



  // listen for BLE peripherals to connect:
  BLEDevice central = BLE.central();

  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());

    // while the central is still connected to peripheral:
    //put into a look because the central is always looking for connection
    while (central.connected()) {


      //////////////////////////////SERVO//////////
      //servo 1
      if (servo1Characteristic.written()) {
        if (servo1Characteristic.value()) {
          servo1.write(servo1Characteristic.value());
        }
      }
      //speed attempt
      //
      //      if (servo1Characteristic.written()) {
      //        if (servo1Characteristic.value()) {
      //          if (millis() - lastUpdate1 >= servo1Characteristic.value()) {
      //            servo1.write(pos1);
      //            pos1 += sp1;
      //
      //            if (pos1 <= 0 || pos1 >= 180) {
      //              sp1 = -sp1;
      //            }
      //            lastUpdate1 = millis();
      //            Serial.print(pos1);
      //            Serial.println("\t");
      //            Serial.print((int)servo1Characteristic.value());
      //          }
      //        }
      //      }



      //servo 2
      //      if (servo2Characteristic.written()) {
      //        if (servo2Characteristic.value()) {
      //          servo2.write(servo2Characteristic.value());
      //        }
      //      }



      if (millis() - lastUpdate2 >= 100) {
        servo2.write(pos2);
        pos2 += sp2;

        if (pos2 <= 0 || pos2 >= 180) {
          sp2 = -sp2;
        }
        lastUpdate2 = millis();
      }

      ////////////////////////////END OF SERVO/////



      //////////////////LED/////////////////////
      //LED 2

      if (led2Chara.written()) {
        if (led2Chara.value()) {
          analogWrite(led2, led2Chara.value());
        }
      }

      //      //LED 3
      //
      //      if (led3Chara.written()) {
      //        if (led3Chara.value()) {
      //          analogWrite(led3, led3Chara.value());
      //        }
      //      }
      //
      //      //LED 4
      //
      //      if (led4Chara.written()) {
      //        if (led4Chara.value()) {
      //          analogWrite(led4, led4Chara.value());
      //        }
      //      }

      //LED 5

      if (led5Chara.written()) {
        if (led5Chara.value()) {
          analogWrite(led5, led5Chara.value());
          Serial.println(led5Chara.value());
        }
      }

      //button controlled LED

      if (ledButtonChara.written()) {
        if (ledButtonChara.value()) {
          digitalWrite(ledButton, HIGH);         // will turn the LED on
        } else {                              // a 0 value
          digitalWrite(ledButton, LOW);
        }
      }


      //////////////////END of LED/////////////////////




    }


    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());


  }
}
