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
#define ACCELERATION_THRESHOLD 3

//digital pins
#define SPEAKER_PIN 11
#define LIGHTS_PIN 6

//how many NeoPixels are attached to the Arduino
#define NUM_PIXELS 31

//neopixel object to interact with lights
Adafruit_NeoPixel pixels(NUM_PIXELS, LIGHTS_PIN);

//miscelaneous variables
float aX, aY, aZ, aSum;
int red, green, blue;


//FUNCTION PROTOTYPES
void hit_animation();
void startup_show();


void setup() {
  //initialize the IMU
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while(1);
  }

  Serial.println("Produced by the Ping Pong Paddle Project Professionals");

  //seed random with data from the board
  randomSeed(analogRead(0));

  //initilize NeoPixel strip object
  pixels.begin();
  pixels.clear();
  pixels.show();
  startup_show();
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
  int frequency;
  for(int i=0; i<NUM_PIXELS; i++){
    //
    frequency = random(50, 400);
    tone(SPEAKER_PIN, frequency, 18);
    
    //pick random RGB value with brightness between 0 and 100
    red = random(0, 100);
    green = random(0, 100);
    blue = random(0, 100);
    pixels.setPixelColor(i, pixels.Color(red, green, blue));
    pixels.show();
    delay(30);
  }
  pixels.clear();
  pixels.show();
}


//light animation show
void startup_show(){
  //RBG values for orange
  red = 20;
  green = 5;
  blue = 0;

  //make all the pixels white
  pixels.fill(pixels.Color(20, 20, 20));

  //trail of 3 pixels that turns orange going from left to right
  //the front orange pixel is brighter than the second and the second is brighter than the last
  for(int i=0; i<NUM_PIXELS; i++){
    //turn the back pixel back to white
    if(i >= 3){
      pixels.setPixelColor(i-3, pixels.Color(20, 20, 20));
    }

    //turn the three pixels orange (vary brightness)
    pixels.setPixelColor(i, pixels.Color(3*red, 3*green, 3*blue));
    if(i >=1){
      pixels.setPixelColor(i-1, pixels.Color(2*red, 2*green, 2*blue));
    }
    if(i >= 2){
      pixels.setPixelColor(i-2, pixels.Color(red, green, blue));
    }

    //show updated pixels and wait before continuing
    pixels.show();
    delay(40);
  }

  //do the same animation but from right to left
  for(int i=NUM_PIXELS-1; i>=0; i--){
    if(i <= NUM_PIXELS-4){
      pixels.setPixelColor(i+3, pixels.Color(20, 20, 20));
    }
    pixels.setPixelColor(i, pixels.Color(3*red, 3*green, 3*blue));
    if(i <= NUM_PIXELS-2){
      pixels.setPixelColor(i+1, pixels.Color(2*red, 2*green, 2*blue));
    }
    if(i <= NUM_PIXELS-3){
      pixels.setPixelColor(i+2, pixels.Color(red, green, blue));
    }
    pixels.show();
    delay(40);
  }

  //turn off lights at end
  pixels.clear();
  pixels.show();
}
