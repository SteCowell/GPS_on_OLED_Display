// =======================================================================
// Program : GPSTest3_GPS_and_Screen
// Purpose : GPS Read and Display position, speed and satellite count on screen.
// Author  : Steve Cowell
// Date    : January 23, 2018
// =======================================================================

// Initialise Screen -----------------------
#include "U8glib.h"
U8GLIB_SH1106_128X64 u8g(10, 7, 12, 11, 13);

// Initialise GPS --------------------------
#include <NMEAGPS.h>
NMEAGPS gps;

// Variables ---------------------
 int numSats = 0;
 double numLatitude ;
 double numLongitude ;
 double numSpeed ;
 double numAltitude ;
 double tmpalttd ;
// Variables used to establish display values (until I find a better way to do it!!!)
 double worknumvalin ;
 int    myworknum1 ;
 int    myworknum2 ;
 String myworkstrNS = "";
 String myworkstrEW = "";
 String myworkstr1    = "";
 String myworkstr2    = "";
 String myworkstr3    = "";
 char   workNumStr[9];


// Serial port for the GPS =================================================================
 #include <AltSoftSerial.h>
 AltSoftSerial gpsPort;  // pin 8 to GPS TX, pin 9 to GPS RX

// Setup ==================================================================================
void setup()
{
// Screen ---------------------------------
  // assign default color value Black and White Pixel ON
    u8g.setColorIndex(1);    
    pinMode(7, OUTPUT);

  Serial.begin(9600);
  
// GPS ------------------------------ 
   gpsPort.begin(9600);
}

// ===================================================== GPS READ START ==========================================
void gpsvalues()
{
//  Serial.println("At Start of VOID GPSVALUES --------------------");
 // GPS Read and Define Display Values --------------------------------------------
  while(gps.available( gpsPort ))
  {
   gps_fix fix = gps.read();
// Satellite FIX -------------------------------------------
    if (fix.valid.satellites)
        numSats = fix.satellites;
// Latitude and Logtitude FIX ------------------------------
    if (fix.valid.location)
        numLatitude  = fix.latitude();  
        numLongitude = fix.longitude();
// Speed ---------------------------------------------------
    if (fix.valid.speed)
        numSpeed = fix.speed_mph();
// Altitude ------------------------------------------------
    if (fix.valid.altitude)
        numAltitude = (fix.altitude() * 3.28084 );
 } 

  
// Satellites -----------------------------------------------------------
//  Serial.println("At Satellites --------------------");
    char satNumStr[9];
    sprintf (satNumStr, "%d", numSats);
    u8g.drawStr(72, 21, satNumStr); 

// Speed ----------------------------------------------------------------- 
//  Serial.print("At Speed -------------------- ");
//  Serial.print(numSpeed);
//  Serial.print(" is converted to ");
    if (numSpeed < 1)
    numSpeed = 0;
    worknumvalin = numSpeed;
//  Serial.println(worknumvalin);
    worknum();
    u8g.drawStr(72, 30, workNumStr);

// Altitude --------------------------------------------------------------  
//  Serial.print("At Altitude -------------------- "); 
//  Serial.println(numAltitude);
    if (numAltitude == 0)
       {worknumvalin = tmpalttd;}
       else 
       { worknumvalin = numAltitude; } 
       
    tmpalttd = numAltitude;
    worknum();
    u8g.drawStr(72, 39, workNumStr);
    
// Latitude --------------------------------------------------------------
//  Serial.println("At Latitude --------------------");
    if (numLatitude == 0)
    { 
      worknumvalin = (numLatitude);
      myworkstrNS = " ";
    } else if (numLatitude < 0)
           {
            worknumvalin = (numLatitude * -1);
            myworkstrNS = "S";
            } else {
              worknumvalin = (numLatitude);
              myworkstrNS = "N";
            }
    worknum();
    u8g.drawStr(72, 48, workNumStr); 
    
// Longitude -------------------------------------------------------------
//  Serial.println("At Longitude --------------------");
    if (numLongitude == 0)
    { 
      worknumvalin = (numLongitude);
      myworkstrEW = " ";
    } else if (numLongitude < 0)
           {
            worknumvalin = (numLongitude * -1);
            myworkstrEW = "W";
            } else {
              worknumvalin = (numLongitude);
              myworkstrEW = "E";
            }
//    worknumvalin = numLongitude;
    worknum();
    u8g.drawStr(72, 57, workNumStr);  
}
// ===================================================== GPS READ END ============================================

// ===================================================== WORKNUM START ===========================================
// I persevered trying to get the display to show the 99.99 value that was coming in from the 'fix.' value but in the
// end I gave up and decided to do it the long way round!!!!
void worknum()
{
//    Serial.println("At Start of VOID WORKNUM --------------------");
    myworknum1 = worknumvalin;
    myworknum2 = ((worknumvalin - myworknum1) * 100);
    myworkstr1 = String(myworknum1);
    myworkstr2 = String(myworknum2);
    myworkstr3 = (myworkstr1 + ":" + myworkstr2);
    sprintf (workNumStr, "%s", myworkstr3.c_str());
}
// ===================================================== WORKNUM END =============================================

// ===================================================== DRAW START ==============================================
// Calculate and Display Values
void draw()
{
//    Serial.println("At Start of VOID DRAW --------------------");
    u8g.setFont(u8g_font_profont10);
    u8g.drawStr( 30, 10, "---- GPS ----");
    u8g.drawStr(2, 21, "Satellites  :");
    u8g.drawStr(2, 30, "Speed(mph)  :");
    u8g.drawStr(2, 39, "Altitude(ft):");
//  u8g.drawStr(2, 48, "Latitude    :");
    String displayLatLon = ("Latitude  " + myworkstrNS + " :");
    sprintf (workNumStr, "%s", displayLatLon.c_str());
    u8g.drawStr(2, 48, workNumStr);
//    u8g.drawStr(2, 57, "Longitude   :");
    displayLatLon = ("Longitude " + myworkstrEW + " :");
    sprintf (workNumStr, "%s", displayLatLon.c_str());
    u8g.drawStr(2, 57, workNumStr);
}
// ===================================================== DRAW END   ==============================================


// ===================================================== LOOP START ==============================================

void loop()
{
  // picture loop
  u8g.firstPage();  
  do {
    gpsvalues();
    draw();
  } while( u8g.nextPage() ); 
} 
// ===================================================== LOOP END ================================================ 

