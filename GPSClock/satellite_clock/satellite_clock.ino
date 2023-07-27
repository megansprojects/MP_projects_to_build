
//                    _
// |\/| _  _  _  _  _|_) __  o _  __| _
// |  |(/_(_|(_|| |_>|  | (_)|(/_(_ |_>
//         _|               _|
//       www.megansprojects.com
// 
// satellite clock

#include <Dusk2Dawn.h>
#include <Adafruit_GPS.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#ifndef PSTR
 #define PSTR // Make Arduino Due happy
#endif

#define LDR_Pin   A0
#define DST_Pin   A1
#define PIN       9

#define NUMPIXELS 512
#define mw 64
#define mh 8
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(mw, mh,  PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);

#define GPSSerial Serial1
Adafruit_GPS GPS(&GPSSerial);
#define GPSECHO  false

void setup()
{
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(40);
  matrix.setTextColor(matrix.Color(255, 0, 0));

  Serial.begin(115200);
  Serial.println("Megansprojects satellite clock");

  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate
  GPS.sendCommand(PGCMD_ANTENNA);

  delay(1000);
  GPSSerial.println(PMTK_Q_RELEASE);
}

#define LOOPS_PER_MSG  4
uint32_t timer = millis();
char dispstr[100]="";
int displen;
int loopcount=0;
int offset=0;

const uint16_t colors[] = {
  matrix.Color(200, 80, 30), //
  matrix.Color(70, 255, 0), //date
  matrix.Color(38, 230, 0), //lat
  matrix.Color(38, 230, 0), //long
  matrix.Color(255, 180, 25), //rise
  matrix.Color(204, 34, 0) //set
};

void showfullmessage(char *msg, int col)
{
  displen=(strlen(dispstr)*6)-1;
  matrix.fillScreen(0);
  offset = 64 - displen;
  offset /=2;
  matrix.setTextColor(matrix.Color(179,29,255));
  matrix.setCursor(offset, 0);
  matrix.print(dispstr);

}

void showmessage(char *msg, int col, int hour, int min)
{
  matrix.fillScreen(0);
  matrix.setTextColor(matrix.Color(102,0,204));
  matrix.setCursor(0,0);
  matrix.print(hour/10);
  matrix.setCursor(5,0);
  matrix.print(hour%10);
  matrix.setCursor(8,0);
  matrix.print(":");
  matrix.setCursor(11,0);
  matrix.print(min/10);
  matrix.setCursor(16,0);
  matrix.print(min%10);
  matrix.setTextColor(colors[col]);
  displen=(strlen(dispstr)*6)-1;
  for (int i=0; i < displen; i++)
  {
    matrix.setCursor(22+5*i, 0);
    matrix.print(dispstr[i]);
  }

}

int TZ=0;

void loop()                     // run over and over again
{
  GPS.read();
  if (GPS.newNMEAreceived())
  {
    GPS.parse(GPS.lastNMEA());
  }

  // approximately every 2 seconds or so, print out the current stats
  if (millis() - timer > 500) 
  {
    int ldr = analogRead(LDR_Pin);
    if ( ldr < 20 ) { ldr = 20;};
    if ( ldr > 200 ) { ldr = 200;};
    matrix.setBrightness(ldr);
    if ( analogRead(DST_Pin) > 100 )
    {
      TZ=1;
    }
    else
    {
      TZ=0;
    }
    timer = millis(); // reset the timer

    if ( loopcount < ( LOOPS_PER_MSG * 6 ))
    {
      sprintf(dispstr,"%02d:%02d:%02d",(GPS.hour+TZ)%24,GPS.minute,GPS.seconds);
      showfullmessage(dispstr,0);
    }
    else if ( 
      ( loopcount >= ( LOOPS_PER_MSG * 6)) && 
      ( loopcount <= ( LOOPS_PER_MSG * 7)) )
    {
      sprintf(dispstr,"%02d.%02d.%02d",GPS.day,GPS.month,GPS.year);
      showmessage(dispstr,1,(GPS.hour+TZ)%24,GPS.minute);
    }
    else if ( 
      ( loopcount >= ( LOOPS_PER_MSG * 7)) && 
      ( loopcount <= ( LOOPS_PER_MSG * 8)) )
    {
      float latitude = (float)GPS.latitudeDegrees;
      float longitude = (float)GPS.longitudeDegrees;
      Dusk2Dawn srs(latitude, longitude, 0);
      int Sunrise  = srs.sunrise(2000+GPS.year,GPS.month, GPS.day, false);
      char minst[6]="";
      Dusk2Dawn::min2str(minst, Sunrise);
      sprintf(dispstr,"   %s",minst);
      showmessage(dispstr,4,(GPS.hour+TZ)%24,GPS.minute);
      matrix.fillCircle( 27,  7,  3, colors[4]);
      matrix.drawLine( 22,  7,  33,  7, colors[0]);
      matrix.drawLine( 33, 5,  33, 1, colors[4]);
      matrix.drawLine( 33, 1,  32, 2, colors[4]);
      matrix.drawLine( 33, 1,  34, 2, colors[4]);
    }
    else if ( 
      ( loopcount >= ( LOOPS_PER_MSG * 8)) && 
      ( loopcount <= ( LOOPS_PER_MSG * 9)) )
    {
      float latitude = (float)GPS.latitudeDegrees;
      float longitude = (float)GPS.longitudeDegrees;
      Dusk2Dawn srs(latitude, longitude, TZ);
      int Sunset   = srs.sunset(2000+GPS.year,GPS.month, GPS.day, false);
      char minst[6]="";
      Dusk2Dawn::min2str(minst, Sunset);
      sprintf(dispstr,"   %s",minst);
      showmessage(dispstr,5,(GPS.hour+TZ)%24,GPS.minute);
      matrix.fillCircle( 27,  7,  3, colors[5]);
      matrix.drawLine( 22,  7,  33,  7, colors[0]);
      matrix.drawLine( 33, 5,  33, 1, colors[5]);
      matrix.drawLine( 33, 5,  32, 4, colors[5]);
      matrix.drawLine( 33, 5,  34, 4, colors[5]);   
      }
    else
    {
      loopcount=0;
    }
    loopcount++;
      matrix.show();
  }

}
