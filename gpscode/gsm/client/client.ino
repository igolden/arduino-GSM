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

// this looks crucial

InetGSM inet;

//CallGSM call;
//SMSGSM sms;

// is this the serial asking for phone number input?

char msg[50];
int numdata;
char inSerial[50];
int i=0;
boolean started=false;

SoftwareSerial mySerial(10, 11); // RX, TX

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
    //delay(1000);

    //Read IP address.
    //gsm.SimpleWriteln("AT+CIFSR");
    //delay(5000);
    //Read until serial buffer is empty.
    //gsm.WhileSimpleRead();

 sendcoordinates();
//

          Serial.println("starting connection...");
          //TCP Client GET, send a GET request to the server and
          //save the reply.
//          numdata=inet.httpGET("33f1f4a7.ngrok.io", 80, "/handle_data.php", "?lat=41.948600&lon=-88.769332", 50);
          //Print the results.
 //         Serial.println("\nNumber of data received:");
 //         Serial.println(numdata);
 //         Serial.println("\nData received:");
 //         Serial.println(msg);
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

void sendcoordinates()
{
  mySerial.println("AT+CGATT?");   //Attach or Detach from GPRS Service (Result  1 = Attach , 2 = Detached )
  delay(300); 
  ShowSerialData();

  mySerial.println("AT+CIPSHUT=0");  //Close TCP Connection
  delay(300); 
  ShowSerialData();

  mySerial.println("AT+SAPBR=3,1,\"broadband\",\"internet\"");//setting the APN, the second need you fill in your local apn server
  delay(1000); 
  ShowSerialData();

  mySerial.println("AT+CIPSPRT=0");
  delay(3000); 
  ShowSerialData();

  mySerial.println("AT+CIPSTART=\"TCP\",\"33f1f4a7.ngrok.io\",\"80\"");//start up the connection

  delay(2000); 
  ShowSerialData();
  mySerial.println();
  mySerial.println("AT+CIPSEND");//begin send data to remote server
  delay(4500);
  ShowSerialData();



  mySerial.print("POST /handle_data.php?");//here is the feed you apply from pachube
  delay(500);
  ShowSerialData();

  mySerial.print("lat=");   //DATA feed name
  mySerial.print("41.123456");   //DATA to send
  delay(10);
  ShowSerialData();

  mySerial.print("&lon=");   //DATA feed name
  mySerial.print("-88.123456");   //DATA to send
  delay(10);
  ShowSerialData();


  mySerial.print(" HTTP/1.1\r\n");
  delay(500);
  ShowSerialData();

  mySerial.print("Host: 33f1f4a7.ngrok.io\r\n");
  delay(500);
  ShowSerialData();


  mySerial.print("Connection: keep-alive");  //working
  //mySerial.print("Connection: close");         //working as well
  mySerial.print("\r\n");
  mySerial.print("\r\n");
  //mySerial.println();
  delay(500);
  ShowSerialData();


  mySerial.println((char)26);//sending
  mySerial.write(0x1A);
  delay(2500);//waitting for reply, important! the time is base on the condition of internet 
  mySerial.println(); 
  ShowSerialData();

  mySerial.println("AT+CIPCLOSE");//close the connection
  delay(100);
  ShowSerialData();

  mySerial.println("AT+CIPSHUT=0");
  delay(100); 
  ShowSerialData();

}
void ShowSerialData()
{
  while(Serial.available()!=0)
    Serial.write(Serial.read());
}
