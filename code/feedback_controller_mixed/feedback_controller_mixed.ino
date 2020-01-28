#define LPPin       PD2 //2
#define MOTOR_PIN7  PD3 //3
#define MOTOR_PIN6  PD4 //4
#define MOTOR_PIN5  PD5 //5
#define MOTOR_PIN4  PD6 //6
#define MOTOR_PIN3  PD7 //7
#define MOTOR_PIN2  PB0 //8
#define MOTOR_PIN1  PB1 //9
#define MOTOR_PIN0  PB3 //11
#define debugPin    PB4 //12

#define TIMEOUT_MIN   1000 //ms
#define COUNTER_MAX   500  //ms

#define PERIOD        1000  //µs
#define PULSE_DUTY    0.5   //%
#define PWM_HIGH_DUTY 0.9   //%
#define PWM_LOW_DUTY  0.7   //%


uint8_t dist[8];
uint8_t pins[8] = {MOTOR_PIN0, MOTOR_PIN1, MOTOR_PIN2, MOTOR_PIN3, MOTOR_PIN4, MOTOR_PIN5, MOTOR_PIN6, MOTOR_PIN7};

uint16_t timeout = 0;
uint16_t counter = 0;
byte dummy;
uint8_t LP = 0;


void setup() 
{  
  Serial.begin(57600);  
  DDRD |= (1<<MOTOR_PIN7) | (1<<MOTOR_PIN6) | (1<<MOTOR_PIN5) | (1<<MOTOR_PIN4) | (1<<MOTOR_PIN3);
  DDRB |= (1<<MOTOR_PIN2) | (1<<MOTOR_PIN1) | (1<<MOTOR_PIN0) | (1<<debugPin);

  write_all_high();
  delay(200);
  write_all_low();
  delay(200);
  write_all_high();
  delay(200);
  write_all_low();
  
  while(Serial.available() > 0) dummy = Serial.read();
}  

void loop()
{  
  uint8_t i;

  
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

  write_all_low(); //all motors initially off 
  
  //turn on 4/3 motors and 2/1 motors if in the counter pulse
  for(i=0; i<8; i++)
  {
    if( dist[i] == 4
     || dist[i] == 3
     || (dist[i] == 2 && counter <= COUNTER_MAX * PULSE_DUTY)
     || (dist[i] == 2 && counter <= COUNTER_MAX * PULSE_DUTY))
    {
      if(i <= 2)
        PORTB |= 1 << pins[i];
      else
        PORTD |= 1 << pins[i]; 
    }
  }

  //wait to the low duty cycle turn off point
  //then turn off the low motors (3 and 1)
  delayMicroseconds(PERIOD * (1-PWM_LOW_DUTY));
  for(i=0; i<8; i++)
  {
    if(dist[i] == 3 || dist[i] == 1)
    {
      if(i <= 2)
        PORTB &= ~(1 << pins[i]);
      else
        PORTD &= ~(1 << pins[i]); 
    }
  }

  //wait to the high duty cycle turn-off point
  //then turn off the high motors (4 and 2)
  delayMicroseconds(PERIOD * (PWM_HIGH_DUTY-PWM_LOW_DUTY));
  for(i=0; i<8; i++)
  {
    if(dist[i] == 4 || dist[i] == 2)
    {
      if(i <= 2)
        PORTB &= ~(1 << pins[i]);
      else
        PORTD &= ~(1 << pins[i]); 
    }
  }
  delayMicroseconds(PERIOD * (1-PWM_HIGH_DUTY));
  
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
  PORTD |= (1<<MOTOR_PIN7) | (1<<MOTOR_PIN6) | (1<<MOTOR_PIN5) | (1<<MOTOR_PIN4) | (1<<MOTOR_PIN3);
  PORTB |= (1<<MOTOR_PIN2) | (1<<MOTOR_PIN1) | (1<<MOTOR_PIN0);
}

//*********************************************************//
// writes 0 to all of the motor pins
//*********************************************************//
void write_all_low()
{
  PORTD &= ~((1<<MOTOR_PIN7) | (1<<MOTOR_PIN6) | (1<<MOTOR_PIN5) | (1<<MOTOR_PIN4) | (1<<MOTOR_PIN3));
  PORTB &= ~((1<<MOTOR_PIN2) | (1<<MOTOR_PIN1) | (1<<MOTOR_PIN0));
}


