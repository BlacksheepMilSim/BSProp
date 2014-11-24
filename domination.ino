void domination(){

  //SETUP INITIAL TIME 
  int minutos = GAMEMINUTES-1;
  boolean showGameTime=true;
  unsigned long a;
  unsigned long iTime=millis(); //  initialTime in millisec 
  unsigned long aTime;
 
  team=0;
  iZoneTime=0;
  aTime=0;
  alphaTime=0;
  bravoTime=0;
  

  int largoTono = 50;
  // 0 = neutral, 1 = alpha team, 2 = bravo team
  a=millis();
  //Starting Game Code
  while(1)  // this is the important code, is a little messy but works good.
  {
	static boolean enableNeo =false;
	static long timerNeo;
	static uint32_t colorNeo;
  	static uint32_t waitNeo;
  
  if(enableNeo){		//is LED enabled
	theaterChase(colorNeo, waitNeo);	//call function
	if(millis()>timerNeo){		//if we have flashed for the desired time
		enableNeo=false;		//disabled LED
	}
  }
  
    if(endGame){
      gameOver();
    }
    
    
    if(team == 0){
      theaterChase(strip.Color(127, 127, 127), 0);
    }
    
    if(team == 1){
      theaterChase(strip.Color(0, 127, 0), 0);
    }
    
    if(team == 2){
      theaterChase(strip.Color(127, 127, 0), 0);
    }
    
    keypad.getKey();
    aTime=millis()- iTime;
    //Code for led blinking
    timeCalcVar=(millis()- iTime)%1000;
    if(timeCalcVar >= 0 && timeCalcVar <= 40)
    {
      if(team==1)digitalWrite(GREENLED, HIGH);  
      if(team==2)digitalWrite(YELLOWLED, HIGH);  
    }
    if(timeCalcVar >= 50 && timeCalcVar <= 100)
    {    
      if(team==1)digitalWrite(GREENLED, LOW);  
      if(team==2)digitalWrite(YELLOWLED, LOW);
    }
    // Sound!!! same as Destroy 
    if(timeCalcVar >= 0 && timeCalcVar <= 40 && soundEnable)tone(tonepin,tonoActivada,largoTono);

    if(timeCalcVar >= 245 && timeCalcVar <= 255 && minutos-aTime/60000<2 && soundEnable)tone(tonepin,tonoActivada,largoTono);
    if(timeCalcVar >= 495 && timeCalcVar <= 510 && minutos-aTime/60000<4 && soundEnable)tone(tonepin,tonoActivada,largoTono);
    if(timeCalcVar >= 745 && timeCalcVar <= 760 && minutos-aTime/60000<2 && soundEnable)tone(tonepin,tonoActivada,largoTono);



    //Help to count 3 secs
//    if(a+2000<millis()){
//      a=millis();   
//      showGameTime=!showGameTime;
//      cls();
//    }
    //THE NEXT TO METHODS SHOW "GAME TIME" AND "CONTROLED ZONE TIME" IT SHOWS 2 AND 2 SEC EACH

//    if(showGameTime){ //THE SECOND IS /2
      lcd.setCursor(3,0);
      lcd.print("GAME TIME");
      lcd.setCursor(3,1);
      printTime(minutos, aTime);
//    }
//    else if (!showGameTime){

      lcd.setCursor(4,2);
      if(team == 0){
        lcd.print("NEUTRAL ZONE");
        lcd.setCursor(0,3);
        printTimeDom(timea, false);
        lcd.setCursor(10,3);
        printTimeDom(timeb, false);
//        theaterChase(strip.Color(127, 127, 127), 50);
//        strip.show();
                enableNeo=true;		//turn on LED
		timerNeo=millis()+5000;	//when they should stop flashing. 0.5 seconds from now.
		colorNeo=strip.Color(127, 127, 127);		//the color
        	waitNeo=0;		//the wait time
      }
      
      lcd.setCursor(2,2);
      if(team == 1){
        lcd.print(" Alpha Conatrol");
        lcd.setCursor(0,3);
        timea = alphaTime + millis() - iZoneTime,false;
        printTimeDom(timea, false);
        lcd.setCursor(10,3);
        printTimeDom(timeb, false);
//        theaterChase(strip.Color( 0, 127, 0), 50);
//        strip.show();
                enableNeo=true;		//turn on LED
		timerNeo=millis()+5000;	//when they should stop flashing. 0.5 seconds from now.
		colorNeo=strip.Color(0, 127, 0);		//the color
        	waitNeo=0;		//the wait time
      }
      lcd.setCursor(2,2);
      if(team == 2){
        lcd.print(" Bravo Control");
        lcd.setCursor(0,3);
        printTimeDom(timea, false);
        lcd.setCursor(10,3);
        timeb = bravoTime + millis() - iZoneTime,false;
        printTimeDom(timeb, false);
//        theaterChase(strip.Color( 127, 127, 0), 50);
//        strip.show();
                enableNeo=true;		//turn on LED
		timerNeo=millis()+5000;	//when they should stop flashing. 0.5 seconds from now.
		colorNeo=strip.Color(127, 127, 0);		//the color
        	waitNeo=0;		//the wait time
      
      }   
   //   if(team>0){
   //     lcd.setCursor(10,3);
   //     printTimeDom(millis()-iZoneTime,false);
   //   }
//    }

    //###########################CHECKINGS##################

    //Check If Game End
    if(minutos-aTime/60000==0 && 59-((aTime/1000)%60)==0)
    {
      gameOver();
    }
    
    //Check If IS neutral
    while((defuseando || cancelando) && team > 0)
    {
      cls();
      lcd.setCursor(4,0);
      if(team>0)lcd.print("NEUTRALIZING");
      unsigned int percent=0;
      unsigned long xTime=millis(); //start disabling time
      while(defuseando || cancelando)
      {
        //check if game time runs out during the disabling
        aTime= millis()- iTime;
        if((minutos-aTime/60000==0 && 59-((aTime/1000)%60)==0) || minutos-aTime/60000>4000000000){ 
          endGame = true;
        }
        
        keypad.getKey();
        timeCalcVar = (millis()- xTime)%1000;

        if( timeCalcVar >= 0 && timeCalcVar <= 20)
        {
          if(soundEnable)tone(tonepin,tonoAlarma1,200);
        }
        if(timeCalcVar >= 480 && timeCalcVar <= 500)
        {
          if(soundEnable)tone(tonepin,tonoAlarma2,200);
          digitalWrite(YELLOWLED, LOW);
        }

        unsigned long seconds= millis() - xTime;
        percent = (seconds)/(ACTIVATESECONDS*10);
        drawPorcent(percent);

        if(percent >= 100)
        {
          delay(1000);

          if(team==1){ 
            alphaTime+= timea;
            iZoneTime=0; 

          }
          if(team==2){ 
            bravoTime+= timeb;
            iZoneTime=0; 
          }
          team=0;
          break;
        }
      }
      cls();
    }

    //Capturing alpha

    while(defuseando && team == 0 )
    {
      enableNeo=false;
      cls();
      lcd.setCursor(0,0);
      if(team==0)lcd.print("BRAVO CAPTURING ZONE");
      unsigned int percent=0;
      unsigned long xTime=millis(); //start disabling time
      while(defuseando)
      {
        keypad.getKey();
        //check if game time runs out during the disabling
        aTime= millis()- iTime;
        if((minutos-aTime/60000==0 && 59-((aTime/1000)%60)==0) || minutos-aTime/60000>4000000000){ 
          endGame = true;
        }
        timeCalcVar = (millis()- xTime)%1000;

        if( timeCalcVar >= 0 && timeCalcVar <= 20)
        {
          digitalWrite(YELLOWLED, HIGH);  
          if(soundEnable)tone(tonepin,tonoAlarma1,200);
        }
        if(timeCalcVar >= 480 && timeCalcVar <= 500)
        {
          if(soundEnable)tone(tonepin,tonoAlarma2,200);
          digitalWrite(YELLOWLED, LOW);
        }

        unsigned long seconds= millis() - xTime;
        percent = (seconds)/(ACTIVATESECONDS*10);
        drawPorcent(percent);

        if(percent >= 100)
        {
          digitalWrite(GREENLED, LOW);
          team=2;
          iZoneTime=millis();
          delay(1000);
          break;
        }
      }
      cls();
      digitalWrite(YELLOWLED, LOW);
    }

    //getting to green zone
    while(cancelando && team == 0 )
    {
      cls();
      lcd.setCursor(0,0);
      if(team==0)lcd.print("ALPHA CAPTURING ZONE");
      unsigned int percent=0;
      unsigned long xTime=millis(); //start disabling time
      while(cancelando)
      {
        keypad.getKey();
        //check if game time runs out during the disabling
        aTime= millis()- iTime;
        if((minutos-aTime/60000==0 && 59-((aTime/1000)%60)==0) || minutos-aTime/60000>4000000000){ 
          endGame = true;
        }
        timeCalcVar = (millis()- xTime)%1000;

        if( timeCalcVar >= 0 && timeCalcVar <= 20)
        {
          digitalWrite(GREENLED, HIGH);  
          if(soundEnable)tone(tonepin,tonoAlarma1,200);
        }
        if(timeCalcVar >= 480 && timeCalcVar <= 500)
        {
          if(soundEnable)tone(tonepin,tonoAlarma2,200);
          digitalWrite(GREENLED, LOW);
        }

        unsigned long seconds= millis() - xTime;
        percent = (seconds)/(ACTIVATESECONDS*10);
        drawPorcent(percent);

        if(percent >= 100)
        {
          digitalWrite(GREENLED, LOW);
          team=1;
          iZoneTime=millis();
          delay(1000);
          break;
        }
      }
      cls();
      digitalWrite(GREENLED, LOW);  
    }
  }
}

void gameOver(){
      
  if(team==1)alphaTime+=millis()-iZoneTime;
  if(team==2)bravoTime+=millis()-iZoneTime;
  digitalWrite(GREENLED, LOW);
  digitalWrite(YELLOWLED, LOW);

 
  while(!defuseando){
    keypad.getKey();
    if(defuseando){
      keypad.getKey();
      break;
    }
    lcd.clear();
    lcd.setCursor(5,0);
    lcd.print("GAME OVER!");

    //check who team win the base
    lcd.setCursor(0,1);
    if(bravoTime>alphaTime){
      //bravo team wins
      lcd.print("BRAVO TEAM DOMINATED");
      digitalWrite(YELLOWLED, HIGH);
    }
    else{
      //alphateam wins 
      lcd.print("ALPHA TEAM DOMINATED");
      digitalWrite(GREENLED, HIGH);
    }
//    delay(3000);
//    keypad.getKey();
//    if(defuseando){
//      keypad.getKey();
//      break;
//    }
//    cls();
    lcd.setCursor(0,2);
    lcd.print("Alpha Time:");
    lcd.setCursor(15,2);
    printTimeDom(alphaTime,false);
//    delay(3000);
//    keypad.getKey();
//    if(defuseando){
      
//      break;
//    }
//    cls();
    lcd.setCursor(0,3);
    lcd.print("Bravo Time:");
    lcd.setCursor(15,3);
    printTimeDom(bravoTime,false);
//    delay(3000);
//    keypad.getKey();
//    if(defuseando){
//      keypad.getKey();
//      break;
//    }

    {

    while(1){
    theaterChase(strip.Color( 65, 0, 0), 10);
    strip.show();    
    var = keypad.getKey();
    if(var == '1' ){
      tone(tonepin,2400,30);
      cls();
      break;
   }
  }
   }
  cls();
  allOff();
  delay(100);
  lcd.print("Play Again?");
  lcd.setCursor(0,1);
  lcd.print("A : Yes B : No");
  while(1)
  {
    var = keypad.waitForKey();
    if(var == 'a' ){
      tone(tonepin,2400,30);
      cls();
      domination();
      break;
    }  
    if(var == 'b' ){
      tone(tonepin,2400,30);
      menuPrincipal();
      break;
    }  
  } 
}
}



