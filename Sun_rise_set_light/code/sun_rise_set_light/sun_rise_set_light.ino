//                    _
// |\/| _  _  _  _  _|_) __  o _  __| _
// |  |(/_(_|(_|| |_>|  | (_)|(/_(_ |_>
//         _|               _|
//       www.megansprojects.com
// 
// Sun rise set light

#include <math.h>
#include <Adafruit_NeoPixel.h>

#define WS2812B_PIN     6
#define WS2812B_NM_LEDS 24

#define MANUAL_PIN      7
#define SET_PIN         8
#define RISE_PIN        9
#define INTENSITY_PIN   A5
#define SPEED_PIN       A6
#define GAMMA_PIN       A7

#define MODE_MANUAL     0
#define MODE_RISE       1
#define MODE_SET        2

#define SPEED_FACTOR    1000
#define MAX_COLOUR_TEMP 15000

Adafruit_NeoPixel strip = Adafruit_NeoPixel( WS2812B_NM_LEDS, WS2812B_PIN, NEO_GRB + NEO_KHZ800 );

void get_colour_values_from_temp (double *r, double *g, double *b, double temp_kelvin )
{
  double kelvin_scaled = temp_kelvin / 100.0;
  
  // Red
  if ( kelvin_scaled <= 66.0 )
  {
    *r = 255;
  }
  else
  {
    *r = kelvin_scaled - 60;
    *r = 329.698727446 * pow ( *r , -0.1332047592);
  }
  
  // Green
  if ( kelvin_scaled <= 66.0 )
  {
    *g = kelvin_scaled;
    *g = 99.4708025861 * log(*g) - 161.1195681661;
  }
  else
  {
    *g = kelvin_scaled - 60;
    *g = 288.1221695283 * pow (*g, -0.0755148492);
  }
  
  // Blue
  if ( kelvin_scaled >= 66 )
  {
    *b = 255;
  }
  else if ( kelvin_scaled <= 19 )
  {
    *b = 0;
  }
  else
  {
    *b = kelvin_scaled - 10;
    *b = 138.5177312231 * log(*b) - 305.0447927307;
  }

  if ( *r < 0 ) *r = 0;
  if ( *r >255 ) *r = 255;
  if ( *g < 0 ) *g = 0;
  if ( *g >255 ) *g = 255;
  if ( *b < 0 ) *b = 0;
  if ( *b >255 ) *b = 255;

  double norm_factor = 255.0 / ( *r + *g + *b );
  *r *= norm_factor;
  *g *= norm_factor;
  *b *= norm_factor;
  
}

void display_colour( double val )
{
  uint8_t col_triplet[WS2812B_NM_LEDS][3];
  double temp_col[3];
  double temp_k = val * 20000;
  get_colour_values_from_temp (&temp_col[0], &temp_col[1], &temp_col[2], val * MAX_COLOUR_TEMP );
   
  for ( int c=0; c < 3; c++)
  {
    if ( temp_k < 10000 )
    {
      temp_col[ c ] *= ( temp_k / MAX_COLOUR_TEMP );
    }

    for ( int i = 0; i < WS2812B_NM_LEDS; i ++)
    {
      col_triplet[ i ][ c ] = (uint8_t)temp_col[ c ];
    }
    double cfract =  WS2812B_NM_LEDS * ( temp_col[ c ] - floor(temp_col[ c ]) );
    int num_leds = (int) ( cfract );
    if ( num_leds > 0 )
    {
      double num_inc_col = WS2812B_NM_LEDS / (double) num_leds;
      double s = 0;
      while ( s < (  WS2812B_NM_LEDS - 1 ) )
      {
        int idx = floor( s );
        col_triplet[ idx ][ c ] ++;
        s += num_inc_col;
      }
    }
  }
   
  for ( int i = 0; i < WS2812B_NM_LEDS; i ++)
  {
    strip.setPixelColor(i, strip.Color(col_triplet[ i ][ 0 ], col_triplet[ i ][ 1 ], col_triplet[ i ][ 2 ])); 
  }   
  strip.show();
}

void setup()
{
  strip.begin();
  strip.setBrightness(255);
  strip.show(); // Initialize all pixels to 'off'
}

int mode = MODE_MANUAL;

void set_operation_mode( void )
{
  if ( 1 == digitalRead( SET_PIN ) )
  {
    mode = MODE_SET;
  }
  else if ( 1 == digitalRead( RISE_PIN ) )
  {
    mode = MODE_RISE;
  }
  else if ( 1 == digitalRead( MANUAL_PIN ) )
  {
    mode = MODE_MANUAL;
  }
}

double col = 0.0;
void loop()
{
  set_operation_mode();
  
  double col_delta = analogRead( SPEED_PIN );
  col_delta /= 1024.0;
  col_delta /= ( double )SPEED_FACTOR;
  double gamma = analogRead( GAMMA_PIN );
  gamma = 5.0 * ( gamma / 1024.0 );

  double delta;
  switch ( mode )
  {
    case MODE_SET:
      col -= col_delta;
      break;
    case MODE_RISE:
      col += col_delta;
      break;
    case MODE_MANUAL:
    default:
      col = analogRead(INTENSITY_PIN);
      col /= 1024.0;
      break;
  }
  
  if ( col < 0.0 ) col = 0.0;
  if ( col > 1.0 ) col = 1.0;

  double gamma_corrected_col = pow ( col , gamma );
  if ( gamma_corrected_col > 1.0 ) gamma_corrected_col = 1.0;
  display_colour ( gamma_corrected_col );
  delay(10);
}
