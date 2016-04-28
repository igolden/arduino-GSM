#include "SIM900.h"
#include <SoftwareSerial.h>
#include "inetGSM.h"
#include "string.h"
#include "ctype.h"
#include "dGPS.h"
#include "client.h"

// Init GSM
InetGSM inet;

//Serial Setup - Client
char msg[50];
int numdata;
char inSerial[50];
int i=0;
boolean started=false;

// Setup test pin GPS
int ledPin = 13;      

// Set the variables
float devLat=0;
float devLon=0;

// Construct the dGPS class
dGPS dgps = dGPS();  

void setup()
{
  // Init dGPS
  pinMode(ledPin, OUTPUT);      
  Serial.end();                
  Serial.begin(9600);         
  dgps.init();               



     //Serial connection.
     Serial.begin(9600);
     Serial.println("GSM Shield testing.");

     if (gsm.begin(2400)) {
          Serial.println("\nstatus=READY");
          started=true;
     } else Serial.println("\nstatus=IDLE");

     if(started) {
          if (inet.attachGPRS("broadband", "", ""))
               Serial.println("status=ATTACHED");
          else Serial.println("status=ERROR");

          Serial.println("starting connection...");

          //gsm.SimpleWriteln('AT+HTTPPARA=lat,41.948600');
          //gsm.SimpleWriteln('AT+HTTPPARA=lat,41.948600');


          // Make Request
          numdata=inet.httpPOST("http://33f1f4a7.ngrok.io", 80, "/handle_data.php?lat=41.948600&lon=-88.769332", msg, 500);
          //Print the results.
          Serial.println("\nNumber of data received:");
          Serial.println(numdata);
          Serial.println("\nData received:");
          Serial.println(msg);
     }
};

void loop()
{
     serialhwread();
     //Read for new byte on NewSoftSerial.
     serialswread();
};

void serialhwread()
{
     i=0;
     if (Serial.available() > 0) {
          while (Serial.available() > 0) {
               inSerial[i]=(Serial.read());
               delay(10);
               i++;
          }

          inSerial[i]='\0';
          if(!strcmp(inSerial,"/END")) {
               Serial.println("_");
               inSerial[0]=0x1a;
               inSerial[1]='\0';
               gsm.SimpleWriteln(inSerial);
          }
          //Send a saved AT command using serial port.
          if(!strcmp(inSerial,"TEST")) {
               Serial.println("SIGNAL QUALITY");
               gsm.SimpleWriteln("AT+CSQ");
          }
          //Read last message saved.
          if(!strcmp(inSerial,"MSG")) {
               Serial.println(msg);
          } else {
               Serial.println(inSerial);
               gsm.SimpleWriteln(inSerial);
          }
          inSerial[0]='\0';
     }
}

void serialswread()
{
     gsm.SimpleRead();
}
void setup() {
  pinMode(ledPin, OUTPUT);       // Initialize LED pin
  Serial.end();                  // Close any previously established connections
  Serial.begin(9600);            // Serial output back to computer.  On.
  dgps.init();                   // Run initialization routine for dGPS.
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
