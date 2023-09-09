
//                    _
// |\/| _  _  _  _  _|_) __  o _  __| _
// |  |(/_(_|(_|| |_>|  | (_)|(/_(_ |_>
//         _|               _|
//       www.megansprojects.com
// 
// Baulders gate 3 laser cut picture

#define FIRE_PIN_1 3
#define FIRE_PIN_2 5
#define FIRE_PIN_3 6
#define LAVA_PIN_1 9
#define LAVA_PIN_2 10
#define LAVA_PIN_3 11

typedef struct 
{
  int pin;
  int min; // minimum PWN value
  int max; // maximum PWN value
  int delta; // Rate of change per cycle
  int wait; // time to wait for update#
  int value;
  int time;
} lightEffect_t;

#define NUM_LIGHTS 6
#define WAIT_TIME_MS 10

lightEffect_t lights[NUM_LIGHTS];


#define FLAME_MIN 0
#define FLAME_MAX 255
#define FLAME_DELTA 255
#define FLAME_WAIT 5

#define LAVA_MIN 100
#define LAVA_MAX 255
#define LAVA_DELTA 20
#define LAVA_WAIT 10

void initLight ( lightEffect_t *pLe, int pin, int min, int max, int delta, int wait)
{
  pLe->pin = pin;
  pLe->min = min;
  pLe->max = max;
  pLe->delta = delta;
  pLe->wait = wait;
  pLe->value = min;
  pLe->time = wait;

  pinMode( pin, OUTPUT );
  analogWrite( pin, min );
}

void runLight ( lightEffect_t *pLe )
{
  if ( 0 == pLe->time-- )
  {
    pLe->time = pLe->wait;
    pLe->value += random( 2 * pLe->delta ) - pLe->delta;
    if ( pLe->value  < pLe->min )
    {
      pLe->value = pLe->min;
    }
    if ( pLe->value  > pLe->max )
    {
      pLe->value = pLe->max;
    }
  }
}

void setup()
{
  Serial.begin(9600);

  initLight ( &lights[0], FIRE_PIN_1, FLAME_MIN, FLAME_MAX, FLAME_DELTA, FLAME_WAIT );
  initLight ( &lights[1], FIRE_PIN_2, FLAME_MIN, FLAME_MAX, FLAME_DELTA, FLAME_WAIT );
  initLight ( &lights[2], FIRE_PIN_3, FLAME_MIN, FLAME_MAX, FLAME_DELTA, FLAME_WAIT );
  initLight ( &lights[3], LAVA_PIN_1, LAVA_MIN, LAVA_MAX, LAVA_DELTA, LAVA_WAIT );
  initLight ( &lights[4], LAVA_PIN_2, LAVA_MIN, LAVA_MAX, LAVA_DELTA, LAVA_WAIT );
  initLight ( &lights[5], LAVA_PIN_3, LAVA_MIN, LAVA_MAX, LAVA_DELTA, LAVA_WAIT );

}

void loop()
{
  for ( int i= 0; i < NUM_LIGHTS; i ++)
  {
    runLight ( &lights[i] );
    delay ( WAIT_TIME_MS );
  }
}
