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

 boolean showMenuDisplay;
 boolean showHoverDisplay;
 int  backVal;
 int  enterVal;
 int  increaseVal;
 int  decreaseVal;
 int  cycleVal;
 int currentMenu; //the menu to be displayed on the screen
 int hover; //which element is highlighted
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
  if(backVal == HIGH){
     Serial.println(" GO BACK");
  }
    if(enterVal == HIGH){
     enterFun();
     delay(500);
  }
    if(increaseVal == HIGH){
     Serial.println("INCREASE");
  }
    if(decreaseVal == HIGH){
     Serial.println("DECREASE");
    }
    if(cycleVal == HIGH){
     //Serial.println("cycle");
     cycleFun();
     delay(500);
  }
}

void backFun(){
  
}
void enterFun(){
    showMenuDisplay = true;
    if(currentMenu== MAINMENU){
      currentMenu = hover;
    }
  displayMenu();
  
}
void increaseFun(){
  
}
void decreaseFun(){
  
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
      Serial.println("temperature submenu ");
      }else if(currentMenu ==LIGHTMENU){
        Serial.println("light submenu");
      }else if(currentMenu ==WATERMENU){
        Serial.println("water submenu");
      }else if(currentMenu ==PRESETMENU){
        Serial.println("preset submenu");
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
  Serial.print("10%");
  Serial.println("*************");
  
}
void displayLightMenu(){
  Serial.println("You have selected the water submenu, below is the current value");
  Serial.println("*************");
  Serial.print("10%");
  Serial.println("*************");
}
void displayTempMenu(){
  Serial.println("You have selected the water submenu, below is the current value");
  Serial.println("*************");
  Serial.print("10%");
  Serial.println("*************");
}
void displayPresetMenu(){
  Serial.println("You have selected the water submenu, below is the current value");
  Serial.println("*************");
  Serial.print("10%");
  Serial.println("*************");
}
