/*
  LED Control

  This example scans for BLE peripherals until one with the advertised service
  "19b10000-e8f2-537e-4f6c-d104768a1214" UUID is found. Once discovered and connected,
  it will remotely control the BLE Peripheral's LED, when the button is pressed or released.

  The circuit:
  - Arduino MKR WiFi 1010, Arduino Uno WiFi Rev2 board, Arduino Nano 33 IoT,
    Arduino Nano 33 BLE, or Arduino Nano 33 BLE Sense board.
  - Button with pull-up resistor connected to pin 2.

  You can use it with another board that is compatible with this library and the
  Peripherals -> LED example.

  This example code is in the public domain.
*/

#include <ArduinoBLE.h>


/////////////servo potentiometer
const int potPin = A6;

/////////////EXTRA! led on central board
const int ledCentral = 12;//flower
const int ledCentral2 = 11;//koi fish

////////////light sensor
const int lightSensor = A0;

////////////rain sensor
const int rainSensor = A7;

//button that control LED
const int buttonPin = 2;
int oldButtonState = LOW;


void setup() {
  Serial.begin(9600);
//  while (!Serial);



  // initialize the BLE hardware
  BLE.begin();
  Serial.println("BLE Central - LED and servo control");

  // start scanning for peripherals
  BLE.scanForUuid("c74cf98c-5bf7-48da-a214-290466c81784");//////////////SERVO
  //  BLE.scanForUuid("4b189721-f170-4358-b9e5-5aa31517c161");///////////////SERVO 2

  BLE.scanForUuid("14478ec9-fe39-4f88-b53e-9bd4c405f3de");//////LED2
  //  BLE.scanForUuid("902c7769-6750-411a-bd7a-bdbbe6f70a10");//////LEd3
  //  BLE.scanForUuid("4e93a2dc-362f-4516-a1a9-c8c2785aa67b");//////LED4
  BLE.scanForUuid("537f1528-4ae3-4613-a22f-8f121b58c832");//////LED5
  BLE.scanForUuid("acd14ef6-0bc3-4d5f-b96e-dbc6e58b725f");//////LED controlled by button

  //EXTRA! LED on central
  pinMode(ledCentral, OUTPUT);
  pinMode(ledCentral2, OUTPUT);

  //lightSensor
  pinMode(lightSensor, INPUT);


  //rainSensor
  pinMode(rainSensor, INPUT);

  //button controlling LED
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {




  // check if a peripheral has been discovered
  BLEDevice peripheral = BLE.available();

  if (peripheral) {
    // discovered a peripheral, print out address, local name, and advertised service
    Serial.print("Found ");
    Serial.print(peripheral.address());
    Serial.print(" '");
    Serial.print(peripheral.localName());
    Serial.print("' ");
    Serial.print(peripheral.advertisedServiceUuid());
    Serial.println();
    if (peripheral.localName() != "Peripheral") {
      return;
    }

    // stop scanning
    BLE.stopScan();
    controlLed(peripheral);

    // peripheral disconnected, start scanning again
    BLE.scanForUuid("c74cf98c-5bf7-48da-a214-290466c81784");///////////////SERVO 1
    //    BLE.scanForUuid("4b189721-f170-4358-b9e5-5aa31517c161");///////////////SERVO 2

    BLE.scanForUuid("14478ec9-fe39-4f88-b53e-9bd4c405f3de");//////LED2
    //    BLE.scanForUuid("902c7769-6750-411a-bd7a-bdbbe6f70a10");//////LEd3
    //    BLE.scanForUuid("4e93a2dc-362f-4516-a1a9-c8c2785aa67b");//////LED4
    BLE.scanForUuid("537f1528-4ae3-4613-a22f-8f121b58c832");//////LED5

    BLE.scanForUuid("acd14ef6-0bc3-4d5f-b96e-dbc6e58b725f");//////LED controlled by button

  }
}

void controlLed(BLEDevice peripheral) {
  // connect to the peripheral
  Serial.println("Connecting ...");

  if (peripheral.connect()) {
    Serial.println("Connected");
  } else {
    Serial.println("Failed to connect!");
    return;
  }

  // discover peripheral attributes
  Serial.println("Discovering attributes ...");
  if (peripheral.discoverAttributes()) {
    Serial.println("Attributes discovered");
  } else {
    Serial.println("Attribute discovery failed!");
    peripheral.disconnect();
    return;
  }


  ////////////////////////////SERVO. retrieve servo characteristic
  //servo 1
  BLECharacteristic servo1Characteristic = peripheral.characteristic("c74cf98c-5bf7-48da-a214-290466c81784");
  if (!servo1Characteristic) {
    Serial.println("servo 1 does not have characteristic!");
    peripheral.disconnect();
    return;
  } else if (!servo1Characteristic.canWrite()) {
    Serial.println("servo 1 does not have a writable characteristic!");
    peripheral.disconnect();
    return;
  }


  //  //servo 2
  //  BLECharacteristic servo2Characteristic = peripheral.characteristic("4b189721-f170-4358-b9e5-5aa31517c161");
  //  if (!servo2Characteristic) {
  //    Serial.println("servo 2 does not have characteristic!");
  //    peripheral.disconnect();
  //    return;
  //  } else if (!servo2Characteristic.canWrite()) {
  //    Serial.println("servo 2 does not have a writable characteristic!");
  //    peripheral.disconnect();
  //    return;
  //  }
  //////////////////////END OF SERVO


  ////////////////////////////LEDs retrieve LEDs characteristic
  //LED2
  BLECharacteristic led2Chara = peripheral.characteristic("14478ec9-fe39-4f88-b53e-9bd4c405f3de");
  if (!led2Chara) {
    Serial.println("servo does not have characteristic!");
    peripheral.disconnect();
    return;
  } else if (!led2Chara.canWrite()) {
    Serial.println("servo does not have a writable characteristic!");
    peripheral.disconnect();
    return;
  }

  //  //LED3
  //  BLECharacteristic led3Chara = peripheral.characteristic("902c7769-6750-411a-bd7a-bdbbe6f70a10");
  //  if (!led3Chara) {
  //    Serial.println("servo does not have characteristic!");
  //    peripheral.disconnect();
  //    return;
  //  } else if (!led3Chara.canWrite()) {
  //    Serial.println("servo does not have a writable characteristic!");
  //    peripheral.disconnect();
  //    return;
  //  }
  //
  //  //LED4
  //  BLECharacteristic led4Chara = peripheral.characteristic("4e93a2dc-362f-4516-a1a9-c8c2785aa67b");
  //  if (!led4Chara) {
  //    Serial.println("servo does not have characteristic!");
  //    peripheral.disconnect();
  //    return;
  //  } else if (!led4Chara.canWrite()) {
  //    Serial.println("servo does not have a writable characteristic!");
  //    peripheral.disconnect();
  //    return;
  //  }

  //LED5
  BLECharacteristic led5Chara = peripheral.characteristic("537f1528-4ae3-4613-a22f-8f121b58c832");
  if (!led5Chara) {
    Serial.println("servo does not have characteristic!");
    peripheral.disconnect();
    return;
  } else if (!led5Chara.canWrite()) {
    Serial.println("servo does not have a writable characteristic!");
    peripheral.disconnect();
    return;
  }

  //LED controlled by button
  BLECharacteristic ledButtonChara = peripheral.characteristic("acd14ef6-0bc3-4d5f-b96e-dbc6e58b725f");
  if (!ledButtonChara) {
    Serial.println("servo does not have characteristic!");
    peripheral.disconnect();
    return;
  } else if (!ledButtonChara.canWrite()) {
    Serial.println("servo does not have a writable characteristic!");
    peripheral.disconnect();
    return;
  }




  while (peripheral.connected()) {
    //////////////////////SERVO
    //servo 1
    int potVal = analogRead(potPin);
    servo1Characteristic.writeValue(map(potVal, 0, 500, 1, 180));

    //    //servo 2
    //    servo2Characteristic.writeValue(map(potVal, 0, 1023, 180, 1));

    ////////////////////////////////////////////END OF SERVO



    //////////////LED 2 to 5/////////////
    //proximity
    //    if (APDS.proximityAvailable()) {
    //      led2Chara.writeValue(map(APDS.readProximity(), 0, 255, 1, 255));
    //      led3Chara.writeValue(map(APDS.readProximity(), 0, 255, 1, 255));
    //      led4Chara.writeValue(map(APDS.readProximity(), 0, 255, 1, 255));
    //      led5Chara.writeValue(map(APDS.readProximity(), 0, 255, 1, 255));
    //    }

    //rain
    int numReadRain = 80;
    int rainSensorSum = 0;
    for (int r = 0; r < numReadRain; r++) {
      rainSensorSum += analogRead(rainSensor);
    }
    int rainSensorAvg = rainSensorSum / numReadRain;
    //    Serial.print(analogRead(rainSensor));
    //    Serial.print('\t');
    //    Serial.println(rainSensorAvg);

    led2Chara.writeValue(map(rainSensorAvg, 250, 1023, 255, 1));
    //    led3Chara.writeValue(map(rainSensorAvg, 250, 1023, 255, 1));
    //    led4Chara.writeValue(map(rainSensorAvg, 250, 1023, 255, 1));
    led5Chara.writeValue(map(rainSensorAvg, 250, 1023, 255, 1));



    //////////////EXTRA LED
    int numRead = 20;
    int lightSensorSum = 0;
    for (int k = 0; k < numRead; k++) {
      lightSensorSum += analogRead(lightSensor);
    }
    int lightSensorAvg = lightSensorSum / numRead;
    //    Serial.print(analogRead(lightSensor));
    //    Serial.print('\t');
    //    Serial.println(lightSensorAvg);
    int constrainAvg = constrain(lightSensorAvg, 20, 90);

    //    analogWrite(ledCentral, map(constrainAvg, 20, 90, 255, 0));
    analogWrite(ledCentral2, map(constrainAvg, 20, 90, 255, 0));

    //////////////////////END OF LEDs


    //////////////button controlling LED/////////////
    // read the button pin
    int buttonState = digitalRead(buttonPin);

    if (oldButtonState != buttonState) {
      // button changed
      oldButtonState = buttonState;

      if (buttonState) {
        Serial.println("button pressed");

        // button is pressed, write 0x01 to turn the LED on
        ledButtonChara.writeValue((byte)0x00);
      } else {
        Serial.println("button released");

        // button is released, write 0x00 to turn the LED off
        ledButtonChara.writeValue((byte)0x01);
      }
    }//lights up flower on peripheral

    //flower light on central also lights up
    digitalWrite(ledCentral, digitalRead(buttonPin));

  }
  //////////////end of button controlling LED/////////////

  Serial.println("Peripheral disconnected");
}
