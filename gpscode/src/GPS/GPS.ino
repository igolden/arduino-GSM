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


// Set the variables
float devLat=0;
float devLon=0;

dGPS dgps = dGPS();               // Construct dGPS class




void setup() {
  pinMode(ledPin, OUTPUT);       // Initialize LED pin
  Serial.end();                  // Close any previously established connections
  Serial.begin(9600);            // Serial output back to computer.  On.
  dgps.init();                   // Run initialization routine for dGPS.

  // is this delay necessary?
  delay(1000);  

  // I think the below question can be removed.

}

void loop() {
  
  // This is where the GPS action is.
  dgps.update(devLat, devLon);    // Calling this updates the GPS data.  The data in dGPS variables stays the same unless
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
//put this here for test input
  Serial.println("Loop is completed."); 
}
