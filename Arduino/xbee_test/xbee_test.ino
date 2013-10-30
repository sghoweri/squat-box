
// Libraries required for the luminosity sensor
#include <Wire.h>



#include <aJSON.h>

#include <XBee.h>
#include <SoftwareSerial.h>






// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 7, 8, 2, 10);
const int contrastPIN = 4;


XBee xbee = XBee();                    //instantiate the XBee object




XBeeAddress64 addr64 = XBeeAddress64(0x0013A200, 0x40A1A14E);
//ZBTxRequest zbTx = ZBTxRequest(addr64, sendPayload, sizeof(sendPayload));
ZBRxResponse zbRX = ZBRxResponse();    //XBee RX packet

ModemStatusResponse msr = ModemStatusResponse();
ZBTxStatusResponse txStatus = ZBTxStatusResponse();


uint8_t ssRX = A0;
uint8_t ssTX = A1;

int buttonPin = A3;
int buttonState = 0;



int pizeoPin = 9;


int redLEDPin = 6;
int greenLEDPin = 3;
int blueLEDPin = 5;



int tempPin = A2;




SoftwareSerial nss(ssRX, ssTX);
//XBeeAddress64 senderAddr;










    








unsigned long last_print = 0;
//aJsonStream serial_stream(&Serial);



byte dataLen;








aJsonObject *root,*fmt;




//aJsonObject* jsonObject = aJson.parse(json_string);














/* Generate message like: { "analog": [0, 200, 400, 600, 800, 1000] } */
aJsonObject *createMessage()
{
  aJsonObject *msg = aJson.createObject();

  int analogValues[6];
  for (int i = 0; i < 6; i++) {
    analogValues[i] = analogRead(i);
  }
  aJsonObject *analog = aJson.createIntArray(analogValues, 6);
  aJson.addItemToObject(msg, "analog", analog);

  return msg;
}



/* Process message like: { "pwm": { "8": 0, "9": 128 } } */
void processMessage(aJsonObject *msg)
{
  aJsonObject *pwm = aJson.getObjectItem(msg, "pwm");
  if (!pwm) {
    Serial.println("no pwm data");
    return;
  }

  const static int pins[] = { 8, 9 };
  const static int pins_n = 2;
  for (int i = 0; i < pins_n; i++) {
    char pinstr[3];
    snprintf(pinstr, sizeof(pinstr), "%d", pins[i]);

    aJsonObject *pwmval = aJson.getObjectItem(pwm, pinstr);
    if (!pwmval) continue; /* Value not provided, ok. */
    if (pwmval->type != aJson_Int) {
      Serial.print("invalid data type ");
      Serial.print(pwmval->type, DEC);
      Serial.print(" for pin ");
      Serial.println(pins[i], DEC);
      continue;
    }

    Serial.print("setting pin ");
    Serial.print(pins[i], DEC);
    Serial.print(" to value ");
    Serial.println(pwmval->valueint, DEC);
    analogWrite(pins[i], pwmval->valueint);
  }
}







void flashLed(int pin, int times, int wait) {

  for (int i = 0; i < times; i++) {
    digitalWrite(pin, HIGH);
    delay(wait);
    digitalWrite(pin, LOW);

    if (i + 1 < times) {
      delay(wait);
    }
  }
}


void setup(void)
{
  lcd.begin(16, 2);
  pinMode(contrastPIN,OUTPUT);
  pinMode(A0, INPUT);
  pinMode(A1, OUTPUT);
  
  
  pinMode(redLEDPin, OUTPUT);
  pinMode(greenLEDPin, OUTPUT);
  pinMode(blueLEDPin, OUTPUT);
  
  
  //pinMode(pizeoPin, OUTPUT);
  
  
  pinMode(buttonPin, INPUT);
  
  
  
  
  pinMode(tempPin, INPUT);
  
  
  
  Serial.begin(9600);
  xbee.setSerial(nss);
  nss.begin(9600);
     
     
     
  // Print a message to the LCD.
  lcd.print("SquatBox");
  // set the cursor to column 0, line 1
  // line 1 is the second row, since counting begins with 0
  lcd.setCursor(0, 1);
  
  
//  // print to the second l.
//    String json_String= aJson.print(root);
//    json_String.replace("{", "<<");
//    json_String.replace("}", ">>");
// 
// 
// 
//    int stringLength = json_String.length();   
//    uint8_t sendPayload[stringLength];
//    
//
//
//    for (int i = 0; i < stringLength; i++){
//        sendPayload[i] = json_String[i];
//    }
//  
//    
//    ZBTxRequest zbTx = ZBTxRequest(addr64, sendPayload, sizeof(sendPayload));
//
//    
//    xbee.send(zbTx);
  
  
  
 
}



void loop(void)
{
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  buttonState = digitalRead(buttonPin);
  
  if (buttonState == HIGH){
    Serial.println("button press!");  
    tone(pizeoPin, 200, 200);
    
    lcd.clear();
    lcd.print("Button press!");
    
    
    
    aJsonObject *sendTest;
    sendTest = aJson.createObject();
    aJson.addItemToObject(sendTest, "name", aJson.createItem("hello world!"));
  
    String json_String= aJson.print(sendTest);
    
    
    
    json_String.replace("{", "<<");
    json_String.replace("}", ">>");
 
 
 
    int stringLength = json_String.length();   
    uint8_t sendPayload[stringLength];
//    
//
//
    for (int i = 0; i < stringLength; i++){
        sendPayload[i] = json_String[i];
    }
//  
//    
    ZBTxRequest zbTx = ZBTxRequest(addr64, sendPayload, sizeof(sendPayload));
//
//    
    xbee.send(zbTx);    
    
    
    aJson.deleteItem(sendTest);    
    
    
    if (xbee.readPacket(500)) {
        // got a response!
    
        // should be a znet tx status            	
        if (xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) {
          xbee.getResponse().getZBTxStatusResponse(txStatus);
    
          // get the delivery status, the fifth byte
          if (txStatus.getDeliveryStatus() == SUCCESS) {
            // success.  time to celebrate
            flashLed(greenL, 5, 50);
          } else {
            // the remote XBee did not receive our packet. is it powered on?
            flashLed(redLEDPin, 3, 500);
          }
        }
      } else if (xbee.getResponse().isError()) {
        //nss.print("Error reading packet.  Error code: ");  
        //nss.println(xbee.getResponse().getErrorCode());
      } else {
        // local XBee did not provide a timely TX Status Response -- should not happen
        flashLed(redLEDPin, 2, 50);
      }
    
      delay(1000);
  }
 
  //digitalWrite(blueLEDPin, HIGH);
  
  
  
  
  
  
   //getting the voltage reading from the temperature sensor
   int reading = analogRead(tempPin);  
   
   // converting that reading to voltage, for 3.3v arduino use 3.3
   float voltage = reading * 5.0;
   voltage /= 1024.0; 
   
   // print out the voltage
   //Serial.print(voltage); Serial.println(" volts");
   
   // now print out the temperature
   float temperatureC = (voltage - 0.5) * 100 ;  //converting from 10 mv per degree wit 500 mV offset
                                                 //to degrees ((voltage - 500mV) times 100)
   //Serial.print(temperatureC); Serial.println(" degrees C");
   
   // now convert to Fahrenheit
   float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;
   //Serial.print(temperatureF); Serial.println(" degrees F");
   
   delay(1000);                                     //waiting a second
  
  
  
  
  
  
  
  
  xbee.readPacket();
    if (xbee.getResponse().isAvailable()) {
        
         
      
      if(xbee.getResponse().getApiId() == ZB_RX_RESPONSE){
          
        tone(pizeoPin, 400, 200);
        lcd.clear();
         // lcd.clear();
          
        //lcd.setCursor(0, 0);
//       
       // lcd.print("Received Data!");
//        lcd.setCursor(0, 1);
//        
//        Serial.println("got data!");
//        digitalWrite(ledPin, HIGH);
//        
//        delay(500);
//        
//        digitalWrite(ledPin, LOW);
        
        
      
        switch (xbee.getResponse().getApiId()) {             //what kind of packet was received
            case ZB_RX_RESPONSE:                             //rx data packet
                xbee.getResponse().getZBRxResponse(zbRX);    //get the received data
                

                char payload[zbRX.getDataLength()];
                
                                
                for (int i = 0; i < zbRX.getDataLength(); i++){
                
                  payload[i] = char(zbRX.getData(i));

                  
                  //Serial.println(payload[i]);
              
               }
               
               
               
               
               //snprintf(payload,sizeof(payload),"Value=%d Value2=%X",123,456);
               
               //Serial.println(payload);
     
               //String test = String(payload);
               
               //Serial.println(test);
               
               //aJsonObject* root = aJson.parse(payload);
               //aJsonObject* name = aJson.getObjectItem(root, "name");
               //Serial.println(name->valuestring);
                 
                  String temp = payload;
                  temp.replace("<<", "{");
                  temp.replace(">>", "}");
                  
                  
                  
                  int length = temp.length();
                  
                  //Serial.print(length);
                  //Serial.print(temp);
                  
                  char receivedData[length];
                 
                  temp.toCharArray(receivedData, length + 1);
                  

                  
                  
                  
                

                  
                  aJsonObject* root =  aJson.parse(receivedData);
                  
                  aJsonObject* booked = aJson.getObjectItem(root, "booked"); 
                  aJsonObject* name = aJson.getObjectItem(root, "name");
                  
                  
                  
                  
                  //lcd.clear();
                  
                  
                  lcd.print(name->valuestring);
                  
                  boolean bookedState = (booked->valuebool);
                  
                  Serial.println(bookedState);
//                  
//                  
                    if (bookedState == 255){
                    digitalWrite(greenLEDPin, LOW);
                    digitalWrite(blueLEDPin, HIGH);
                    
                    digitalWrite(redLEDPin, LOW);
                  } else if (bookedState == 0){
                    
                    digitalWrite(greenLEDPin, HIGH);
                    digitalWrite(blueLEDPin, LOW);
                    
                    digitalWrite(redLEDPin, LOW);
                  } else {
                    digitalWrite(blueLEDPin, HIGH);
                    digitalWrite(redLEDPin, LOW);
                    digitalWrite(greenLEDPin, LOW);
                 }
                 
                  aJson.deleteItem(root);
//                   
                
//                if ((data == 1) || (data == '1')){
//                   
//                  //Serial.print("equals 1");
//                  
//                   for (int i=1; i < zbRX.getDataLength(); i++){
//                     
//                     payload[i] = zbRX.getData(i);
//                     
//                     if ((payload[i]== 60) || (payload[i] == '60')){
//                       Serial.print('<');
//                     } else if ((payload[i]== 62) || (payload[i] == '62')){
//                       Serial.print('>');
//                     } else {
//                       Serial.print(payload[i]);
//                     }
//                     
//                     delay(50);
//                     
//                     
//                     
////                     Serial.print (startOfNumberDelimiter);    
////                      Serial.print (255);
////                      Serial.print (endOfNumberDelimiter);  
////                      
////                      
////                      Serial.print (startOfNumberDelimiter);    
////                      Serial.print (255);
////                      Serial.print (endOfNumberDelimiter);  
////                      
////                      
////                      Serial.print (startOfNumberDelimiter);    
////                      Serial.print (255);
////                      Serial.print (endOfNumberDelimiter);
//                     
//                   }               
//                  
//                  
//                   digitalWrite(9, HIGH);
//                   
//                   delay(500);
//                   
//                   digitalWrite(9, LOW);
//                   
//                   delay(50);
//                  
//                   //sendPayload[1] = '1';
//                   
//                   xbee.send(zbTx);
//
//                    // flash TX indicator
//                    flashLed(statusLed, 1, 100);
//                  
//                    // after sending a tx request, we expect a status response
//                    // wait up to half second for the status response
//                    if (xbee.readPacket(500)) {
//                      // got a response!
//                  
//                      // should be a znet tx status               
//                      if (xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) {
//                        xbee.getResponse().getZBTxStatusResponse(txStatus);
//                  
//                        // get the delivery status, the fifth byte
//                        if (txStatus.getDeliveryStatus() == SUCCESS) {
//                          // success.  time to celebrate
//                          digitalWrite(9, HIGH);
//                          delay(300);
//                          digitalWrite(9, LOW);
//                           delay(300);
//                          digitalWrite(9, HIGH);
//                          delay(300);
//                          digitalWrite(9, LOW);
//                          
//                          flashLed(statusLed, 5, 50);
//                        } else {
//                          // the remote XBee did not receive our packet. is it powered on?
//                          flashLed(errorLed, 3, 500);
//                        }
//                      }
//                    } else if (xbee.getResponse().isError()) {
//                      //nss.print("Error reading packet.  Error code: ");  
//                      //nss.println(xbee.getResponse().getErrorCode());
//                    } else {
//                      // local XBee did not provide a timely TX Status Response -- should not happen
//                      flashLed(errorLed, 2, 50);
//                    }
//                } else {
//                   digitalWrite(10, HIGH);
//                   
//                   delay(500);
//                   
//                   digitalWrite(10, LOW);
//                }
//                
//                switch (zbRX.getOption()) {
//                    case ZB_PACKET_ACKNOWLEDGED:
//                        dataLen = zbRX.getDataLength();
//                        senderAddr = zbRX.getRemoteAddress64();
//                        break;
//                }
//                break;
        }
        
      }
    }
}
