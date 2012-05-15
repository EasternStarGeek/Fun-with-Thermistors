
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
 
 Concepts developed and demonstrated here:
 1. Using a Lookup Table (LUT)
 2. Interpolating between LUT entries for greater precision
 3. How to do a running average, or "Low-Pass Filter" in quasi real-time
 4. How to limit the resolution to match the limitations of the system
 
 */

// ################################################################################
// #################### PRE-PROCESSOR DEFINITIONS #################################
// ################################################################################

#define pinTherm A4
#define printInterval 3000


// ################################################################################
// ######################## VARIABLE DECLARATIONS #################################
// ################################################################################

float tempC;
float tempF;
float avgTempF;
float avgTempC;

boolean initLPF = 1;  // Flag used to initialize the Low Pass Filter

unsigned long millisPrev;
unsigned long millisPrevPrint = -printInterval;


// ################################################################################
// ######################## SETUP PROCEDURE #######################################
// ################################################################################

void setup() {
  // This is the Setup part of your project. This is for code that is run only ONCE at startup.

  pinMode(pinTherm, INPUT);  // Set Pin Mode of the Analog thermistor input
  analogReference(EXTERNAL); // Use the voltage appearing at AREF pin as the ADC reference
  Serial.begin(9600);
  
}  // END Setup Procedure


// ################################################################################
// ######################### LOOP PROCEDURE #######################################
// ################################################################################

void loop() {
  // This is the main part of your project.  Put all continuously running code here:

  if (millis() > millisPrev + 1000) {  // Take a temperature reading every second

    tempC = getTempFloat(pinTherm);  // Get instantaneous temperature reading in deg C

    if (tempC > 998)
      Serial.println("Error: Sensor reading over range!");
    else if (tempC < -998)
      Serial.println("Error: Sensor reading under range!");
    else {

      tempC = tempC + 1.0;  // Calibration offset by empirical measurement
      
      // tempC = resDull(tempC);  // Optional function to limit resolution to 0.5 deg C
      
      tempF = (tempC * 1.8) + 32;  // convert instantaneous deg C reading to deg Fahrenheit

      avgTempC = LPF(tempC, 20, initLPF);  // Get LPF average reading over 20 samples
      initLPF = 0;  // Discard the initialization flag after the first reading is recorded
      
      avgTempF = (avgTempC * 1.8) + 32;  // convert average deg C to deg Fahrenheit
    
      
      if (millis() > millisPrevPrint + printInterval) {  // print out the readings
        Serial.print("Instantaneous Temperature Readings:  ");
        Serial.print(tempC, 2);
        Serial.print(" degC, ");
        Serial.print(tempF, 2);
        Serial.println(" degF");
        
        Serial.print("      Average Temperature Readings:  ");
        Serial.print(avgTempC, 1);
        Serial.print(" degC,  ");
        Serial.print(avgTempF, 1);
        Serial.println(" degF");
        Serial.println();
        millisPrevPrint = millis();
      }
    }
    millisPrev = millis();  
  }


}  // END Loop Procedure


















