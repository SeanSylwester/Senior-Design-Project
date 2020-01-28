#define LPPin       PD2 //2
#define MOTOR_PIN0  PD3 //3
#define MOTOR_PIN1  PD4 //4
#define MOTOR_PIN2  PD5 //5
#define MOTOR_PIN3  PD6 //6
#define MOTOR_PIN4  PD7 //7
#define MOTOR_PIN5  PB0 //8
#define MOTOR_PIN6  PB1 //9
#define MOTOR_PIN7  PB3 //11
#define debugPin    PB4 //12

#define NO_CHANGE_MIN 1000 //~sec
#define TIMEOUT_MIN   1000 //~sec
#define COUNTER_MAX   250  //ms
#define PERIOD        1000 //µs

uint8_t dist[8];
uint8_t prev[8];
uint8_t duty[6] = {100, 70, 40, 35, 30, 0};

uint16_t timeout = 0;
uint16_t counter = 0;
byte dummy;
uint8_t LP = 0;
uint16_t no_change = 0;


void setup() 
{  
  Serial.begin(57600);  
  DDRD |= (1<<MOTOR_PIN0) | (1<<MOTOR_PIN1) | (1<<MOTOR_PIN2) | (1<<MOTOR_PIN3) | (1<<MOTOR_PIN4);
  DDRB |= (1<<MOTOR_PIN5) | (1<<MOTOR_PIN6) | (1<<MOTOR_PIN7) | (1<<debugPin);

  write_all_high();
  delay(200);
  write_all_low();
  delay(200);
  write_all_high();
  delay(200);
  write_all_low();
  
  while(Serial.available() > 0) dummy = Serial.read();
}  

//approx time: 
void loop()
{  
  //Check for LP mode
  if(LP == 0 && (PIND & 1<<LPPin)) LP = 1;
  else if(LP == 1 && !(PIND & 1<<LPPin)) LP = 0;
  
  //read full data packet of 5 bytes if available
  if(Serial.available() >= 5) 
  {
    if(Serial.read() == 0xff)
    {
      read_bluetooth();
      timeout = 0;
    }
  }
  else timeout++;

  //if no signal for 1s, go into standby
  if(timeout >= TIMEOUT_MIN)
  {
    for(uint8_t i=0; i<=7; i++)
    {
      dist[i] = 0;
      timeout = 0;
    }  
    write_all_high();
    delay(200);
    write_all_low();
  }

 

  //runs a PWM period if there was a change in dists,
  // or based on a long pulse pattern
  write_all_low(); //all motors initially off 
  if(check_for_change() ||
     (LP == 0 && counter < COUNTER_MAX/2) ||
     (LP == 1 && counter < COUNTER_MAX/4))
  {
    delayMicroseconds(PERIOD * (100-duty[0])/100);
    for(uint8_t index=0; index<5; index++)
    {
      check_all(4-index);
      delayMicroseconds(PERIOD * (duty[index]-duty[index+1])/100);
    }
  }
  else 
  {
    write_all_low();
    delayMicroseconds(PERIOD);
  }
  
  update_prev();
  counter++;
  if(counter >= COUNTER_MAX) counter = 0;
}

//*********************************************************//
// reads the 4-byte packet of sensor data from the sensor
//  controller
// TODO: adjust for magnetometer data packet
//*********************************************************//
void read_bluetooth()
{
  byte input0 = byte(Serial.read());  //read 1st byte
  byte input1 = byte(Serial.read());  //read 2nd byte
  byte input2 = byte(Serial.read());  //read 3rd byte
  byte input3 = byte(Serial.read());  //read 4th byte

  dist[4] =  uint8_t(input0 & 0x0F);
  dist[3] = uint8_t((input0 & 0xF0) >> 4);
  dist[2] =  uint8_t(input1 & 0x0F);
  dist[1] = uint8_t((input1 & 0xF0) >> 4);
  dist[0] =  uint8_t(input2 & 0x0F);
  dist[7] = uint8_t((input2 & 0xF0) >> 4);
  dist[6] =  uint8_t(input3 & 0x0F);
  dist[5] = uint8_t((input3 & 0xF0) >> 4);
}

//*********************************************************//
// writes 1 (turns off) to all of the motor pins
//*********************************************************//
void write_all_high()
{
  PORTD |= (1<<MOTOR_PIN0) | (1<<MOTOR_PIN1) | (1<<MOTOR_PIN2) | (1<<MOTOR_PIN3) | (1<<MOTOR_PIN4);
  PORTB |= (1<<MOTOR_PIN5) | (1<<MOTOR_PIN6) | (1<<MOTOR_PIN7);
}

//*********************************************************//
// writes 0 to all of the motor pins
//*********************************************************//
void write_all_low()
{
  PORTD &= ~((1<<MOTOR_PIN0) | (1<<MOTOR_PIN1) | (1<<MOTOR_PIN2) | (1<<MOTOR_PIN3) | (1<<MOTOR_PIN4));
  PORTB &= ~((1<<MOTOR_PIN5) | (1<<MOTOR_PIN6) | (1<<MOTOR_PIN7));
}

//*********************************************************//
// writes 0 (turns on) motors depending on their distance
//  range and what index (i) the pwm loop is at
//*********************************************************//
void check_all(uint8_t i)
{
    PORTD |= ((dist[0] > i) << MOTOR_PIN0) 
           | ((dist[1] > i) << MOTOR_PIN1) 
           | ((dist[2] > i) << MOTOR_PIN2) 
           | ((dist[3] > i) << MOTOR_PIN3) 
           | ((dist[4] > i) << MOTOR_PIN4);
    PORTB |= ((dist[5] > i) << MOTOR_PIN5) 
           | ((dist[6] > i) << MOTOR_PIN6)
           | ((dist[7] > i) << MOTOR_PIN7);
}

//*********************************************************//
// updates the "prev" array with the current dists values
//*********************************************************//
void update_prev()
{
  for(uint8_t i=0; i<8; i++)
    prev[i] = dist[i];
}

//*********************************************************//
// checks if any dists values have changed
//*********************************************************//
uint8_t check_for_change()
{
  uint8_t diff = 0;
  for(uint8_t i=0; i<8; i++)
    if(prev[i] != dist[i]) diff = 1;

  if(diff == 0) no_change++;
  else no_change = 0;

  if(no_change > NO_CHANGE_MIN) return 0;
  return 1;
}


