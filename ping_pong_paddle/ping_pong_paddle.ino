/*
 * Ping Pong Paddle Project code
 * 
 * By Sadie Thomas and Alex Prestwich
 */

//libraries for accelerometer data
#include <Arduino_LSM9DS1.h>

//libraries for neopixel lights
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

//threshold of significance in G's
#define ACCELERATION_THRESHOLD 5

//digital pins
#define SPEAKER_PIN 11
#define LIGHTS_PIN 6

//how many NeoPixels are attached to the Arduino
#define NUM_PIXELS 32

//neopixel object to interact with lights
Adafruit_NeoPixel pixels(NUM_PIXELS, LIGHTS_PIN);

//miscelaneous variables
float aX, aY, aZ, aSum;
int red, green, blue;


//FUNCTION PROTOTYPES
void hit_animation();


void setup() {
  //initialize the IMU
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  //seed random with data from the board
  randomSeed(analogRead(0));

  //initilize NeoPixel strip object
  pixels.begin();
  pixels.clear();
  pixels.show();
  //startup_show();
}

void loop() {
  if (IMU.accelerationAvailable()) {
    //read the acceleration adata
    IMU.readAcceleration(aX, aY, aZ);
    
    //sum up the absolutes
    aSum = fabs(aX) + fabs(aY) + fabs(aZ);
    
    //check if it's above the threshold
    if (aSum >= ACCELERATION_THRESHOLD){
      Serial.println("Motion detected!");
      hit_animation();
    }
  }
}

//play sound and turn on lights
void hit_animation(){
  tone(SPEAKER_PIN, 200);

  for(int i=0; i<NUM_PIXELS; i++){
    //pick random RGB value with brightness between 0 and 100
    red = random(0, 100);
    blue = random(0, 100);
    green = random(0, 100);
    pixels.setPixelColor(i, pixels.Color(red, green, blue));
    pixels.show();
    delay(40);
  }
  pixels.clear();
  pixels.show();
  
  noTone(SPEAKER_PIN);
}
