#include <Arduino.h>
#include "esp_timer.h"
#include <esp32-hal-ledc.h>
int State = 0;
unsigned long RPMs = 0;
#define Switch 21
#define Sensor 18
#define PWM 16 //PWM Pin
#define Enable 5 //Enable Pin
// setting PWM properties
const int freq = 10000;
const int PWMChannel = 0;
const int resolution = 10; //Resolution 8, 10, 12, 15
int Throttle = 0;
int64_t t = 0;
int64_t t0;
unsigned long Tick = 0;

void IRAM_ATTR isr() 
{
	Tick++;
}

void setup() 
{
digitalWrite(Enable, false);
Serial.begin(115200);
pinMode (Sensor, INPUT_PULLUP);                //Sensor Input
pinMode(PWM,OUTPUT);                    //PWM Output
pinMode(Enable,OUTPUT);                    //Enable Output
pinMode (Switch, INPUT);               //Switch Input
State = 0;

// configure LED PWM functionalitites
ledcSetup(PWMChannel, freq, resolution);

// attach the channel to the GPIO2 to be controlled
ledcAttachPin(PWM, PWMChannel);
attachInterrupt(Sensor, isr, RISING);
}

void loop() 
{

/*switch (State) 
{
    
    case 0:                                 // Etapa Inicial
        digitalWrite(Enable, false);
        ledcWrite(PWMChannel,0);

        if (digitalRead(Switch) == true)
            {
                State = 1;
                delay(500);
                t0 = esp_timer_get_time();
            }
        break;

    case 1:*/
        if (Serial.available())
        {
            Throttle = Serial.parseInt();
        }
        digitalWrite(Enable, true);
        ledcWrite(PWMChannel,Throttle);
        t = esp_timer_get_time() ;
            if ((t- t0) >= 1000000)
            {
                RPMs = Tick;
                Tick = 0;
                Serial.println(RPMs);
                t0 = t;
            }
        if (digitalRead(Switch) == true)
            {
                State = 0;
                delay(500);
            }

  //default:
    //ledcWrite(PWMChannel,0);
//}
}
