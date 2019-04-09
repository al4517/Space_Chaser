/***************************************************************************************
*    Title: Space Chaser Gizmo
*    Author: Alison Luo
*    Date: 10/04/2019
*    Code version: 2
*    Availability: 
***************************************************************************************/
//Libraries used
#include <Servo.h>
#include <SparkFun_TB6612.h>
#include <pitches.h>
#include <LiquidCrystal595.h> 
//----DEFINING SERVO MOTORS---------------------------------------
Servo servo1;
Servo servo2;
//----------------------------------------------------------------
//Pin assignment                                              
int y_key = A1; 
int throttle=A0;                                              
int servo1_pin = 3;
int servo2_pin =5;
#define AIN1 2
#define AIN2 10
#define PWMA 11
#define STBY 6
LiquidCrystal595 lcd(7,8,9);     // datapin, latchpin, clockpin
//---------------------------------------------------------------
//Previous values for millis
unsigned long previousMillis = 0;
unsigned long previousMillis1 = 0;
unsigned long previousMillis3 = 0;
unsigned long previoustimer=0;
unsigned long prevshipservo=0;
unsigned long prevufoservo=0;
unsigned long prevlinearthrot=0;
unsigned long prevmotor=0;
unsigned long prevscore=0;
unsigned long previouscoredisp=0;
unsigned long prevsound=0;
unsigned long prevscrdisp=0;
//Current values for millis
unsigned long currentMillis;
unsigned long currentMillis1=0;
unsigned long currentMillis3=0;
//---------------------------------------------------------------
//Interval values for millis [this code uses polling to check if actions need to be taken
long interval=1000;       // therefore needs intervals for each action]
long timerinterval=500;
long timership=20;
long timerufo=1000;
long timerthrot=50;
long timermotor=50;
long timerscore=50;
long timerscoredisp=500;
long timersound=300;
long timerscrdisp=500;
//---------------------------------------------------------------
//Global variables
float velocity;   //Velocity of how fast the UFO changes position (servo motor)
int throtsense;   //Value of the linear potentiometer
int score=0;      //Initialises score
int motorSpeed = 0;   //Speed of DC motor 
int play=1;       //Play mode status 1=keep playing 0=stop playing
int endchoice;    //User input on if they want to play again
int y_pos;        //Value of joystick
int initial_position = 90;    //Initial positions for spaceship + UFO
int initial_position1 = 90;
int choice;   //Display instructions or not   
int soundscore=0;   //To notify score sound to play
int scoredisp=0;    //To notify score message display
int position;     //Position of UFO
//----------------------------------------------------
//Initialise DC motor
const int offsetA = 1;
const int offsetB = 1;
Motor motor1 = Motor(AIN1, AIN2, PWMA, offsetA, STBY);
//----------------------------------------------------
//initialise LCD screen
char number[] = "************************";
float countdown=60;
//----------------------------------------------------
//SOUNDS PIEZO SPEAKER
//Point scored melody
int win[] = {
  NOTE_C5, NOTE_D5
};
int winDurations[] = {
  8, 2
};
//-------------------------------------------------------------------
//Game over tune
int finish[] = {
  NOTE_G5, NOTE_C6, NOTE_G5, NOTE_C6, NOTE_G5, NOTE_C6, NOTE_C7, NOTE_C7, NOTE_C7
};
int finDurations[] = {
  8, 2, 8, 2, 8, 2, 4, 4, 4
};
//-------------------------------------------------------------------
//Background tune
int waiting[] = {
  NOTE_C4, NOTE_B4, NOTE_C5, NOTE_C4,NOTE_C4, NOTE_B4,NOTE_C5,NOTE_B3,NOTE_A4, NOTE_D5, NOTE_E5, NOTE_D5,NOTE_C5, NOTE_B4, NOTE_G5, NOTE_A5, NOTE_B4,NOTE_A4
};
int waitDurations[] = {
  2, 4,4, 2,2, 4,4,2,4,8,8,8,6,6,4,4,4,3,2
};
//------------------------------------------------------------------

void setup ( ) {

  TCCR1B = TCCR1B & B11111000 | B00000001;    //Set PWM frequency for D11 & D12 to 31kHZ
  TCCR2B = TCCR2B & B11111000 | B00000001;    //Set PWM frequency for D9 & D10 to 31kHZ to stop audible vibrations

  Serial.begin (9600) ;
  servo1.attach (servo1_pin ) ;     //Defining servo motor pins
  servo2.attach (servo2_pin ) ; 
  servo1.write (initial_position);
  servo2.write (initial_position1);
                       
  pinMode (y_key, INPUT) ;         //Defining joystick pin
  //SETUP LCD screen
  
  //3-pin Arduino interface for HD44780 LCDs via 74HC595 Shift Register code by Rowan Simms
  //http://rowansimms.com/article.php/lcd-hookup-in-seconds
  
  lcd.begin(16,2);             // 16 characters, 2 rows
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.setLED2Pin(HIGH);
  lcd.shift595();
}


void loop ( ) {
  servo1.write (4);
  servo2.write (180);   //initialise servo motors to their start positions
  lcd.clear();          //setting up LCD screen display
  lcd.setCursor(0,0);
  lcd.print(String("loading..."));
  previousMillis=millis();
  currentMillis=millis();
  while (currentMillis-previousMillis<500) {
    currentMillis=millis();
  }
  int instructions=initial_page();    //loads up initial page
  if (instructions==1) {              //if user has chosen instructions
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(String("Instructions:"));
    previousMillis=millis();
    currentMillis=millis();
    while (currentMillis-previousMillis<1000) {
      currentMillis=millis();
    }
    int readinstrucs;
    readinstrucs=instructioninfo();   //goes to instructions function
  }
  while (play==1) {   //while the user has not chosen to end game
    score=0;          //initialise score
    countdown=60;     //60 seconds a game
    lcd.clear();      
    lcd.setCursor(0,0);
    lcd.print(String("GAME STARTING"));
    previousMillis=millis();
    currentMillis=millis();
    while (currentMillis-previousMillis<2000) {
      currentMillis=millis();
    }
    int gamemode;
    gamemode=startgame();   //enters game mode function
    int endchoice;        
    endchoice=finalpage();  //enters game over page

  }
}


int initial_page ( ) {      //Loads first screen for user to choose INSTRUCTIONS or PLAY
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(String("SPACE CHASER"));
  lcd.blink();
  previousMillis=millis();    //Displays 'SPACE CHASER' for 1 second
  currentMillis=millis();
  while (currentMillis-previousMillis<1000) {
    currentMillis=millis();
  }
  lcd.noBlink();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(String("JOYSTK UP: PLAY"));   //Displays choices 
  lcd.setCursor(0,1);
  lcd.print(String("DOWN:INSTRUCTNS"));
  previousMillis=millis();
  currentMillis=millis();
  while (currentMillis-previousMillis<1500) {
    currentMillis=millis();
  }
  while (analogRead(y_key)>490 && analogRead(y_key)<510) {                  //while the joystick is at central pos
                                                                           //Play waiting tune
    //-----toneMelody code by Tom Igoe http://www.arduino.cc/en/Tutorial/Tone
    
    for (int thisNote = 0; thisNote < 19; thisNote++) {
      int noteDuration = 1000 / waitDurations[thisNote];
      tone(12, waiting[thisNote], waitDurations);
      int pauseBetweenNotes = 400;
      previousMillis=millis();
      currentMillis=millis();
      while (currentMillis-previousMillis<pauseBetweenNotes){
        currentMillis=millis();
      }
      noTone(12);
    }
    //-------  
  }
  choice=analogRead(y_key);  //Reads the joystick position
  if (choice < 490) {          //User chose UP for PLAY
    int instructions=0;      //Program will not show instructions
    return instructions;
  }
  if (choice > 510) {        //User chose DOWN for INSTRUCTIONS
    int instructions=1;     //Program will show instructions
    return instructions;
  }
}

int instructioninfo ( ) {   //Displays instructions of game
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(String("Chase the UFO"));   //First instruction page
  lcd.setCursor(0,1);
  lcd.print(String("with joystick"));
  previousMillis=millis();
  currentMillis=millis();
  while (currentMillis-previousMillis<3000) {   //Each page lasts for 3 secs
    currentMillis=millis();
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(String("to shoot and "));   //Second instruction page
  lcd.setCursor(0,1);
  lcd.print(String("score points"));
  previousMillis=millis();
  currentMillis=millis();
  while (currentMillis-previousMillis<3000) {
    currentMillis=millis();
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(String("Use throttle "));   //Third instruction page
  lcd.setCursor(0,1);
  lcd.print(String("For more speed"));
  previousMillis=millis();
  currentMillis=millis();
  while (currentMillis-previousMillis<3000) {
    currentMillis=millis();
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(String("+more POINTS"));    //Forth instruction page
  int timing;
  previousMillis=millis();
  currentMillis=millis();
  while (currentMillis-previousMillis<3000) {
    currentMillis=millis();
  }
}


int startgame ( ){      //Starts play mode
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(String("60 seconds"));
  previousMillis=millis();
  currentMillis=millis();
  while (currentMillis-previousMillis<2000) {
    currentMillis=millis();
  }
  while (countdown>0){
    currentMillis = millis();
  }
  if(currentMillis - previoustimer > timerinterval) {   //using polling to set time intervals for timer countdown                                                  
    previoustimer = currentMillis;                      //0.5 seconds per step
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(String("Time left: ")+String(countdown));   //Displays countdown
    countdown=countdown-0.5;                              //Updates countdown variable
  }
  if(currentMillis - previouscoredisp > timerscoredisp) {   //Updates score after set interval
    previouscoredisp = currentMillis;
    lcd.setCursor(0,1);
    lcd.print(String("Score:")+String(score));
  }
  if(currentMillis - prevshipservo > timership) {   //Checks position of joystick every 20ms
    
    //Servo control by joystick by Ali Hamza https://maker.pro/arduino/tutorial/how-to-control-servo-motors-with-an-arduino-and-joystick 
    
    prevshipservo = currentMillis;
    y_pos = analogRead (y_key) ;    //Reads joystick position                      
    if (y_pos > 700) {              //If joystick at down position
      if (initial_position1 < 31) { //If current position is less than lower boundary
      }                             //Do nothing because spaceship cannot move lower
      else { 
        initial_position1 = initial_position1 - 15;   //Updating position of spaceship
        servo2.write (initial_position1);             //Digital output to servo
      }  
    } 

    if (y_pos < 300) {               //If joystick at up position
      if (initial_position1 > 180) { //If current position is more than upper boundary
      }                              //Do nothing because spaceship cannot move higher
      else {
        initial_position1 = initial_position1 + 15;   //Updating position of spaceship
        servo2.write (initial_position1);             //Digital output to servo
      }
    }
  }
  
  if(currentMillis-prevufoservo > timerufo) {   //Updating position of UFO
    prevufoservo = currentMillis;   
    position=random (15,140);                   //Choosing random position for UFO
    servo1.write (position);                  //Updating servo position
  }
  
  if(currentMillis-prevlinearthrot > timerthrot) {    //Checking throttle speed
    prevlinearthrot = currentMillis;
    throtsense=analogRead (throttle);                 //Reading linear potentiometer value
    velocity = map(throtsense, 0, 1021,0, 100);       //Mapping digital input to 'velocity' variable for UFO speed
    motorSpeed = map(throtsense, 0, 1023,70, 150);    //Mapping digital input to change conveyor speed (DC motor)
    timerufo= 1000-(velocity*5);                      //Updating how fast the UFO changes position (lower=faster)
  }
  
  if(currentMillis-prevmotor > timermotor) {    //Updating speed of DC motor
      prevmotor = currentMillis;
      //TB6612FNG H-Bridge Motor Driver Library code by Michelle @ SparkFun Electronics https://github.com/sparkfun/SparkFun_TB6612FNG_Arduino_Library
      
      motor1.drive(motorSpeed);                 //Writing in updated value to DC motor via TB6612
  }
  
  if(currentMillis-prevscore > timerscore) {    //Checking position of spaceship relative to UFO
      prevscore = currentMillis;
      if (initial_position1 > (171-position) && initial_position1 < (181-position)) {   // range defined so easier to align the two characters
        score=score+(10+velocity);                                                 // If spaceship and UFO alligned add score which depends on speed of UFO
        scoredisp=1;    //Alerts code to display score message
        soundscore=1;   //Alerts code to play score sound
      }
  }
  
  if(currentMillis-prevsound > timersound) {    //Checks if score sound needs to be played
    prevsound = currentMillis;
    if (soundscore==1) {                        //If score sound needs to be played
      soundscore=0;
      for (int thisNote = 0; thisNote < 2; thisNote++) {
        int noteDuration = 1000 / winDurations[thisNote];
        tone(12, win[thisNote], winDurations);
        int pauseBetweenNotes = 100;
        previousMillis3 = millis();
        currentMillis3 = millis();
        while (currentMillis3-previousMillis3<pauseBetweenNotes) {
          currentMillis3 = millis();
        }
        noTone(12);
      }
    }
    soundscore=0;
  }
  
  if(currentMillis-prevscrdisp > timerscrdisp) {    //Checks whether 'SCORE' message needs to be displayed
    prevscrdisp = currentMillis;
    if (scoredisp == 1) {
      scoredisp=0;
      lcd.setCursor(11,1);
      lcd.print(String("SCORE")+String(countdown)); //Displays temporary message
    }
  }
}


int finalpage ( ) {       //Game over page function
  for (int thisNote = 0; thisNote < 9; thisNote++) {    //Plays game over tune
    int noteDuration = 1000 / finDurations[thisNote];
    tone(12, finish[thisNote], finDurations);
    int pauseBetweenNotes = 200;
    previousMillis=millis();
    currentMillis=millis();
    while (currentMillis-previousMillis<pauseBetweenNotes) {
      currentMillis=millis();
    }
    noTone(12);
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Congratulations!");            //Displays final score
  lcd.setCursor(0,1);
  lcd.print("You scored:"+String(score));
  previousMillis=millis();
  currentMillis=millis();
  while (currentMillis-previousMillis<5000){
    currentMillis=millis();
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Play again?");             //Asks user whether they want to replay
  lcd.setCursor(0,1);
  lcd.print("Yes=UP");
  while (analogRead(y_key) > 490 && analogRead(y_key) < 510) {
  }                                               //While joystick is in central position do nothing
  endchoice=analogRead(y_key);                    //Reads joystick position
  if (endchoice < 490) {        //User chose up so play again
     play=1;
  }
  else {                        //User chose not to play again
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("SPACE CHASER");    //Display finish screen
    previousMillis=millis();
    currentMillis=millis();
    while (currentMillis-previousMillis<300000) {
      currentMillis=millis();
    }
  }
}
