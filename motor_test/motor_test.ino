
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

#define BUFLEN 6
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
  buffer[2] = 0xb0; // motor voltage cmd, works also with e.g. 0xb1 (other direction)
  buffer[3] = 3;
  buffer[4] = 64; // 25% PWM
}

uint8_t get_checksum() {
  uint8_t sum = 0;
  for (unsigned k = 0; k < BUFLEN-1; ++k)
    sum += buffer[k];
  return ~sum +1;
}  

bool verify_response()
{
  uint8_t sum = 0;
  uint8_t bytes_read = 0;
  while (Serial.available()) {
    sum += Serial.read(); // read bytes 
    ++bytes_read;
  }
  
  // all bytes must sum to zero if packet was correct
  return (bytes_read == 15) and (sum == 0);
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

  buffer[BUFLEN-1] = get_checksum();
  send_mode();
  Serial.write(buffer, BUFLEN);
  Serial.flush();
  recv_mode();

  // delay 10 ms with LED 10% on
  delay(9);  
  digitalWrite(LED, HIGH);
  delay(1);
 
  if (Serial.available() and verify_response()) // got response
  {
    // be happy
  } else {
    delay(1000);
    digitalWrite(LED, LOW);
    delay(1000);
  }
  digitalWrite(LED, LOW);
 
}
