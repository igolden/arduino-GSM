// Possible solutions:
//  CIP SEND reconfiguratoin
//  HTTP Send Params // GET Commands // POST Commands
// https://www.youtube.com/watch?v=QsNcfxc8YSA&feature=youtu.be
#include "SIM900.h"
#include <SoftwareSerial.h>
#include "inetGSM.h"
//#include "sms.h"
//#include "call.h"

//To change pins for Software Serial, use the two lines in GSM.cpp.

//GSM Shield for Arduino
//www.open-electronics.org
//this code is based on the example of Arduino Labs.

//Simple sketch to start a connection as client.

InetGSM inet;
//CallGSM call;
//SMSGSM sms;

char msg[50];
char postdata;
int numdata;
char inSerial[50];
int i=0;
boolean started=false;

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
    if (inet.attachGPRS("broadband", "", ""))
      Serial.println("status=ATTACHED");
    else Serial.println("status=ERROR");
    delay(1000);

    //Read IP address.
    //gsm.SimpleWriteln("AT+CIFSR");
//    delay(5000);
    //Read until serial buffer is empty.
   // gsm.WhileSimpleRead();
//delay(2500);

 //   postdata=inet.httpPOST("f8422906.ngrok.io", 80, "/handle_data.php", "?lat=309&lon=563", msg, 50);
//    sendCoordinates();
  }
}



  //Read for new byte on serial hardware,
  //and write them on NewSoftSerial.
  //serialhwread();
  //Read for new byte on NewSoftSerial.
  //serialswread();


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
void sendCoordinates()
{


 //Serial.println("\nStarting the TCP Connection");
 gsm.SimpleWriteln("AT+CIPSTART=\"TCP\",\"38dd48c6.ngrok.io\",\"80\"");  //Make tcp connection to server using default TCP and HTTP port
 gsm.SimpleRead();
delay(2000);

//Serial.println("\nStarting HTTP Commands");
gsm.SimpleWriteln("AT+HTTPINIT");
gsm.SimpleRead();
gsm.SimpleWriteln("AT+HTTPPARA=\"CID\",1");
gsm.SimpleRead();
delay(1000);
gsm.SimpleWriteln("AT+HTTPPARA=\"URL\",\"http://0095f515.ngrok.io/handle_data.php?lat=309&lon=309:80\"");
gsm.SimpleRead();
delay(1000);
gsm.SimpleWriteln("AT+HTTPACTION=0");
gsm.WhileSimpleRead();
delay(1000);
//gsm.SimpleWriteln("AT+CIPSEND=");  // (1360 is max length or maybe min length ) Allows you to make a request to the server, but formatted differently depending on if it is a GET, PUT, or POST request
//gsm.SimpleWrite("GET /handle_data.php/lat=309&lon=563");
//gsm.SimpleWrite("Host: 38dd48c6.ngrok.io");
//gsm.SimpleWrite("Connection: keep-alive");
//gsm.SimpleWrite(0x1A);
//gsm.SimpleWrite("");
//gsm.SimpleRead();
//delay(1000);


///////all this below is unnecessary

// Serial.println("\nPrompt Comment Goes Here..");
//gsm.SimpleWriteln("AT"); //This just tests the modem
//gsm.SimpleWrite("");
//gsm.SimpleRead();
//delay(1000);

// Serial.println("\nPrompt Comment Goes Here..");
// gsm.SimpleWriteln("AT+CREG?");  //Tells us the sim is ready and has connected to the network
// gsm.SimpleWrite("");
// gsm.SimpleRead();
//delay(1000);

// Serial.println("\nPrompt Comment Goes Here..");
// gsm.SimpleWriteln("AT+CGATT?");  //Check if sim has internet access
//gsm.SimpleWrite("");
//gsm.SimpleRead();
//delay(1000);

// Serial.println("\nPrompt Comment Goes Here..");
// gsm.SimpleWriteln("AT+CIPSTATUS");  //Query IP settings
// gsm.SimpleWrite("");
//gsm.SimpleRead();
//delay(1000);

// Serial.println("\nPrompt Comment Goes Here..");
// gsm.SimpleWriteln("AT+CIPMUX=0");  //configure modem to make single port open for connections)
//gsm.SimpleWrite("");
// gsm.SimpleRead();
//delay(1000);

// Commented out beecause you're already connected to internet at this point. See command on line 45
// Serial.println("\nPrompt Comment Goes Here..");
// gsm.SimpleWriteln("AT+CSTT="); //connect to internet, not sure how to set this up, they say "APN", "username", "password" but that doesn't work for me
//  gsm.SimpleWrite("broadband");
//  gsm.SimpleRead();
//delay(1000);

// Serial.println("\nPrompt Comment Goes Here..");
// Serial.println("is active?\n");
// gsm.SimpleWriteln("AT+CIICR");  //Brings up wireless network and ensure that a data plan is active
//gsm.SimpleWrite("");
//gsm.SimpleRead();
//delay(1000);



// Serial.println("\nPrompt Comment Goes Here..");
//  gsm.SimpleWriteln("AT+CIPSHUT");  //This closes TCP connections, for some reason is needed
//  gsm.SimpleWrite("");
//  gsm.SimpleRead();
//delay(1000);







// Serial.println("\nPrompt Comment Goes Here..");
// gsm.SimpleWriteln("AT+CMGF=1"); //This command is used to change from PDU to text mode
//  gsm.SimpleWrite("");
//  gsm.SimpleRead();
//delay(1000);

// Serial.println("\nPrompt Comment Goes Here..");
// gsm.SimpleWriteln("AT+CSMP=17,167,0,16"); //This command sets the text mode parameters
// gsm.SimpleWrite("");
//gsm.SimpleRead();
//delay(1000);


// Serial.println("\nPrompt Comment Goes Here..");
//gsm.SimpleWriteln("AT+CMGS=<3097372866>[,<129>]<CR>Hello<ctrl-Z/ESC>");  //This command sends the SMS 
//gsm.SimpleWrite("");
//gsm.SimpleRead();
//delay(1000);





}
