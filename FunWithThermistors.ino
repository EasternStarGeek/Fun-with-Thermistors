
/*
Project Description:
 
 This project illustrates a simple method of measuring temperature with an Arduino and NTC Thermistor.
 The Thermistor used for this project is sold by Adafruit Industries:
 http://www.adafruit.com/products/372
 and has a resistance of 10K ohms at 25 degrees C.  It is used in a simple voltage divider circuit.
 
 Since the response is non-linear, a lookup table is used.  The values in the LUT are the predicted
 values of the Analog to Digital converter at every temperature between -20 and +69 degrees C.
 
 The LUT has been derived from the manufacturer's published resistance table with the help of an
 Excel spreadsheet.
 
 The LUT table approach offers the advantage of higher processing speed
 
 */


// ################################################################################
// #################### PRE-PROCESSOR DEFINITIONS #################################
// ################################################################################


#define pinTherm A4


// ################################################################################
// ######################## VARIABLE DECLARATIONS #################################
// ################################################################################

// Example:  Declare variables to hold the value of the analog input devices:

float tempF;
float tempC;

unsigned long millisPrev;

// ################################################################################
// ######################## SETUP PROCEDURE #######################################
// ################################################################################

void setup() {
  // This is the Setup part of your project. This is for code that is run only ONCE at startup.

  pinMode(pinTherm, INPUT);
  analogReference(EXTERNAL);
  Serial.begin(115200);


}  // END Setup Procedure


// ################################################################################
// ######################### LOOP PROCEDURE #######################################
// ################################################################################

void loop() {
  // This is the main part of your project.  Put all continuously running code here:

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

// get raw ADC value from Thermistor voltage divider circuit
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











