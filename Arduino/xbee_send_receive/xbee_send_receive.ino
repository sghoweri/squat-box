#include <ButtonEvent.h>

// Libraries required for the luminosity sensor
//#include <Wire.h>



#include <aJSON.h>            // JSON Library

#include <XBee.h>             // XBee Library
#include <SoftwareSerial.h>


#include <LiquidCrystal.h>


aJsonObject *sendTest;
aJsonObject *xbeeDataReceived;


/*** SETUP LCD DISPLAY ***/

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 7, 8, 2, 10);
const int contrastPIN = 4;


String json_String;


XBee xbee = XBee();                    //instantiate the XBee object


XBeeAddress64 addr64 = XBeeAddress64(0x0013A200, 0x40A1A14E);
//ZBTxRequest zbTx = ZBTxRequest(addr64, sendPayload, sizeof(sendPayload));
ZBRxResponse zbRX = ZBRxResponse();    //XBee RX packet

ModemStatusResponse msr = ModemStatusResponse();
ZBTxStatusResponse txStatus = ZBTxStatusResponse();


uint8_t ssRX = A0;
uint8_t ssTX = A1;
















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

String temp;







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


void setup(void){
  lcd.begin(16, 2);
  pinMode(contrastPIN,OUTPUT);
  pinMode(A0, INPUT);
  pinMode(A1, OUTPUT);
  
  

  pinMode(redLEDPin, OUTPUT);
  pinMode(greenLEDPin, OUTPUT);
  pinMode(blueLEDPin, OUTPUT);


  //pinMode(pizeoPin, OUTPUT);


  //pinMode(buttonPin, INPUT);




  pinMode(tempPin, INPUT);
  


  Serial.begin(9600);
  xbee.setSerial(nss);
  
  nss.begin(9600);



  // Print a message to the LCD.
  lcd.print("SquatBox");
  // set the cursor to column 0, line 1
  // line 1 is the second row, since counting begins with 0
  lcd.setCursor(0, 1);

   ButtonEvent.addButton(A3,       //button pin
                        onDown,   //onDown event function
                        NULL,     //onUp event function
                        NULL,   //onHold event function
                        NULL,     //hold time in milliseconds
                        NULL, //onDouble event function
                        NULL);     //double time interval
                        

}




                

        





void loop(){
  
  ButtonEvent.loop();  
  xbee.readPacket();
  
  



 


      

        
        if (xbee.getResponse().isAvailable()) {
          initialize();
          
          
        } else {
        
          
          
          xbee.getResponse().isError(); 
          switch(xbee.getResponse().getErrorCode()) { 
          case CHECKSUM_FAILURE: 
            Serial.println("CHECKSUM_FAILURE");
            break; 
          case PACKET_EXCEEDS_BYTE_ARRAY_LENGTH: 
            //XBErrCount++; 
             Serial.println("PACKET_EXCEEDS_BYTE_ARRAY_LENGTH");
            break; 
          case UNEXPECTED_START_BYTE:
            Serial.println("UNEXPECTED_START_BYTE");
            break; 
          } 
        }
        

      
        

      
}


void onDown(ButtonInformation* Sender) {

    
    sendTest = aJson.createObject();
    
  
    //Serial.println("button press!");
    tone(pizeoPin, 200, 200);

    lcd.clear();
    lcd.print("Button press!");



		
    aJson.addItemToObject(sendTest, "name", aJson.createItem("hello world!"));
    json_String = aJson.print(sendTest);
      
    
    
    
    

    json_String.replace("{", "<<");
    json_String.replace("}", ">>");
    
    
    

    int stringLength = json_String.length();
    uint8_t sendPayload[stringLength];

    for (int i = 0; i < stringLength; i++){
      sendPayload[i] = json_String[i];
    }
    
    
   

    ZBTxRequest zbTx = ZBTxRequest(addr64, sendPayload, sizeof(sendPayload));
    
    
    xbee.send(zbTx);
    
    
    
    //REALLY IMPORTANT: need to delete the "object" (it's not really an object) created by aJson from memory AND delete the JSON string created in order to stop a memory leak!!
    free(sendPayload);
    aJson.deleteItem(sendTest);
    
    //json_String = "";
    //
    

}




void initialize(){
  
        //Serial.println("receiving data!");
       
    if (xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) {
      
      xbee.getResponse().getZBTxStatusResponse(txStatus);
      
      // get the delivery status, the fifth byte
      if (txStatus.getDeliveryStatus() == SUCCESS) {
		          // success.  time to celebrate
                      
                      
                      delay(200);
                      
		        	flashLed(greenLEDPin, 5, 50);

                      tone(pizeoPin, 400, 200);
                      delay(100);
                      tone(pizeoPin, 500, 200);
                      delay(100);
                      tone(pizeoPin, 600, 200);
                      delay(300);
                      tone(pizeoPin, 800, 400);
            }
    } else if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
       tone(pizeoPin, 400, 200);
                        lcd.clear();
                        
                        

                        
                        

		        // now fill our zb rx class
		       	xbee.getResponse().getZBRxResponse(zbRX);
                        
                        
                        
                        
                        
		       	char payload[zbRX.getDataLength()];


			   for (int i = 0; i < zbRX.getDataLength(); i++){
				  payload[i] = char(zbRX.getData(i));
			   }


				temp = payload;


				temp.replace("<<", "{");
				temp.replace(">>", "}");



				int length = temp.length();


				char receivedData[length];

				temp.toCharArray(receivedData, length + 1);




				xbeeDataReceived =  aJson.parse(receivedData);
                                
                                
				aJsonObject* booked = aJson.getObjectItem(xbeeDataReceived, "booked");
                                boolean bookedState = (booked->valuebool);

				
                                aJsonObject* name = aJson.getObjectItem(xbeeDataReceived, "name");
                                lcd.print(name->valuestring);
                                
                                
                                
                                
//

//
				//Serial.println(bookedState);
                                
                                

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
          
        free(payload);
        aJson.deleteItem(xbeeDataReceived);

            

            
            
              

    } else {
        flashLed(redLEDPin, 2, 50);
    }
        
}

