
/*-----------------------------+
 | Supreme Machines            |
 | Sensorimotor rev1-1         |
 | Ping Example                |
 | Matthias Kubisch            |
 | Berlin, July 20th 2018      |
 +-----------------------------+
 | LED blinks long MotorID+1   |
 | times if a motor responded  |
 | to the ping cmd send.       |
 +-----------------------------*/


#define BAUDRATE 1000000
#define READ_DISABLE 5
#define WRITE_ENABLE 4

#define BUFLEN 5
#define LED 13

uint8_t id = 0;

uint8_t buffer[BUFLEN];

void setup() {
  Serial.begin(BAUDRATE); /* 1 MBaud */
  pinMode(READ_DISABLE, OUTPUT);
  pinMode(WRITE_ENABLE, OUTPUT);
  pinMode(LED, OUTPUT);
  
  /* prepare buffer preamble */
  buffer[0] = 0xff; // sync 0
  buffer[1] = 0xff; // sync 1
  buffer[2] = 0xe0; // ping cmd, works also with e.g. 0xc0 (state request)
}

uint8_t get_checksum() {
  uint8_t sum = 0;
  for (unsigned k = 0; k < BUFLEN-1; ++k)
    sum += buffer[k];
  return ~sum +1;
}  

void send_mode() {
  digitalWrite(READ_DISABLE, HIGH);
  digitalWrite(WRITE_ENABLE, HIGH);
}

void recv_mode() {
  digitalWrite(READ_DISABLE, LOW);
  digitalWrite(WRITE_ENABLE, LOW);
}

void loop() {
  buffer[3] = id;
  buffer[4] = get_checksum();
 
  send_mode();
  Serial.write(buffer, BUFLEN);
  Serial.flush();
  recv_mode();
  
  delay(20);
 
  if (Serial.available()) // got response
  {
    while (Serial.available())
      Serial.read(); // eat bytes
    for (unsigned j = 0; j < id+1; ++j) {
      delay(500);
      digitalWrite(LED, HIGH);
      delay(500);
      digitalWrite(LED, LOW);
    }  
  }
  
  
  ++id;
  if (id>=10)
    id = 0;
  
  delay(980);
  digitalWrite(LED, HIGH);
  delay(20);
  digitalWrite(LED, LOW);
}
