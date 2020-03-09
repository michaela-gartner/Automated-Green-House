//screen variables 
#define BACK 7 //black
#define ENTER 12 //green
#define INCREASE 11 //blue
#define DECREASE 9 //yellow
#define CYCLE 10 //red
#define MAINMENU 4
#define TEMPMENU 0
#define LIGHTMENU 1
#define WATERMENU 2
#define PRESETMENU 3
#define HOVERTEMP 0
#define HOVERLIGHT 1
#define HOVERWATER 2
#define HOVERPRESET 3

//light sensor variables 
unsigned int digitalValue = 0;
int LightOut = 2; //pin out # 

int WindowOutput = 3;
unsigned long dayStart = 0;
bool WindowClose = false;
int sunlightCounter = 0;


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
 
 
void setup() {
  // put your setup code here, to run once:
  pinMode(BACK, INPUT);
  pinMode(ENTER, INPUT);
  pinMode(INCREASE, INPUT);
  pinMode(DECREASE, INPUT);
  pinMode(CYCLE, INPUT);
  Serial.begin(9600);
  hover = HOVERTEMP;
  currentMenu = MAINMENU;
  showMenuDisplay = true;
  showHoverDisplay = true;
  waterLevel=50;
  lightLevel=8;
  tempMaxLevel=30;
  tempMinLevel=25;

  //light sensor set up
  pinMode(WindowOutput, OUTPUT); //LED circuit output
  digitalWrite(WindowOutput, LOW);
  dayStart = millis(); //starts the timer when the program starts running
  pinMode(LightOut, INPUT); // Digital Input from light sensor
 
  Serial.println("Hello! welcome to our automated greenhouse!");
  delay(2000);


}

 
void loop() {
  
  //Serial.println("hello world");
  backVal = digitalRead(BACK);
  enterVal = digitalRead(ENTER);
  increaseVal = digitalRead(INCREASE);
  decreaseVal = digitalRead(DECREASE);
  cycleVal = digitalRead(CYCLE);

  displayMenu();
  displayHover();
  lightSensorControl();
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
  if(sunlightCounter == 10 && dayCurrent < 10000){ //checks if the counter reaches 10 within 10s
    digitalWrite(WindowOutput, HIGH);
    WindowClose = true;

    digitalValue = 1;
   // Serial.println("Window Closed");
    delay(10000 - dayCurrent);
    dayStart = millis(); //resets the start of the timer to the the time after the delay
    sunlightCounter = 0;
  }
  else{
    digitalValue = digitalRead(LightOut);
    
    if(digitalValue == 0){
      sunlightCounter = sunlightCounter + 1;
    }
    
    //Serial.println(digitalValue);
    
    digitalWrite(WindowOutput, LOW);
    WindowClose = false;
    
    delay(500); // reads a value from the light sensor every .5s
  } 
}
