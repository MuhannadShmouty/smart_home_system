// security code for first arduino

#include <LiquidCrystal.h>
#include <Servo.h>

// pins
#define RS 2
#define EN 3
#define D4 4
#define D5 5
#define D6 6
#define D7 7
#define LED1 8
#define servo 9
#define C1 10
#define C2 11
#define C3 12
#define R1 A0
#define R2 A1
#define R3 A2
#define R4 A3

LiquidCrystal lcd(RS, EN, 4, 5, 6, 7);
Servo me_servo; 
char keypad[4][3] = {{'1', '2', '3'},
                     {'4', '5', '6'},
                     {'7', '8', '9'},
                     {'*', '0', '#'}};
// the password
char password[] = "00";
char input[16]; 
int index = 0;


void setup() {
  Serial.begin(9600);
  for(int hh = A0; hh <= A3; hh++)
  {
    pinMode(hh, OUTPUT);
  }
  pinMode(LED1, OUTPUT); 
  for(int i = 10; i <= 12; i++)
  {
    pinMode(i, INPUT_PULLUP);
  }
   me_servo.attach(servo, 1000, 2000);
   me_servo.write(0);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.clear();
  lcd.write("Password: ");
  lcd.setCursor(0, 1);
}

void loop() {
           /********   Nested loop to obtain the pressed character   ********/ 
  for(int i = 0; i < 4; i++)
  {
    digitalWrite(R1 + i, LOW);
    delayMicroseconds(10);
    for(int j = 0; j < 3; j++)
    {
       if(!digitalRead(C1 + j))
       {
          /********   check if password input is finished   ********/ 
          if(i == 3 && j == 2)
          {
            // the pressed button is '#'
            
            if(checkPass(password, input))
            {
              // correct password
              
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.write("Welcome!           ");
              digitalWrite(LED1, HIGH);
              me_servo.write(90);              // turn servo 90 degree
              Serial.print('1');
              while(!Serial.available());
              // Reset
              Serial.read();
              digitalWrite(LED1, LOW);
              me_servo.write(0);
              lcd.clear();
              lcd.write("Password: ");
              lcd.setCursor(0, 1);
              index = 0;
              return;
            }
            else
            {
              lcd.setCursor(0, 0);
              lcd.write("Wrong Input!!        ");
              delay(3000);
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.write("Password:       ");
              lcd.setCursor(0, 1);
              index = 0;
              continue;
            }
          }
          
            // storing the new input
            input[index] = keypad[i][j];
            index++;
            lcd.write('*');
            while(!digitalRead(C1 + j));
       }
    }
    digitalWrite(R1+i, HIGH);
  }
 
}

bool checkPass(char arr1[], char arr2[])
{
  if (index != sizeof(password) - 1)
    return false;
    
  for(int i = 0; i < sizeof(password) - 1; i++)
  {
    if(arr1[i] != arr2[i])
    {
      return 0;
    }
  }
  return 1;
}
