#define MOTOR_PIN0  PD3 //3
#define MOTOR_PIN1  PD4 //4
#define MOTOR_PIN2  PD5 //5
#define MOTOR_PIN3  PD6 //6
#define MOTOR_PIN4  PD7 //7
#define MOTOR_PIN5  PB0 //8
#define MOTOR_PIN6  PB1 //9
#define MOTOR_PIN7  PB3 //11
#define PERIOD      1000//µs

uint8_t dist[8] =  {0, 0, 0, 0, 0, 0, 0, 0};
uint8_t dist5[8] = {5, 5, 0, 0, 0, 0, 0, 5};
uint8_t dist4[8] = {4, 4, 0, 0, 0, 0, 0, 4};
uint8_t dist3[8] = {3, 3, 0, 0, 0, 0, 0, 3};
uint8_t dist2[8] = {2, 2, 0, 0, 0, 0, 0, 2};
uint8_t dist1[8] = {1, 1, 0, 0, 0, 0, 0, 1};
uint8_t dist0[8] = {0, 0, 0, 0, 0, 0, 0, 0};
uint8_t duty[6] = {100, 70, 40, 0, 0, 0};

void setup() 
{  
  Serial.begin(57600);  
  DDRD |= (1<<MOTOR_PIN0) | (1<<MOTOR_PIN1) | (1<<MOTOR_PIN2) | (1<<MOTOR_PIN3) | (1<<MOTOR_PIN4);
  DDRB |= (1<<MOTOR_PIN5) | (1<<MOTOR_PIN6) | (1<<MOTOR_PIN7);
}  

void loop()
{
  uint16_t i;
  uint8_t copy;
  for(copy=0; copy<8; copy++) dist[copy] = dist5[copy];
  for(i=0; i<1000; i++)
  {
    write_all_low(); //all motors initially off
    delayMicroseconds(PERIOD * (100-duty[0])/100);
    for(uint8_t index=0; index<5; index++)
    {
      check_all(4-index);
      delayMicroseconds(PERIOD * (duty[index]-duty[index+1])/100);
    }
  }
  
  for(copy=0; copy<8; copy++) dist[copy] = dist4[copy];
  for(i=0; i<1000; i++)
  {
    write_all_low(); //all motors initially off
    delayMicroseconds(PERIOD * (100-duty[0])/100);
    for(uint8_t index=0; index<5; index++)
    {
      check_all(4-index);
      delayMicroseconds(PERIOD * (duty[index]-duty[index+1])/100);
    }
  }
  
  for(copy=0; copy<8; copy++) dist[copy] = dist3[copy];
  for(i=0; i<1000; i++)
  {
    write_all_low(); //all motors initially off
    delayMicroseconds(PERIOD * (100-duty[0])/100);
    for(uint8_t index=0; index<5; index++)
    {
      check_all(4-index);
      delayMicroseconds(PERIOD * (duty[index]-duty[index+1])/100);
    }
  }
  
  for(copy=0; copy<8; copy++) dist[copy] = dist2[copy];
  for(i=0; i<1000; i++)
  {
    write_all_low(); //all motors initially off
    delayMicroseconds(PERIOD * (100-duty[0])/100);
    for(uint8_t index=0; index<5; index++)
    {
      check_all(4-index);
      delayMicroseconds(PERIOD * (duty[index]-duty[index+1])/100);
    }
  }
  
  for(copy=0; copy<8; copy++) dist[copy] = dist1[copy];
  for(i=0; i<1000; i++)
  {
    write_all_low(); //all motors initially off
    delayMicroseconds(PERIOD * (100-duty[0])/100);
    for(uint8_t index=0; index<5; index++)
    {
      check_all(4-index);
      delayMicroseconds(PERIOD * (duty[index]-duty[index+1])/100);
    }
  }
  
  for(copy=0; copy<8; copy++) dist[copy] = dist0[copy];
  for(i=0; i<1000; i++)
  {
    write_all_low(); //all motors initially off
    delayMicroseconds(PERIOD * (100-duty[0])/100);
    for(uint8_t index=0; index<5; index++)
    {
      check_all(4-index);
      delayMicroseconds(PERIOD * (duty[index]-duty[index+1])/100);
    }
  }
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
// writes 1 (turns on) motors depending on their distance
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

