#include "SIM900.h"
#include <SoftwareSerial.h>
#include "inetGSM.h"
InetGSM inet;
char msg[50];
char postdata;
int numdata;
char inSerial[50];
int i=0;
boolean started=false;

void setup()
{
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
    delay(1000);

  // Setup HTTP Profile
  gsm.SimpleWriteln("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
  gsm.SimpleRead();
  delay(2500);

  // Setup APN
  gsm.SimpleWriteln("AT+SAPBR=3,1,\"APN\",\"broadband\"");
  gsm.SimpleRead();
  gsm.WhileSimpleRead();
  delay(2500);


  // HTTP INIT
  gsm.SimpleWriteln("AT+HTTPINIT");
  gsm.SimpleRead();
  gsm.WhileSimpleRead();
  delay(2500);

  // Setup Client ID
  gsm.SimpleWriteln("AT+HTTPPARA=\"CID\",1");
  gsm.SimpleRead();
  gsm.WhileSimpleRead();
  delay(2500);

  // Setup Request
  gsm.SimpleWriteln("AT+HTTPPARA=\"URL\",\"http://443bcc59.ngrok.io/handle_data.php?lat=563&lon=309\"");
  gsm.SimpleRead();
  gsm.WhileSimpleRead();
  delay(2500);

  // Make GET Request
  gsm.SimpleWrite("AT+HTTPACTION=0");
  gsm.SimpleRead();
  gsm.WhileSimpleRead();
  delay(2500);

  // Read Request
  gsm.SimpleWriteln("AT+HTTPREAD");
  gsm.SimpleRead();
  delay(1000);

}
};

void loop()
{
};

