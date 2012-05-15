
/*
 ************************************************
 ************** LOW PASS FILTER *****************
 ************************************************
 
 EasternStarGeek: 15 May, 2012
 
 This function is a Low-Pass Filter for smoothing out signals or averaging.
 It uses a static buffer to store the last "n" readings, and each time it is called,
 the oldest reading is discarded, the new reading is added, and the stack average
 value is returned.
 The buffer size is user-adjustable, but is constrained between 2 and 50
 An initialization function fills the entire buffer with the input value. This is useful
 when the LPF function is called for the first time.
 
 Note: To meet the definition of a true LPF, this function must be called at regular intervals.
 */

float LPF( float input, int bufferSize, boolean initialize)  {

#define bufferCap 50  //maximum buffer capacity

  static float buffer[bufferCap];
  byte samples;
  float tempSum;
  float output;

  bufferSize = constrain(bufferSize, 2, bufferCap);

  // Initialize buffer if requested
  if (initialize) {
    for ( int i=0; i < bufferSize; i++ ) {
      buffer[i] = input;
    } // END For
  }  // END if

  // Push down the buffer Stack to discard oldest reading, making room for the new
  for ( int i=bufferSize - 2; i >= 0; i--) {
    buffer[i+1] = buffer[i];
  } // END For

  // Record the Current input value
  buffer[0] = input;

  // Calculate current stack average
  tempSum = 0;
  for ( int i=0; i < bufferSize; i++ ) {
    tempSum = tempSum + buffer[i];
  } // END For

  output = tempSum / bufferSize;

  return(output);


} // End function








