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
    gsm.SimpleWriteln("AT+CIFSR");
//    delay(5000);
    //Read until serial buffer is empty.
    gsm.WhileSimpleRead();

 //   postdata=inet.httpPOST("f8422906.ngrok.io", 80, "/handle_data.php", "?lat=309&lon=563", msg, 50);
    sendCoordinates();
    Serial.println(postdata);
  }
};

void loop()
{
  //Read for new byte on serial hardware,
  //and write them on NewSoftSerial.
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
void sendCoordinates()
{
  gsm.SimpleWriteln("AT+CGATT?");   //Attach or Detach from GPRS Service (Result  1 = Attach , 2 = Detached )
  delay(300); 
  gsm.WhileSimpleRead();

  gsm.SimpleWriteln("AT+CIPSHUT=0");  //Close TCP Connection
  delay(300); 
  gsm.WhileSimpleRead();

  Serial.println("Broadband connections..");
  gsm.SimpleWriteln("AT+SAPBR=3,1,\"broadband\",\"internet\"");//setting the APN, the second need you fill in your local apn server
  delay(1000); 
  gsm.WhileSimpleRead();

  Serial.println("\nNew commands starting..");
  gsm.SimpleWriteln("AT+HTTPINIT");
  gsm.SimpleRead();
  gsm.WhileSimpleRead();
  gsm.SimpleWriteln("AT+HTTPPARA=\"CID\",1");
  gsm.SimpleRead();
  gsm.WhileSimpleRead();
  gsm.SimpleWriteln("AT+HTTPPARA=\"URL\",\"http://f8422906.ngrok.io/handle_data.php?lat=309&lon=563\"");
  gsm.SimpleRead();
  gsm.WhileSimpleRead();
  gsm.SimpleWriteln("AT+HTTPACTION=0");
  gsm.SimpleRead();
  gsm.WhileSimpleRead();
  gsm.SimpleWriteln("AT+HTTPREAD");
  gsm.SimpleRead();
  gsm.WhileSimpleRead();
  gsm.SimpleWriteln("AT+HTTPTERM");
  Serial.println("\nNew commands done..");


  gsm.SimpleWriteln("AT+CIPSHUT=0");
  delay(100); 
  gsm.SimpleRead();
  gsm.WhileSimpleRead();

}
