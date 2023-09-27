#include <Arduino.h>
int State = 0;
int Switch = 22;
int Enable = 24;
int PWM = 2;
int Throttle = 0;
unsigned long t = 0;
unsigned long t0;
void setup() 
{
pinMode (PWM,OUTPUT);               //PWM Output
pinMode (Enable,OUTPUT);              //Enable Output
pinMode (Switch,INPUT);              //Switch Input

}

void loop() 
{

switch (State) 
{
    case 0:                                 // Etapa Inicial
        digitalWrite(Enable, false);
        analogWrite(PWM,0);

        if (digitalRead(Switch) == true)
            {
                State = 1;
            }
        break;

    case 1:                                 // Etapa Aceleración
        digitalWrite(Enable, true);
        analogWrite(PWM,Throttle);
        if (digitalRead(Switch) == true)
            {
                delay(500);
                Throttle = Throttle + 40;
                if (Throttle > 255)
                {
                    Throttle = 255;
                }
            }
        else
            {
                State = 2;
                t0 = millis();
            }

      break;
    case 2:                                 // Etapa Espera Switch
        digitalWrite(Enable, true);
        analogWrite(PWM,Throttle);

        if (digitalRead(Switch) == true)
            {
                State = 3;
            } 
        else
            {
                t = millis() - t0;
                if (t >> 1000)
                {
                    State = 0;
                }
            }
          
      break;
    case 3:                                 //Etapa Marcha Normal
        
        if (digitalRead(Switch) == true)
            {
                digitalWrite(Enable, true);
                analogWrite(PWM,Throttle);
                t0 = millis();
            }
        else
            {
                t = millis() - t0;
                if (t >> 1000)
                {
                    State = 0;
                }
            } 

      break;
  default:
    digitalWrite(Enable, false);
    analogWrite(PWM,0);
}
}