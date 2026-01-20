#include <LiquidCrystal.h>

void Correction (int x);
float CalculateRMSCurrent();
float CalculateCapacitance(float Vrms, float Irms, float pf);
float CalculateDur();
float CalculatePF(float y);
void OC_Print();

#define RS 7
#define E 8
#define D4 9
#define D5 10
#define D6 11
#define D7 12

#define PULSE_PIN 2  // XOR output from ZCD

// Capacitor Relay Pins
#define RELAY_10UF 3  
#define RELAY_20UF 4  
#define RELAY_40UF 5  
#define RELAY_80UF 6  

// Current Sensor Pins
#define CURRENT_SENSOR A0  
#define CURRENT_SENSOR_RMS A1  


const float Vrms = 230;
const float pi = 3.14159265359;
const float frequency = 50.0;


LiquidCrystal LCD(RS, E, D4, D5, D6, D7);

float PF,PF1, Angle, capacitance;
int dur = 0;
int Case = 0;
float IL, IL1;
float TimeD = 0; 
float TimeD1 = 0;

void setup() {
    Serial.begin(9600);
    
    LCD.begin(20, 4);
    LCD.setCursor(0, 0);
    LCD.print("PF Measurement");

    // Set pin modes
    pinMode(PULSE_PIN, INPUT);
    pinMode(CURRENT_SENSOR, INPUT);
    pinMode(CURRENT_SENSOR_RMS, INPUT); 

    pinMode(RELAY_10UF, OUTPUT);
    pinMode(RELAY_20UF, OUTPUT);
    pinMode(RELAY_40UF, OUTPUT);
    pinMode(RELAY_80UF, OUTPUT);

    // Turn off all capacitors initially
    digitalWrite(RELAY_10UF, LOW);
    digitalWrite(RELAY_20UF, LOW);
    digitalWrite(RELAY_40UF, LOW);
    digitalWrite(RELAY_80UF, LOW);
}


float CalculateDur(){
  float Samples[5];
  float TimeD = 0 ;
  for (int w=0; w<5; w++){
    Samples[w] = pulseIn(PULSE_PIN, HIGH);
    delay (300);
  }
  for (int u=0; u<5; u++){
    TimeD +=Samples[u];
  }
  TimeD = TimeD/5;
  return TimeD;
}


float CalculateRMSCurrent() {
    const int numSamples = 100;        
    const float sensitivity = 0.100;   
    const float VCC = 5.0;
    const float zeroCurrentVoltage = 2.5;
    const float samplingTime = 20.0 / numSamples;

    long sumSquared = 0;
    unsigned long startTime = millis();

    for (int i = 0; i < numSamples; i++) {
        int sensorValue = analogRead(CURRENT_SENSOR_RMS);
        float voltage = sensorValue * (VCC / 1023.0);
        float current = (voltage - zeroCurrentVoltage) / sensitivity;
        sumSquared += current * current;

        while (millis() - startTime < (i + 1) * samplingTime) {}
    }

    return sqrt(sumSquared / numSamples);
}


float CalculatePF(float TimeD){
  float Angle = (TimeD * frequency * 360.0) / 1e6;
  float PF = cos(Angle * 0.0174533);
  return PF;
}

float CalculateCapacitance(float Vrms, float Irms, float PF) {
    float theta = acos(PF);
    float sin_theta = sin (theta);
    float denominator = 2 * pi * frequency * Vrms * Vrms;
    float numerator = Vrms * Irms * sin_theta;
    float capacitance = numerator / denominator;
    
    //Serial.print("Capacitance: "); 
    //Serial.println(capacitance * 1e6, 3); // Convert to µF for display

    return capacitance;
}

void Correction(int x) {
    //Serial.print("Switching to Case: ");
    //Serial.println(x);

    digitalWrite(RELAY_10UF, x & 0b0001);
    digitalWrite(RELAY_20UF, x & 0b0010);
    digitalWrite(RELAY_40UF, x & 0b0100);
    digitalWrite(RELAY_80UF, x & 0b1000);
}

void OC_Print() {
    LCD.clear();
    LCD.setCursor(0, 1);
    LCD.print("        NO LOAD");
    LCD.setCursor(0, 2);
    LCD.print("       CONNECTED");
}


void loop() {
    // Wait for load connection
    /*
    IL = analogRead(CURRENT_SENSOR);
    delay(500);
    IL1 = analogRead(CURRENT_SENSOR);
    while (IL < 1 && IL1 < 1) {
        LCD.clear();
        OC_Print();
        delay (500);        //Measure();
    }
    */
    
    
    Correction(Case);
    

    TimeD = CalculateDur();
    PF = CalculatePF(TimeD);
    LCD.setCursor(0,1);
    LCD.print("PF before : ");
    LCD.print(PF);
    Serial.print("PF before : ");
    Serial.println(PF);
    float Irms = CalculateRMSCurrent();
    float capacitance = CalculateCapacitance (Vrms, Irms, PF);

    capacitance *= 1e6;  // Convert from Farads to microfarads
    if (capacitance < 10) Case = 0;
    else if (capacitance < 20) Case = 1;
    else if (capacitance < 30) Case = 2;
    else if (capacitance < 40) Case = 3;
    else if (capacitance < 50) Case = 4;
    else if (capacitance < 60) Case = 5;
    else if (capacitance < 70) Case = 6;
    else if (capacitance < 80) Case = 7;
    else if (capacitance < 90) Case = 8;
    else if (capacitance < 100) Case = 9;
    else if (capacitance < 110) Case = 10;
    else if (capacitance < 120) Case = 11;
    else if (capacitance < 130) Case = 12;
    else if (capacitance < 140) Case = 13;
    else if (capacitance < 150) Case = 14;
    else Case = 15;

    Correction (Case);
    float Timed1 = CalculateDur();
    PF1 = CalculatePF(Timed1);
    LCD.setCursor(0, 2);
    LCD.print("Capacitance : ");
    LCD.print(capacitance);
    Serial.print("Capacitance : ");
    Serial.println(capacitance);
    Serial.print("Irms : ");
    Serial.println(Trms);
    Serial.print("Case : ");
    Serial.println(Case);

    //LCD.setcursor(0,1);
    Serial.print("PF after : ");
    Serial.println(PF1);
    LCD.setCursor(0, 3);
    LCD.print("PF after : ");
    LCD.print(PF1);
    delay(500);
}
