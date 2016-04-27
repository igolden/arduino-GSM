/*
*/

// Run this script without the void and presetup with destination.

// dependencies
#include "string.h"
#include "ctype.h"
#include "SoftwareSerial.h"
#include "dGPS.h"
#include "client.h"

// Software serial TX & RX Pins for the GPS module
// Initiate the software serial connection

int ledPin = 13;                  // LED test pin

// This is why Paul saw the 0,0
// We need to set the lat/long manually starting, or get rid of the destination lat/long completely.
// * note this code just setups of the serial questions and input.

float desLat=0;                   //Destination Latitude filled by user in Serial Monitor Box
float desLon=0;                   //Destination Longitude filled by user in Serial Monitor Box
char fla[2];                      //flag (Y/N) whether to print checksum or not. Filled by user in Serial Monitor Box
char fla2[2];                     //flag (Y/N) whether to print Altitude, number of satellites used and HDOP. Filled by user in Serial Monitor Box
dGPS dgps = dGPS();               // Construct dGPS class


 // This function reads the input from above ^^

 void getflag(char *str)
   // function to read the flag character from the user 
  {  
    while (Serial.available()==0)
    {;}
    int index=0;
    if (Serial.available()>0)
    { if (index<2){
      str[index]=Serial.read();
      index++;
     }
      else str[index]='\0';
      delay(10);
    }  
  }

// Start the setup


void setup() {
  pinMode(ledPin, OUTPUT);       // Initialize LED pin
  Serial.end();                  // Close any previously established connections
  Serial.begin(9600);            // Serial output back to computer.  On.
  dgps.init();                   // Run initialization routine for dGPS.

  // is this delay necessary?
  delay(1000);  

  // I think the below question can be removed.

  // Ask to clear buffer
  *fla2=Serial.read();           // To clear rhe buffer before the actual flag value is read
  memset(fla2, 0, sizeof(fla2));
  Serial.print("Do you want to display Altitude, Satellites used and HDOP (Y/N): "); 
  delay(3000);
  getflag(fla2);
  Serial.println(*fla2);
  
}

void loop() {
  
  // This is where the GPS action is.
  dgps.update(desLat, desLon);    // Calling this updates the GPS data.  The data in dGPS variables stays the same unless
  Serial.print("Status: ");
  Serial.println(dgps.Status());  // A - Satellites acquired and a valid signal.  V - No sats and not a valid signal.
  
  Serial.print("Latitude: ");
  Serial.print(dgps.Lat(), 6);    // Lattitude - in DD.MMSSSS format (decimal-degrees format)  (D-Degree; M-Minute; S-Second)
  Serial.println(" degrees");
  
  Serial.print("Longitude: ");
  Serial.print(dgps.Lon(), 6);    // Longitude - in DD.MMSSSS format (decimal-degrees format)  (D-Degree; M-Minute; S-Second)
  Serial.println(" degrees");
  
  // can get rid of the velocity, becuase we are updating every 5 seconds, unless we want to display the speed of the movement.
  
  Serial.print("Velocity: ");
  Serial.print(dgps.Vel(), 6);    // Velocity, in knots.
  Serial.println(" knots");
  
  // can probably comment azimuth out.
  
  Serial.print("Azimuth to destination: "); 
  Serial.print(dgps.Azim());      //Azimuth of the destination coordinates from the current location in degrees. Correct upto 2 decimal points
  Serial.println(" degrees");
  

  // This needs to be moved up above, the mode may be crucial, not sure.

  Serial.print("Mode Indicator: "); 
  Serial.println(dgps.Mode());     //Mode Indicator (N-Data not valid,A-Autonomous mode,D-Differential mode,E-Estimated mode,M-Manual input mode,S-Simulator mode

// more user input... gotta make it all setup to run without user input.

    switch(*fla){                 //SWITCH CASE TO CHECK IF THE USER WANTS CHECKSUM OR NOT
     case 'Y':
     case 'y':
       Serial.print("Received CheckSum: ");
       Serial.println(dgps.Checks()); //Checksum received from packet
       Serial.print("Computed Checksum: ");
       Serial.println(dgps.Checked(),HEX); //Checksum computed
       break;
     case 'N':
     case 'n':
       Serial.print("");
       break;
     default: Serial.print(""); }
     
     switch(*fla2){               //SWITCH CASE TO CHECK IF THE USER WANTS Number Of Satellites, HDOP, and Altitude OR NOT
     case 'Y':
     case 'y':
       dgps.updategga();         //updates the values of Number of Satellites, HDOP and Altitude 
       Serial.print("Number of Satellites in use: ");
       Serial.println(dgps.SatView()); // Number of Satellites in use
       Serial.print("HDOP: ");
       Serial.println(dgps.Hdop());    // HDOP
       Serial.print("Altitude: ");
       Serial.print(dgps.Alti());      // Altitude (in meters) above sea-level
       Serial.println(" meters above sea level");
       break;
     case 'N':
     case 'n':
       Serial.print("");
       break;
     default: Serial.print(""); }
       
  // I added the value here to test it out.
  Serial.println("We're Done, ya bastard."); 
}
