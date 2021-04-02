// code for second control ardino


#define fan_motor 9
#define encoder 2
#define lock 3
#define buzz 4
#define temp_snsr A1
#define relay 5
#define LDR A0
#define CLK 6
#define DATA 7
#define LATCH 8
#define seg_1 10
#define seg_2 11
#define seg_3 12
#define seg_4 13


/*** declarations and initializations ***/

const int ppr = 48; // 24 pulse per rotation for rising edge
unsigned long ms = 0;
int RPM = 0;
volatile long count = 0;
long pulsePerSec;
int tempreature;
float volt;
int digits[] = {seg_1, seg_2, seg_3, seg_4};
// corresponding display to the numbers on the seven segment 
int nums[] ={0b11111100 , 0b01100000, 0b11011010, 0b11110010, 0b01100110, 0b10110110, 0b10111110, 0b11100000, 0b11111110, 0b11110110};   
             /*0*/        /*1*/       /*2*/       /*3*/       /*4*/       /*5*/       /*6*/       /*7*/       /*8*/       /*9*/
 

          
void setup() {
  for(int i = 4; i <= 13; i++)
  {
    pinMode(i, OUTPUT);
  }
  pinMode(encoder, INPUT);
  pinMode(temp_snsr, INPUT);
  pinMode(LDR, INPUT);
  pinMode(lock, INPUT);
  digitalWrite(relay, LOW);
  digitalWrite(fan_motor, LOW);
  digitalWrite(buzz, LOW);
  for(int i = 0; i < 4; i++)
  {
    digitalWrite(digits[i], HIGH);
  }
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(encoder), pulse, CHANGE);
  ms = millis();
  pulsePerSec = count;
  while(!Serial.available());
  Serial.read();
}


void loop() {
      Temp_cntrl_sys();
      Count_RPM();
      segmentOut();
      schmitt_trigger();
      if(!digitalRead(lock))                         // check if the button is pressed in order to turn off the system
      {
        digitalWrite(relay, LOW);
        digitalWrite(fan_motor, LOW);
        digitalWrite(buzz, LOW);
        for(int i = 0; i < 4; i++)
        {
          digitalWrite(digits[i], HIGH);
        }
        Serial.print(1);
        RPM = 0;
        while(!Serial.available());
        Serial.read();
      }
}


void pulse()
{
  count++;
}

void Temp_cntrl_sys()
{
                  /****** Tempreature Control System ******/
    volt = (5.0/1023.0)*analogRead(temp_snsr);
    tempreature = (volt) * 100;
    if(tempreature < 15)
    {
      // stop fan
      analogWrite(fan_motor, 0);
    }
    else if(tempreature < 25)
    {
      // fan is set to 1/3 of its full speed
      analogWrite(fan_motor, 85);           // third of max PWM value 255
    }
    else if(tempreature < 35)
    {
      // fan is set to 2/3 of its full speed
      analogWrite(fan_motor, 170);          // two thirds of max PWM value 255
      digitalWrite(buzz, LOW);
    }
    else if(tempreature < 45)
    {
      // fan is set to full speed
      analogWrite(fan_motor, 255);          // PWM max value
      digitalWrite(buzz, LOW);
    }
    else
    {
      // turn the alarming Buzzer on
      digitalWrite(buzz, HIGH);
    }
}

void Count_RPM()
{
          /******   Counting fan RPM through the encoder   ******/
  if(millis() - ms >= 1000)
  {
    pulsePerSec = count - pulsePerSec;
    RPM = (abs(pulsePerSec) * 60) / ppr;
    ms = millis();
    pulsePerSec = count;
  }
}

void segmentOut()
{
  /******   Display the fan RPM on the Seven Segment   ******/
  int x = RPM;
  for (int i = 3; i >= 0; i--)
  {
    shiftOut(DATA, CLK, LSBFIRST, nums[x%10]);
    digitalWrite(LATCH, HIGH);
    delayMicroseconds(10);
    digitalWrite(digits[i], LOW);
    digitalWrite(LATCH, LOW);
    delay(1);
    x = x / 10;
    digitalWrite(digits[i], HIGH);
   }
}

void schmitt_trigger()
{
                /******   Light Control System   ******/
  if(analogRead(LDR) >= 600)
  {
    digitalWrite(relay, LOW);
  }
  else if(analogRead(LDR) < 400)
  {
    digitalWrite(relay, HIGH);
  }
}
