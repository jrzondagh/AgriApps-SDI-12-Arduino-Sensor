#include <SDI12.h>

#define DATAPIN 4         // change to the proper pin
SDI12 mySDI12(DATAPIN, 0); 


void setup(){
  Serial.begin(9600); 
  Serial.println("SDI-12 Master Test - Acknowledge Active");
  
  mySDI12.begin(); 
  delay(500); // allow things to settle down
  Serial.println("Setup complete");
}

void loop(){

  checkActive('a', 1);
  delay(5000); 
}


void printBufferToScreen(){
  Serial.println("Printing buffer");
  String buffer = "";
  mySDI12.read(); // consume address
  while(mySDI12.available()){
    char c = mySDI12.read();
    if(c == '+' || c == '-'){
      buffer += ',';   
      if(c == '-') buffer += '-'; 
    } 
    else {
      buffer += c;  
    }
    delay(100); 
  }
 Serial.print(buffer);
}


boolean checkActive(char address, int attempts){              

  Serial.println("Checking if [" + String(address) + "] is active");
  
  String command = "" + String(address) + "!";

  Serial.println("Sending command: " + command);

  mySDI12.sendCommand(command);

 // delay(100);

  printBufferToScreen();


    
  String response = "";
  
  for(int m = 0; m < 1000; m++){

    int avail = mySDI12.available();
  
    if(avail < 0){
      
      Serial.println("ERROR: We have had a buffer overflow, flushing.");
      mySDI12.flush();
      
    }else if(avail > 0){
  
      for(int a = 0; a < avail; a++){
        
        char charReceived = mySDI12.read(); 
    
        if (charReceived == '\n'){  
          response += String(charReceived);
          Serial.println("Response Received: " + response + String(charReceived));
          break;
          
        }else{
          response += String(charReceived);
        }
      }
    }
    
    
    delayMicroseconds(1000);
  }
  Serial.println("Response: " + response);
  
  return false; 
}


