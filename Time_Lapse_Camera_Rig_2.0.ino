//including libraries
#include <Stepper.h>            //stepper motor control library
#include <Wire.h>               //I2C library
#include <LiquidCrystal_I2C.h>  //lcd screen library

//defining what pins on stepper motor control board are connected to what pins on the Arduino board
#define HALFSTEP 4              //tells the program how many wires the motor has
#define motorPin1 A0
#define motorPin2 A1
#define motorPin3 A2
#define motorPin4 A3

//initializing the stepper motor
//NOTE* 2038 steps in one revolution
Stepper stepper1(HALFSTEP, motorPin1, motorPin3, motorPin2, motorPin4);

//setting up lcd screen with I2C address and screen size
const int LCD_Width = 20;
LiquidCrystal_I2C lcd(0x27,LCD_Width,4);

                                //VARIABLES

//variable to hold the constant value of steps per degree (with fudge to keep more accurate)
const int StepsPerDegree = 6;

//setting up variables for buttons and step counters etc.
int Counter = 0;                //counts steps
int DelayTime = 250;            //keeps track of delay between steps
int StepUp = 1;                 //steps to be taken at a time
int TotalSteps = 1;             //total steps to be taken
int Degrees = 5;                //degrees to be moved will be converted and saved to TotalSteps

//initializing buttons to control motor movement
const int ButtonOne = 2;        //increases DelayTime by 250 milliseconds
const int ButtonTwo = 3;        //decreases DelayTime by 250 milliseconds
const int ButtonThree = 4;      //increases StepUp by 1 step
const int ButtonFour = 5;       //decreases StepUp by 1 step
const int ButtonFive = 6;       //increase Degrees by 5 degrees
const int ButtonSix = 7;        //decrease Degrees by 5 degrees
const int ButtonSeven = 8;      //start button

//starting program with button pins set to low (off) and save button states
int ButtonState1 = LOW;
int ButtonState2 = LOW;
int ButtonState3 = LOW;
int ButtonState4 = LOW;
int ButtonState5 = LOW;
int ButtonState6 = LOW;
int ButtonState7 = LOW;

                               //FUNCTIONS

//function used to populate the screen with setup information
int PopulateScreen(){
  //first clear the screen
  lcd.clear();
  //print Delay line
  lcd.setCursor(0,0);
  lcd.print("Delay = ");
  //print Steps line
  lcd.setCursor(0,1);
  lcd.print("Steps = ");
  //print Degrees line
  lcd.setCursor(0,2);
  lcd.print("Degrees = ");
  //print button layout line
  lcd.setCursor(0,3);
  lcd.print((String)"D"+char(43)+" "+char(45)+"  S"+char(43)+" "+char(45)\
  +"  "+char(223)+char(43)+" "+char(45)+"  "+char(45)+char(62));
  //fill screen with current settings
  lcd.setCursor(8,0);
  String timeString = DelayString();
  lcd.print(timeString);
  lcd.setCursor(8,1);
  lcd.print((String)StepUp + "  - for CCW");
  lcd.setCursor(10,2);
  lcd.print((String)Degrees + char(223));
  lcd.setCursor(13,3);
  }

//function to display the progress screen
void ProgressScreen(int moved, float percent){
  lcd.clear();
  lcd.setCursor(6,1);
  lcd.print("Progress");
  //centering the text for the progress information
  String text = (String)moved + char(223) + " " + percent + char(37);
  int textLength = text.length();
  int start = (LCD_Width - textLength) / 2;
  //writing centered text
  lcd.setCursor(start,2);
  lcd.print(text);
  }

//function that converts milliseconds into seconds and converts into a string
//with the word sec after it to be displayed on the Delay line of LCD
String DelayString(){
  float delayed = (float)DelayTime / 1000;
  String timeString = (String)delayed + " sec";
  return timeString;
  }

//function to convert the degrees entered by the user to steps
void Steps(){
  TotalSteps = Degrees * StepsPerDegree;
  }

//function to convert steps to degrees
int StepsToDegrees(){
  int degree = Counter / StepsPerDegree;
  return degree;
  }

//function to calculate the percent complete
float PercentComplete(){
  float percent = ((float)Counter / TotalSteps) * 100;
  return percent;
  }

//setup is for things that you want to run only once
void setup() {
  //setting stepper motor movement speed
  stepper1.setSpeed(4000);

  //initializing lcd, saying welcome, and populationg screen
  lcd.init();
  lcd.setBacklight(HIGH);
  lcd.setCursor(7,1);
  lcd.print("Welcome");
  lcd.setCursor(7,2);
  lcd.print((String)"Ver 2.1");
  delay(2500);
  PopulateScreen();
  
  //setting buttons as inputs
  pinMode(ButtonOne, INPUT);
  pinMode(ButtonTwo, INPUT);
  pinMode(ButtonThree, INPUT);
  pinMode(ButtonFour, INPUT);
  pinMode(ButtonFive, INPUT);
  pinMode(ButtonSix, INPUT);
  pinMode(ButtonSeven, INPUT);
  
}

//loop is the main method that runs in a loop
void loop() {
  //since the arduino just runs the code in a continuous loop,
  //we have to check the state of each button to see if they have been pressed

  //ButtonOne increases the delay time by 250 milliseconds
  ButtonState1 = digitalRead(ButtonOne);
  if(ButtonState1 == HIGH) {
    DelayTime += 250;
    delay(200); //delay so button does not continually press too fast
    lcd.setCursor(0,0);
    lcd.print("Delay = ");
    //clearing line so numbers display correctly when changing number of digits
    lcd.setCursor(8,0);
    lcd.print("            ");
    String timeString = DelayString();
    lcd.setCursor(8,0);
    lcd.print(timeString);
    }

  //ButtonTwo decreases the delay time by 250 milliseconds
  ButtonState2 = digitalRead(ButtonTwo);
  if(ButtonState2 == HIGH) {
    DelayTime -= 250;
    delay(200);
    //preventing DelayTime from having negative values
    if(DelayTime <= 0){
      DelayTime = 250;}
    lcd.setCursor(0,0);
    lcd.print("Delay = ");
    lcd.setCursor(8,0);
    lcd.print("            ");
    String timeString = DelayString();
    lcd.setCursor(8,0);
    lcd.print(timeString);
    }

  //ButtonThree increases the steps per movement by 1 step
  ButtonState3 = digitalRead(ButtonThree);
  if(ButtonState3 == HIGH) {
    StepUp += 1;
    delay(200);
    //preventing 0 steps
    if(StepUp == 0){
      StepUp += 1;
      }
    lcd.setCursor(8,1);
    lcd.print("            ");
    if(StepUp > 0){
      lcd.setCursor(8,1);
      lcd.print((String)StepUp + " CW");
      }
    if(StepUp < 0){
      lcd.setCursor(8,1);
      lcd.print((String)StepUp + " CCW");
      }
    }

  //ButtonFour decreases the steps per movement by 1 step
  ButtonState4 = digitalRead(ButtonFour);
  if(ButtonState4 == HIGH) {
    StepUp -= 1;
    delay(200);
    //preventing 0 steps
    if(StepUp == 0){
      StepUp -= 1;
      }
    lcd.setCursor(8,1);
    lcd.print("            ");
    if(StepUp > 0){
      lcd.setCursor(8,1);
      lcd.print((String)StepUp + " CW");
      }
    if(StepUp < 0){
      lcd.setCursor(8,1);
      lcd.print((String)StepUp + " CCW");
      };
    }

  //ButtonFive increases the total degree travel by 5 degrees
  ButtonState5 = digitalRead(ButtonFive);
  if(ButtonState5 == HIGH){
    Degrees += 5;
    delay(200);
    //preventing 0 degrees
    if(Degrees == 0){
      Degrees = 5;
      }
    lcd.setCursor(10,2);
    lcd.print("           ");
    lcd.setCursor(10,2);
    lcd.print((String)Degrees + char(223));
    }
  
  //ButtonSix decreases the total degree travel by 5 degrees
  ButtonState6 = digitalRead(ButtonSix);
  if(ButtonState6 == HIGH){
    Degrees -= 5;
    delay(200);
    //preventing 0 degrees
    if(Degrees == 0){
      Degrees = 5;
      }
    lcd.setCursor(10,2);
    lcd.print("           ");
    lcd.setCursor(10,2);
    lcd.print((String)Degrees + char(223));
    }

  //ButtonSeven starts the rotation of the gimbal
  ButtonState7 = digitalRead(ButtonSeven);
  if(ButtonState7 == HIGH){
    //get the steps from the degrees entered and save it to TotalSteps
    Steps();
    //controlling when to stop rotating depending on if it's going CW or CCW
    if(StepUp > 0){
      while(Counter <= TotalSteps){
        //moving motor by user entered steps
        stepper1.step(StepUp);
        delay(DelayTime);
        //getting the degrees moved
        int moved = StepsToDegrees();
        //getting the percent complete
        float percent = PercentComplete();
        //displaying progress on the lcd screen
        ProgressScreen(moved, percent);
        Counter += StepUp;
        }
      }
    else if(StepUp < 0 ){
      //make sure TotalSteps contains a negative value;
      TotalSteps -= (2 * TotalSteps);
      while(Counter >= TotalSteps){
        //moving motor by user entered steps
        stepper1.step(StepUp);
        delay(DelayTime);
        //getting degrees moved
        int moved = StepsToDegrees();
        //getting perecentage complete
        float percent = PercentComplete();
        //displaying progress on the lcd screen
        ProgressScreen(moved, percent);
        Counter += StepUp;
        }
      }

      //setting Counter back to 0
      Counter = 0;
      //restoring setup screen
      PopulateScreen();
    }
    
}
