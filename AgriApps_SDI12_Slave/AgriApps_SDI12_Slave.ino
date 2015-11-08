#include <SDI12.h>

// ------ SETTINGS ------

#define DATAPIN 4 

String sdiVersion = "13";
String companyName = "AGRIAPPS";
String sensorModelNumber = "100000";
String sensorVersion = "1.2";
String serialNumber = "101";

// ------ END SETTINGS ------

SDI12 mySDI12(DATAPIN, 1); 

String commandReceived = "";
char currentAddress = '0';
int currentReadingDelay = 2;    // Default to 2 seconds


void setup(){
  
  Serial.begin(9600);
  Serial.println("SDI-12 sensor");

  delay(2000);
  
  mySDI12.begin(); 
  
  
  delay(500); // allow things to settle
  
}

void loop(){
  
  int avail = mySDI12.available();

  if(avail < 0){
    
    Serial.println("ERROR: We have had a buffer overflow, flushing.");
    mySDI12.flush();
    
  }else if(avail > 0){

    for(int a = 0; a < avail; a++){
      
      char charReceived = mySDI12.read(); 
  
      if (charReceived == '!'){  

        Serial.println("Command Received: " + commandReceived + String(charReceived));

        parseCommand(commandReceived);
        
        commandReceived = "";
 
        
      }else{
        commandReceived += String(charReceived);
      }
    }
  } 
  
}


void sendResponse(String response){
      
  sendResponseUsingAddress(currentAddress, response);

}

void sendResponseUsingAddress(char address, String response){

  //response += "\r\n";
  
  Serial.println("Sending Response: " + response);
  mySDI12.sendCommand(response);

}





void parseCommand(String command){

  // First char of command is always the address of the device being probed

  // ---- FROM SDI-12 SPEC ----
  // ASCII '0' through ASCII '9' are the standard addresses which all sensors and data
  // recorders must support. Should there be a need for more than 10 sensors, use an address in the
  // range ASCII 'A' through ASCII 'Z' (decimal 65 through 90) and ASCII 'a' through ASCII 'z'
  // (decimal 97 through 122). 
  //
  // It can also be a ? if Master is querying the device's address
  //
  // So possible addresses are 0-9, a-z, A-Z, and ?

  currentAddress = command.charAt(0);

  Serial.println("Current address set to: " + currentAddress);
  
  if(command.length() == 1){

    // ----- This is Acknowledge Active request -----
    Serial.println("Received an Acknowledge Active command");
    acknowdgeActive();
    
  }else{

    String action = command.substring(1);
    
  
    if(action.charAt(0) == 'I'){
      // ----- This is a Send Identification action -----
      sendIdentification();
      
    }else if(action.charAt(0) == 'A'){
      // ----- This is a Change Address action -----
      
      changeAddress(action.charAt(1));
      
    }else if(action.charAt(0) == '?'){
      // ----- This is and Address Query action -----
      addressQuery();
      
    }else if(action.charAt(0) == 'M'){
      // ----- This is Start Measurement action -----
      startMeasurement();
      
    }else if(action.charAt(0) == 'D'){
      // ----- This is Send Data action -----
      sendDataTwo(int(action.charAt(1)));
      
    }else if(action.charAt(0) == 'V'){
      // ----- This is Start Verification action -----
      // TODO
      
    }else if(action.charAt(0) == 'C'){
      // ----- This is Start Concurrent Measurement action -----
      // TODO
      
    }else if(action.charAt(0) == 'R'){
      // ----- This is a Continuous Measurements action -----
      // TODO
      
    }else{
      // Unknown request
      Serial.println("ERROR: Unknown command");
    }
  }
  
}
    

void acknowdgeActive(){

    // ------ Acknowledge Active ------
    Serial.println("Sending Acknowledge Active response, current address: " + String(currentAddress));
    sendResponse("123123");
      

}

void sendIdentification(){
  
  // ------ Send Identification ------
      
  String response = "" + sdiVersion + companyName + sensorModelNumber + sensorVersion + serialNumber;
  Serial.println("sendIdentification: " + response);
  
  sendResponse(response);
  
}


void changeAddress(char newAddress){
  
    // ------ Change Address ------
    // TODO - Actually swap the addresses

    sendResponseUsingAddress(newAddress, "");

}

void addressQuery(){
  // ------- Address Query -------
  Serial.println("addressQuery");
  sendResponse("");
}


void startMeasurement(){
    // ------ Start Measurement -------
    // ttt - number of seconds till measurement is ready
    // d - number the number of measurement values the sensor will make and return
    // in one or more subsequent D commands; n is a single digit integer with
    // a valid range of 1 to 9

    // TODO - Defaulting to 2 seconds

    String response = "";
  
    currentReadingDelay = 2;
    int d = 0;

    if(currentReadingDelay < 10){
      response = "00" + currentReadingDelay;
    }else if(currentReadingDelay >= 10 && currentReadingDelay < 100){
      response = "0" + currentReadingDelay;
    }else if (currentReadingDelay >= 100){
      response = "" + currentReadingDelay;
    }else{
      Serial.println("ERROR: Invalid ttt for sensor " + currentAddress);
    }

    response = response + d;
    Serial.println("startMeasurement: " + response);
    sendResponse(response);

    

}

void startMeasurementWithCRC(){
    // ------ Start Measurement and Request CRC ------
    // TO BE IMPLEMENTED
    
}
 
    

void sendDataTwo(int measurementNumber){

  // ------- Send Data ------

  String dataToSend = "{'data': {'sensorType': 'atmospheric-temperature', 'value': 25.00}}";
  Serial.println("sendData: " + dataToSend);
  sendResponse(dataToSend);

}
 
void additionalMeasurements(){

  // ------ Additional Measurements ------
  // TODO
 
}


