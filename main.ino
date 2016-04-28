// GSM Dependencies
#include "SIM900.h"
#include <SoftwareSerial.h>
#include "inetGSM.h"

// dGPS dependencies
#include "string.h"
#include "ctype.h"
#include "SoftwareSerial.h"
#include "dGPS.h"
#include "client.h"


// The setup of the GSM
InetGSM inet;


// SETUP

void setup()
{
     //Serial connection.
     Serial.begin(9600);
     Serial.println("GSM Shield testing.");
     //Start configuration of shield with baudrate.
     //For http uses is raccomanded to use 4800 or slower.
     if (gsm.begin(2400)) {
          Serial.println("\nstatus=READY");
          started=true;
     } else Serial.println("\nstatus=IDLE");

     if(started) {
          //GPRS attach, put in order APN, username and password.
          //If no needed auth let them blank.
          if (inet.attachGPRS("internet.wind", "", ""))
               Serial.println("status=ATTACHED");
          else Serial.println("status=ERROR");
          delay(1000);

          //Read IP address.
          gsm.SimpleWriteln("AT+CIFSR");
          delay(5000);
          //Read until serial buffer is empty.
          gsm.WhileSimpleRead();


          // Set Variables
          char latitude[] = dgps.Lat();
          char longitude[] = dgps.Lon();
          char velocity[] = dgps.Vel();
          char azimuth[] = dgps.Azim();

          //TCP Client GET, send a GET request to the server and
          //save the reply.
          numdata=inet.httpGET("stickytrack.com", 80, "/handle_data.php", msg, 50);
          //Print the results.
          Serial.println("\nNumber of data received:");
          Serial.println(numdata);
          Serial.println("\nData received:");
          Serial.println(msg);
// END GSM SETUP

// START dGPS setup
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
// END dGPS setup
  

};

// LOOP

void loop()
{
// GSM LOOP
     //Read for new byte on serial hardware,
     //and write them on NewSoftSerial.
     serialhwread();
     //Read for new byte on NewSoftSerial.
     serialswread();
// END GSM LOOP
// START dGPS LOOP
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

// END dGPS LOOP

}

// EXTERNAL CLIENT FUNCTIONS
//    These functions essentially read the request, if it werwe s
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
