#include <Arduino.h>
int State = 0;
int Switch = 21;
int Enable = 15;
#define PWM 2 //PWM Pin
// setting PWM properties
const int freq = 5000;
const int PWMChannel = 0;
const int resolution = 10; //Resolution 8, 10, 12, 15
int Throttle = 0;
unsigned long t = 0;
unsigned long t0;
void setup() 
{
Serial.begin(115200);
pinMode (Enable, OUTPUT);                //Enable Output
pinMode(PWM,OUTPUT);                    //PWM Output
pinMode (Switch, INPUT);                 //Switch Input

// configure LED PWM functionalitites
ledcSetup(PWMChannel, freq, resolution);

// attach the channel to the GPIO2 to be controlled
ledcAttachPin(PWM, PWMChannel);
}

void loop() 
{

switch (State) 
{
    
    case 0:                                 // Etapa Inicial
        digitalWrite(Enable, false);
        ledcWrite(PWMChannel,0);
        Throttle = 0;

        if (digitalRead(Switch) == true)
            {
                State = 1;
            }
        break;

    case 1:
        digitalWrite(Enable, true);
        ledcWrite(PWMChannel,0);
        Throttle = 0;
        delay(500);
        State = 2;

    case 2:                                 // Etapa Aceleración
        digitalWrite(Enable, true);
        ledcWrite(PWMChannel,Throttle);
        if (digitalRead(Switch) == true)
            {
                delay(100);
                Throttle = Throttle + 30;
                if (Throttle > 1023)
                {
                    Throttle = 1023;
                }
            }
        else
            {
                State = 3;
                t0 = esp_timer_get_time();
            }

      break;
    case 3:                                 // Etapa Espera Switch
        digitalWrite(Enable, true);
        ledcWrite(PWMChannel,Throttle);

        if (digitalRead(Switch) == true)
            {
                delay(100);
                State = 4;
            } 
        else
            {
                t = esp_timer_get_time() - t0;
                if (t > 100000)
                {
                    State = 0;
                }
            }
          
      break;
    case 4:                                 //Etapa Marcha Normal
        
        if (digitalRead(Switch) == true)
            {
                delay(100);
                digitalWrite(Enable, true);
                ledcWrite(PWMChannel,Throttle);
                t0 = esp_timer_get_time();
            }
        else
            {
                t = esp_timer_get_time() - t0;
                if (t > 100000)
                {
                    State = 0;
                }
            } 

      break;
  default:
    digitalWrite(Enable, false);
    ledcWrite(PWMChannel,0);
}
Serial.print(t);
Serial.print(", ");
Serial.println(State);
//delay(500);
}