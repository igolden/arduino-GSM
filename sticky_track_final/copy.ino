/* Comment on the top */

//Setup Serial, etc


void setup() {

  pinMode(onModulePin, OUTPUT);
  Serial.begin(9600);    

  power_on();
  power_onGPS();
  power_onGPRS();
  delay(5000);

  setup_APN();
}

void loop() {

get_GPS();
send_HTTP();
}
void setup_APN() {
  // Test CPIN?
  sendATcommand("AT+CPIN=****", "OK", 2000);  

  delay(3000);  

  // Register the network
  while( (sendATcommand("AT+CREG?", "+CREG: 0,1", 1000) || sendATcommand("AT+CREG?", "+CREG: 0,5", 1000)) == 0 );


  // Don't need this - calling line registration
  // sendATcommand("AT+CLIP=1", "OK", 1000); 

  // Look into this 
  while ( start_GPS() == 0);

  // while (sendATcommand("AT+CREG?", "+CREG: 0,1", 2000) == 0);

  // sets APN , user name and password
  sendATcommand("AT+SAPBR=3,1,\"Contype\",\"GPRS\"", "OK", 2000);
  sendATcommand("AT+SAPBR=3,1,\"APN\",\"*******\"", "OK", 2000);

}

// Redo this function with our library 
int8_t get_GPS(){

    int8_t counter, answer;
    long previous;

    // First get the NMEA string
    // Clean the input buffer
    while( Serial.available() > 0) Serial.read(); 
    // request Basic string
    sendATcommand("AT+CGPSINF=0", "AT+CGPSINF=0\r\n\r\n", 2000);

    counter = 0;
    answer = 0;
    memset(frame, '\0', 100);    // Initialize the string
    previous = millis();
    // this loop waits for the NMEA string
    do{

        if(Serial.available() != 0){    
            frame[counter] = Serial.read();
            counter++;
            // check if the desired answer is in the response of the module
            if (strstr(frame, "OK") != NULL)    
            {
                answer = 1;
            }
        }
        // Waits for the asnwer with time out
    }
    while((answer == 0) && ((millis() - previous) < 2000));  

    frame[counter-3] = '\0'; 
    
    // Parses the string 
    strtok(frame, ",");
    strcpy(longitude,strtok(NULL, ",")); // Gets longitude
    strcpy(latitude,strtok(NULL, ",")); // Gets latitude
    strcpy(altitude,strtok(NULL, ".")); // Gets altitude 
    strtok(NULL, ",");    
    strcpy(date,strtok(NULL, ".")); // Gets date
    strtok(NULL, ",");
    strtok(NULL, ",");  
    strcpy(satellites,strtok(NULL, ",")); // Gets satellites
    strcpy(speedOTG,strtok(NULL, ",")); // Gets speed over ground. Unit is knots.
    strcpy(course,strtok(NULL, "\r")); // Gets course

    convert2Degrees(latitude);
    convert2Degrees(longitude);
    
    return answer;
}


void send_HTTP(){
    
    uint8_t answer=0;
    // Initializes HTTP service
    answer = sendATcommand("AT+HTTPINIT", "OK", 10000);
    if (answer == 1)
    {
        // Sets CID parameter
        answer = sendATcommand("AT+HTTPPARA=\"CID\",1", "OK", 5000);
        if (answer == 1)
        {
            // Sets url 
            sprintf(aux_str, "AT+HTTPPARA=\"URL\",\"http://%s/demo_sim908.php?", url);
            Serial.print(aux_str);
            sprintf(frame, "visor=false&latitude=%s&longitude=%s&altitude=%s&time=%s&satellites=%s&speedOTG=%s&course=%s",
            latitude, longitude, altitude, date, satellites, speedOTG, course);
            Serial.print(frame);
            answer = sendATcommand("\"", "OK", 5000);
            if (answer == 1)
            {
                // Starts GET action
                answer = sendATcommand("AT+HTTPACTION=0", "+HTTPACTION:0,200", 30000);
                if (answer == 1)
                {

                    Serial.println(F("Done!"));
                }
                else
                {
                    Serial.println(F("Error getting url"));
                }

            }
            else
            {
                Serial.println(F("Error setting the url"));
            }
        }
        else
        {
            Serial.println(F("Error setting the CID"));
        }    
    }
    else
    {
        Serial.println(F("Error initializating"));
    }

    sendATcommand("AT+HTTPTERM", "OK", 5000);
    
}

// Send AT Command Function
int8_t sendATcommand(char* ATcommand, char* expected_answer, unsigned int timeout){

    uint8_t x=0,  answer=0;
    char response[100];
    unsigned long previous;

    memset(response, '\0', 100);    // Initialice the string
    
    delay(100);
    
    while( Serial.available() > 0) Serial.read();    // Clean the input buffer
    
    if (ATcommand[0] != '\0')
    {
        Serial.println(ATcommand);    // Send the AT command 
    }


    x = 0;
    previous = millis();

    // this loop waits for the answer
    do{
        if(Serial.available() != 0){    // if there are data in the UART input buffer, reads it and checks for the asnwer
            response[x] = Serial.read();
            //Serial.print(response[x]);
            x++;
            if (strstr(response, expected_answer) != NULL)    // check if the desired answer (OK) is in the response of the module
            {
                answer = 1;
            }
        }
    }while((answer == 0) && ((millis() - previous) < timeout));    // Waits for the asnwer with time out

    return answer;
}
