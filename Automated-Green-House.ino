

#include <OneWire.h>
#include <DallasTemperature.h>
#include <Servo.h>

//screen variables 
#define ONE_WIRE_BUS 2
#define TempIn 8
#define limitswitchbutton 6
#define MaxTemp 26
#define MinTemp 21

OneWire oneWire(ONE_WIRE_BUS);  
DallasTemperature sensors(&oneWire);
Servo servo1;

#define BACK 7 //black
#define ENTER 12 //green
#define INCREASE 11 //blue
#define DECREASE 9 //yellow
#define CYCLE 10 //red
//water
#define PUMP_PIN 7 
#define WATER_SENSOR_PIN 0

//light sensor 
#define LIGHT_OUT 2
#define WINDOW_OUTPUT 3

#define MAINMENU 4
#define TEMPMENU 0
#define LIGHTMENU 1
#define WATERMENU 2
#define PRESETMENU 3
#define HOVERTEMP 0
#define HOVERLIGHT 1
#define HOVERWATER 2
#define HOVERPRESET 3
//water sensor



//temp sensor vals

boolean isDoorOpen;

//light sensor variables 
unsigned int digitalValue = 0;
int LightOut = 2; //pin out # 

int WindowOutput = 3;
unsigned long dayStart = 0;
bool WindowClose = false;
int sunlightCounter = 0;
int dayLength = 1000;

//internal values to keep track of what is being displayed 
 boolean showMenuDisplay;
 boolean showHoverDisplay;
 int  backVal;
 int  enterVal;
 int  increaseVal;
 int  decreaseVal;
 int  cycleVal;
 
 int currentMenu; //the menu to be displayed on the screen
 int hover; //which element is highlighted

 //thresholds set by user 
 int waterLevel;
 int lightLevel;
 int tempMaxLevel;
 int tempMinLevel;

 //water sensor values
  float user_moisture_percent = 80;
  float AirValue = 550;
  float WaterValue = 330;

 
void setup() {
  // put your setup code here, to run once:
//  uiSetup();
  Serial.begin(9600);
  //light sensor set up
  pinMode(WindowOutput, OUTPUT); //LED circuit output
  digitalWrite(WindowOutput, LOW);
  dayStart = millis(); //starts the timer when the program starts running
  pinMode(LightOut, INPUT); // Digital Input from light sensor

  //water sensor setup
//  delay(2000);
   pinMode(PUMP_PIN, OUTPUT); //pump pin is output
 
  Serial.println("Hello! welcome to our automated greenhouse!");
 // delay(2000);


}
void uiSetup(){
    pinMode(BACK, INPUT);
  pinMode(ENTER, INPUT);
  pinMode(INCREASE, INPUT);
  pinMode(DECREASE, INPUT);
  pinMode(CYCLE, INPUT);
  
  hover = HOVERTEMP;
  currentMenu = MAINMENU;
  showMenuDisplay = true;
  showHoverDisplay = true;
  waterLevel=50;
  lightLevel=8;
  tempMaxLevel=30;
  tempMinLevel=25;
}

void tempSetup(){
   sensors.begin();
  pinMode(TempIn,INPUT);
  pinMode(limitswitchbutton,INPUT);
  servo1.attach(10);
  servo1.write(90);
  delay(1000);
  isDoorOpen = false;
}
 
void loop() {
  lightSensorControl();
  waterSensorLoop();
 // temperatureControlLoop();
  //checkScreenButtons();

}

void waterSensorLoop(){
    int waterSensorVal = analogRead(WATER_SENSOR_PIN);
  int moisture_threshold = (int)(AirValue - (user_moisture_percent/((float)100))*(AirValue - WaterValue));
  //Serial.println(waterSensorVal);
  //value for sensor in air  
  if(waterSensorVal>moisture_threshold){
    digitalWrite(PUMP_PIN, LOW); //turn pump on
    delay(300); // turn on for a hot sec
  }
  digitalWrite(PUMP_PIN, HIGH); //turn pump off
  delay(1000);
  Serial.print(waterSensorVal);
  Serial.print(" ");
  Serial.print(moisture_threshold);
  Serial.println();
}

void checkScreenButtons(){
   backVal = digitalRead(BACK);
  enterVal = digitalRead(ENTER);
  increaseVal = digitalRead(INCREASE);
  decreaseVal = digitalRead(DECREASE);
  cycleVal = digitalRead(CYCLE);

  displayMenu();
  displayHover();
  if(backVal == HIGH){
     //Serial.println(" GO BACK");
     backFun();
     delay(500);
  }
    if(enterVal == HIGH){
     enterFun();
     delay(500);
  }
    if(increaseVal == HIGH){
     //Serial.println("INCREASE");
     increaseFun();
     delay(500);
  }
    if(decreaseVal == HIGH){
   //  Serial.println("DECREASE");
     decreaseFun();
     delay(500);
    }
    if(cycleVal == HIGH){
     //Serial.println("cycle");
     cycleFun();
     delay(500);
  }
}
void backFun(){
   showMenuDisplay = true;
    if(currentMenu!= MAINMENU){
      currentMenu = MAINMENU;
    }
  displayMenu();
}
void enterFun(){
    showMenuDisplay = true;
    if(currentMenu== MAINMENU){
      currentMenu = hover;
    }
  displayMenu();
  
}
void increaseFun(){
  showMenuDisplay=true;
  if(currentMenu == TEMPMENU){
    
  }else if(currentMenu == WATERMENU && waterLevel<100){
    waterLevel++;
  }else if(currentMenu == PRESETMENU){
    
  }else if (currentMenu == LIGHTMENU && lightLevel<16){
    lightLevel++;
  }
  displayMenu();
}
void decreaseFun(){
  showMenuDisplay=true;
   if(currentMenu == TEMPMENU){
        
  }else if(currentMenu == WATERMENU && waterLevel>0){
    waterLevel--;
  }else if(currentMenu == PRESETMENU){
    
  }else if (currentMenu == LIGHTMENU && lightLevel>0){
    lightLevel--;
  }
  displayMenu();
}

void cycleFun(){
  showHoverDisplay = true;
  if(currentMenu == MAINMENU){
    if(hover== HOVERPRESET){
      hover = HOVERTEMP;
    }
    else{
      hover++;
    }
  }
  displayHover();
}

void displayMenu(){
  if(showMenuDisplay){
    Serial.print("the current menu is: ");
    if(currentMenu ==TEMPMENU){
      //Serial.println("temperature submenu ");
        displayTempMenu();
      }else if(currentMenu ==LIGHTMENU){
        //Serial.println("light submenu");
        displayLightMenu();
      }else if(currentMenu ==WATERMENU){
        //Serial.println("water submenu");
        displayWaterMenu();
      }else if(currentMenu ==PRESETMENU){
       // Serial.println("preset submenu");
       displayPresetMenu();
      }else if(currentMenu ==MAINMENU){
        Serial.println("main submenu");
      } 
  } 
  showMenuDisplay=false;
}
void displayHover(){
  if(showHoverDisplay){
      Serial.print("the current selection is: ");
  if(hover ==HOVERTEMP){
    Serial.println("temperature submenu highlighted");
    }else if(hover ==HOVERLIGHT){
      Serial.println("light submenu highlighted");
    }else if(hover ==HOVERWATER){
      Serial.println("water submenu highlighted");
    }else if(hover ==HOVERPRESET){
      Serial.println("preset submenu highlighted");
    }  
  }
    showHoverDisplay = false;
}

void displayWaterMenu(){
  Serial.println("You have selected the water submenu, below is the current value");
  Serial.println("*************");
  Serial.println(waterLevel);
  Serial.println("%");
  Serial.println("*************");  
}

void displayLightMenu(){
  Serial.println("You have selected the light submenu, below is the current value");
  Serial.println("*************");
  Serial.println(lightLevel);
  Serial.println("h");
  Serial.println("*************");
}

void displayTempMenu(){
  Serial.println("You have selected the temp submenu, below is the current value");
  Serial.println("*************");
  Serial.println("10%");
  Serial.println("*************");
}

void displayPresetMenu(){
  Serial.println("You have selected the preset submenu, below is the current value");
  Serial.println("*************");
  Serial.println("10%");
  Serial.println("*************");
}

void lightSensorControl(){
     int dayCurrent = millis() - dayStart; //tracks how much time is left in the day
  //Serial.println(dayCurrent);
  
  if(dayCurrent > 10000){
    dayStart = millis(); //resets the start of the day
    sunlightCounter = 0;
    Serial.println("New Day");
  }
  
  //closes the window if the plant has recieved 5 hours of sunlight
  else if(sunlightCounter > 4){ //checks if the counter reaches 10 within 10s
    digitalWrite(WindowOutput, HIGH);
    WindowClose = true;

   // digitalValue = 1;
    Serial.println("Window Closed");
    //delay(10000 - dayCurrent);
    //dayStart = millis(); //resets the start of the timer to the the time after the delay
  //  sunlightCounter = 0;
  }
  else{
    digitalValue = digitalRead(LightOut);
    
    if(digitalValue == 0){
      sunlightCounter = sunlightCounter + 1;
    }
    
    Serial.println(digitalValue);
    
    digitalWrite(WindowOutput, LOW);
  //  WindowClose = false;
    
    delay(500); // reads a value from the light sensor every .5s
  } 
}

void temperatureControlLoop(){
    // put your main code here, to run repeatedly:
  sensors.requestTemperatures();            // Input from temperature sensor. Not currently used in code. 
  int limitswitch = digitalRead(limitswitchbutton);
  int TempInput = digitalRead(TempIn);
  Serial.print("Temperature: "); 
  Serial.print(sensors.getTempCByIndex(0)); //Value
  Serial.print((char)127); //Space character  
  Serial.println("C  ");
    
    if(limitswitch == HIGH && TempInput == LOW){ // Door is Closed, Servo is stationary, Temperature is good.
     // Serial.println("limit switch highm temp input low");
      servo1.write(90); // Servo is not moving
    }
    else if(limitswitch == HIGH && TempInput == HIGH){ //Temperature is high but the door is currently closed.
      dooropen();                                      // Must Open Door.
      isDoorOpen =true;
    // Serial.println("limit switch highm temp input high");
      servo1.write(90); //Servo is not moving
    }
    else if(limitswitch == LOW && TempInput == HIGH){ //Door is open, Temperature is still too high.
      servo1.write(90); //Servo is not moving     
    //Serial.println("limit switch low temp input high");    
    } 
    else if (limitswitch == LOW && TempInput == LOW && isDoorOpen){ // Door is Open, But temperature is no longer too high.
      servo1.write(180); //Servo closing door
      isDoorOpen=false;
     
    } 
    else{
     // Serial.println("int else");
    }
}

void dooropen() { // Opens door 
    servo1.write(0); //Servo opening door.
    delay(500); // Time spent with servo opening door. 
}
