import processing.serial.*;

//Serial myPort;  // Create object from Serial class
//String val;     // Data received from the serial port

//*********************************************
// Example Code for Designing UI for E-Tech (DUIET)
//Muca_v2_Gestures
// Rong-Hao Liang: r.liang@tue.nl
//*********************************************

final int TH_NOISE = 15; //threshold of noise
final float TH_S = 30; // unit: px
final float TH_R = 30; // unit: degree
final float TH_T = 30; // unit: px
int[] skippedRows ={}; //the row numbers to discard. e.g., {0, 11}
int[] skippedCols ={}; //the column numbers to discard. e.g., {0,20}
boolean vFlipped = true;  //flip the image vertically
boolean hFlipped = false; //flip the image horizontally
boolean bRawImage = true; //(press 'r' key to toggle) show the raw image
boolean bUniPolar = false; //(press 'u' key to toggle) true: show only positive components; false: show both positive and negative components.
boolean bShowContext = true; //(press 'u' key to toggle) show the context of signal processing.

int EXAMPLE_MODE = 1; // 0: nothing; 1: discrete gestures; 2: continuous gestures

void setup() {
  size(720, 630);
  initSerial(); //initialize the serial port
  initParam(); //initialize the parameters
}

void draw() {
  background(0);
  if (bRawImage) { //press 'r' to toggle bRawImage
    drawCalibedImage(); //draw and update the raw image (row x column) 
    translate(iF*row, 0);
    drawBackgroundImage(); //draw the background image (row x column) used for background subtraction
  } else {
    updateRawImage(); //update the rawimage im(row x column) without drawing
    updateInterpolatedImage(); //bi-cubically interpolated image (iF*row x iF*column) without drawing
    updateBlobsAndTouchManager(); //extract blobs from the interpolated image using TH_NOISE, and update the touchmanager that resolves the order of touch events.
    updateMetrics(); //compute the metrics that define the gestures.
    if (bShowContext) {
      pushMatrix();
      drawInterpolatedImage(); //bi-cubically interpolated image (iF*row x iF*column)
      translate(iF*row, 0);
      drawBlobs(); //draw the blobs
      tm.draw(); //draw the touch points detected by the touchmanager.
      if (tm.getNumberOfBlobs()>1) drawGestureProxy(m.x, m.y, d, theta);
      popMatrix();
    }
    
    //Use and Modify the code below for your own application.
    pushMatrix();
    if (tm.getNumberOfBlobs()<=1) {
      drawGestureType(scale, rotation, translation); //check this function for the triggers of discrete gestures.
    } else if (tm.getNumberOfBlobs()>1) {
      drawInfoText(0, height/2); //check this function for the handles of continuous gestures.
    }
    popMatrix();
  }
}
