
/*
Project Description:
 
 This project is a Template for all projects made using the Home School Shield.  
 The first thing you should do is to save this project to another name, and then
 put your own comments in this space.  It is always a good idea to document your projects!
 
 This project has been divided into five clearly marked areas:
 1. Pre-processor Definitions
 2. Variable Declarations
 3. Setup Procedure
 4. Loop Procedure
 5. Function Area
 
 You may not need all the areas for your project, but maintaining the template structure 
 will help keep your projects well-organized.
 */


// ################################################################################
// #################### PRE-PROCESSOR DEFINITIONS #################################
// ################################################################################

// Give symbolic names to all Home School Shield device Pin Connections:
#define pinS1 A2
#define pinPB1 8
#define pinPB2 4
#define pinPot A0
#define pinLDR A3
#define pinTherm A4
#define pinLED1 7
#define pinLED2 11
#define pinRelay 2
#define pinNPN1H 3
#define pinNPN2H 10
#define pinNPN3 5
#define pinNPN4 6
#define pinServo 9


// ################################################################################
// ######################## LOOKUP TABLE DECLARATIONS #############################
// ################################################################################



// ################################################################################
// ######################## VARIABLE DECLARATIONS #################################
// ################################################################################

// Example:  Declare variables to hold the value of the analog input devices:
int potValue;
int LDRValue;

float tempF;
float tempC;

unsigned long millisPrev;

// Example:  Declare variables to hold the state of the digital input devices:
boolean PB1State;
boolean PB2State;
boolean S1State;

boolean edgePB1;

// ################################################################################
// ######################## SETUP PROCEDURE #######################################
// ################################################################################

void setup() {
  // This is the Setup part of your project. This is for code that is run only ONCE at startup.

  // Homeschool Shield I/O Pin Modes
  // Warning!  Make no changes to Pin Modes!
  pinMode(pinS1, INPUT);
  pinMode(pinPB1, INPUT);
  pinMode(pinPB2, INPUT);
  pinMode(pinPot, INPUT);
  pinMode(pinLDR, INPUT);
  pinMode(pinTherm, INPUT);

  pinMode(pinLED1, OUTPUT);
  pinMode(pinLED2, OUTPUT);
  pinMode(pinRelay, OUTPUT);
  pinMode(pinNPN1H, OUTPUT);
  pinMode(pinNPN2H, OUTPUT);
  pinMode(pinNPN3, OUTPUT);
  pinMode(pinServo, OUTPUT); 
  // End Home School Shield Pin Modes

  // Add more Setup tasks if needed...
  analogReference(EXTERNAL);
  Serial.begin(115200);


}  // END Setup Procedure


// ################################################################################
// ######################### LOOP PROCEDURE #######################################
// ################################################################################

void loop() {
  // This is the main part of your project.  Put all continuously running code here:

  // Example: Get the instantaneous value from the three analog input devices
  potValue = analogRead(pinPot);
  LDRValue = analogRead(pinLDR);
  

  // Example: Get the instantaneous states of the three input switches.
  // Because of the nature of the external circuitry used, all switches on the 
  // Home School Shield will return a Logic-0 when they are in the
  // actuated state, so the negation operator (!) must be used.
  PB1State = !digitalRead(pinPB1);  
  PB2State = !digitalRead(pinPB2);
  S1State = !digitalRead(pinS1);

if (millis() > millisPrev + 2000) {
  
  tempC = getThermTemp(pinTherm) + 1.65;  // Add a Calibration Offset
  tempF = (tempC * 1.8) + 32;
  
  Serial.print("TempC: ");
  Serial.print(tempC);
  Serial.print("  TempF: ");
  Serial.println(tempF);
  
  millisPrev = millis();
}
  


}  // END Loop Procedure


// ################################################################################
// ##################### OUTBOARD FUNCTIONS #######################################
// ################################################################################

// If you are using your own Functions, put 'em here!

float getThermTemp (int thermPin)  {
  
/* The following lookup table contains the predicted ADC values for -20 deg C to + 69 deg C
   in a Voltage Divider Circuit with Ra=Thermistor and Rb=10K.
   +Vref---[Thermistor]---+--[10K]---GND
                          |
                         ADC @ thermPin

   ADC = 1023 * 10000/(Rtherm+10000)
   ADC Values were externally calculated from the Thermistor Resistance Table
*/
const int LUT_Therm[90] = {
  105, 110, 116, 121, 127, 133, 139, 145, 152, 159,   // -20C to -11C 
  165, 173, 180, 187, 195, 203, 211, 219, 227, 236,   // -10C to -1C 
  245, 254, 264, 273, 283, 293, 303, 313, 324, 334,   //  0C  to +9C 
  345, 355, 366, 377, 388, 399, 410, 422, 433, 444,   //  10C to 19C 
  455, 467, 478, 489, 500, 512, 523, 534, 545, 555,   //  20C to 29C 
  566, 577, 588, 598, 608, 619, 629, 639, 648, 658,   //  30C to 39C 
  667, 677, 686, 695, 704, 712, 721, 729, 737, 745,   //  40C to 49C 
  753, 760, 768, 775, 782, 789, 795, 802, 808, 814,   //  50C to 59C 
  820, 826, 832, 837, 843, 848, 853, 858, 863, 867    //  60C to 69C 
};

float tempC;
int ADC_Lo;
int ADC_Hi;
int Temp_Lo;
int Temp_Hi;


int thermValue = analogRead(thermPin);

// Sensor Data bounds check
    if (thermValue < LUT_Therm[0]) 
      tempC = -999;  // Under-range dummy value
    else if (thermValue > LUT_Therm[89])
      tempC = 999;  // Over-range dummy value
    else {

      // if Sensor Value is within range...
      for (int i=0; i <= 89; i++){
        if (LUT_Therm[i] > thermValue) {  // Look for a match in the lookup table
          int ADC_Hi = LUT_Therm[i];
          int Temp_Hi = i - 20;

          if (i != 0)  {
            int ADC_Lo = LUT_Therm[i-1];
            int Temp_Lo = i - 21;
          }
          else  {
            ADC_Lo = LUT_Therm[i];
            Temp_Lo = i - 20;
          }

          // Interpolate the temperature value for greater precision
          tempC = float( map(thermValue, ADC_Lo, ADC_Hi, Temp_Lo*100, Temp_Hi*100) )/100;
          
          break;  // exit for-next loop after the match is detected
        }
      }
    }
    return (tempC);
  }




// END Outboard Function Area











