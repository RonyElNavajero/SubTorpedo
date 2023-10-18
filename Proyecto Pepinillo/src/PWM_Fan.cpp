#include <Arduino.h>
int State = 0;
int Switch = 21;
int Sensor = 18;
unsigned long RPMs = 0;
#define PWM 17 //PWM Pin
// setting PWM properties
const int freq = 5000;
const int PWMChannel = 0;
const int resolution = 10; //Resolution 8, 10, 12, 15
int Throttle = 0;
unsigned long t = 0;
unsigned long t0;
unsigned long Tick = 0;

void IRAM_ATTR isr() 
{
	Tick++;
}

void setup() 
{
Serial.begin(115200);
pinMode (Sensor, INPUT);                //Sensor Input
pinMode(PWM,OUTPUT);                    //PWM Output
pinMode (Switch, INPUT);                 //Switch Input

// configure LED PWM functionalitites
ledcSetup(PWMChannel, freq, resolution);

// attach the channel to the GPIO2 to be controlled
ledcAttachPin(PWM, PWMChannel);
attachInterrupt(Sensor, isr, RISING);
}

void loop() 
{

switch (State) 
{
    
    case 0:                                 // Etapa Inicial
        ledcWrite(PWMChannel,0);
        Throttle = 0;

        if (digitalRead(Switch) == true)
            {
                State = 1;
                delay(500);
            }
        break;

    case 1:
        ledcWrite(PWMChannel,200);
        Throttle = 0;
        t = esp_timer_get_time() - t0;
            if (t >= 1000000)
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

  default:
    ledcWrite(PWMChannel,0);
}
//Serial.print(t);
//Serial.print(", ");
//Serial.println(State);
//delay(500);
}
