
#include <lorawan.h>
#include <stdlib.h> // needed for random()

// OTAA credentials
const char *devEui = "60C5A8FFFE78A255";
const char *appEui = "0000000000000000";
const char *appKey = "32B36D9139B3FAB0FA33CFE991C2175D";

const unsigned long interval = 20000;    // 20 s interval to send message
unsigned long previousMillis = 0;  // will store last time message sent
unsigned int counter = 0;     // message counter

char myStr[50];
char outStr[255];
byte recvStatus = 0;

const sRFM_pins RFM_pins = {
  .CS = SS,
  .RST = RFM_RST,
  .DIO0 = RFM_DIO0,
  .DIO1 = RFM_DIO1,
  .DIO2 = RFM_DIO2,
  .DIO5 = RFM_DIO5,
};

void setup() {
  // Setup loraid access
  Serial.begin(115200);
  while(!Serial);

  if(!lora.init()){
    Serial.println("RFM95 not detected");
    delay(5000);
    return;
  }

  // Set LoRaWAN Class change CLASS_A or CLASS_C
  lora.setDeviceClass(CLASS_A);

  // Set Data Rate
  lora.setDataRate(SF8BW125);

  // set channel to random
  lora.setChannel(MULTI);
  
  // Put OTAA Key and DevAddress here
  lora.setDevEUI(devEui);
  lora.setAppEUI(appEui);
  lora.setAppKey(appKey);

  // Join procedure
  bool isJoined;
  do {
    Serial.println("Joining...");

    isJoined = lora.join();
    
    //wait for 10s to try again
    delay(10000);
  }while(!isJoined);
  Serial.println("Joined to network");
}

void loop() {
  // Check interval overflow
  if(millis() - previousMillis > interval) {
    previousMillis = millis(); 

    int temp = random(10, 40); // random temperature value between 10 and 40 degrees Celsius
    int hum = random(30, 80); // random humidity value between 30% and 80%

    sprintf(myStr, "Temp=%d,Hum=%d", temp, hum); 

    Serial.print("Sending: ");
    Serial.println(myStr);
    
    lora.sendUplink(myStr, strlen(myStr), 0,1);
    counter++;
  }

  recvStatus = lora.readData(outStr);
  if(recvStatus) {
    Serial.println(outStr);
  }
  
  // Check Lora RX
  lora.update();
}
