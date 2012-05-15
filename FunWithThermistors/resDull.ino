
/*
 ************************************************
 ************** DULL RESOLUTION *****************
 ************************************************
 
 EasternStarGeek: 15 May, 2012
 
 This function will dull the resolution of a floating-point number to 0.5,
 and can be used with the Thermistor read function if desired.  This is because
 the tolerance stack-up of all related components and the analog signal chain 
 results in a practical useable resolution of approximately 0.5 deg C.
 
 This function will work with positive and negative input values.
 */

float resDull(float input) {

  float output;

  if ( abs(input - int(input)) > 0.5 )  {
    if (input > 0)
      output = int(input) + 0.5;
    else 
      output = int(input) - 0.5;
  }
  else
    output = int(input);

  return(output);

}



