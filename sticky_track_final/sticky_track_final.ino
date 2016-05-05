/* Ian's Shit */




void setup() {
init_code();
}

void loop() {

setup_GPS();
send_HTTP();

}


void init_code() {
  pinMode(onModulePin, OUTPUT);
  Serial.begin(9600);    
// Init GPS
// Attach GRPS
  setup_APN();

}

void setup_APN() {
  // Test CPIN?
  gsm.SimpleWriteln("AT+CPIN=9215");  
  delay(3000);  

  // Register the network
  gsm.SimpleWriteln("AT+CREG?", "+CREG: 0,1", 1000); 

  // sets connection type and apn
  gsm.SimpleWriteLn("AT+SAPBR=3,1,\"Contype\", \"GPRS\"");
  gsm.SimpleWriteLn("AT+SAPBR=3,1,\"APN\", \"broadband\"");
}

void setup_GPS() {
// Read GPS
// Convert to valuable format
// Init GPS
// Set GPS data to variables referenced in send_HTTP();
}

void send_HTTP() {
// HTTP INIT
// HTTP PARA CID
// HTTP PARA URL
// HTTP ACTION
// HTTP READ

}


