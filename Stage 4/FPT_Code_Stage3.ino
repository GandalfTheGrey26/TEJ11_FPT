/*
Title: Traffic Light Challenge 2.0 (FPT)
Creator: Glenn M.
Date Of Creation: January 10th, 2022 
Last Update: January 19th, 2022
*/

//Store all the pin numbers under variables:
int red[] = {13, 10, 7};	//red led pins (North, EastWest, South)
int ylw[] = {12, 9, 6};	    //yellow led pins(North, EastWest, South)
int grn[] = {11, 8, 5};	    //green led pins (North, EastWest, South)
int walk[] = {4, 3};        //led pins (Red, Green)
int button = 2;             //the cross walk button pin
int photo = A0;             //photoresistor pin
int buzzer = A1;             //piezo buzzer pin

float delays[][3] = {{5, 1.4, 1}, {5, 1.4, 1}, {3.5, 0, 0}}; 
//delay times [(direction{N and S, EW, Nleft})][(time{green, yellow, red})] 

int multip = 1500;	   //delay multiplier
int blinks = 6;        //amount of blinks for north left turn

int dir = 1;	         //direction of traffic (0 = NS; 1 = EW;)
bool leftTurn = false;   //wether the resistor detects a 'car' in the 'North Left lane'
bool cross = false;      //whether the crosswalk button has been pressed

void setup() {
  for(int i = 0; i < 3; i++){   //loop three times
    pinMode(red[i], OUTPUT);	 //set current red pin to output
    digitalWrite(red[i], HIGH);  //set current red pin to high
    pinMode(ylw[i], OUTPUT);	 //set current yellow pin to output
    digitalWrite(ylw[i], LOW);	 //set current yellow pin to low
    pinMode(grn[i], OUTPUT);	 //set current green pin to output
    digitalWrite(grn[i], LOW);}	 //set current green pin to low
  pinMode(walk[0], OUTPUT);   //set the red cross walk pin to output
  digitalWrite(walk[0], HIGH);//set the red cross walk pin to HIGH
  pinMode(walk[1], OUTPUT);   //set the green cross walk pin to output
  digitalWrite(walk[1], LOW); //set the green cross walk pin to LOW
  pinMode(photo, INPUT);      //set the photoresistor pin to input
  pinMode(button, INPUT);     //set the button pin to input
  pinMode(buzzer, OUTPUT);    //set the buzzer pin to output
  pinMode(buzzer, LOW);       //set the buzzer to low
  Serial.begin(9600);	      //begin the serial monitor
  delay(2500);			      //wait for 2.5 seconds
  readPhoto();
}

void loop(){ 
  updateLEDS();	 //update the traffic lights
}

void updateLEDS(){
  /* 
  Using the current direction of traffic (variable 'dir'), set the 
  traffic lights accordingly (using the functions: northSouth(), 
  eastWest(), crossWalk, and northLeft();). Than switch the traffic 
  direction.
  */
  if(dir == 0){         //traffic direction is East/West...?
   eastWest();             //...update the 'traffic lights' for a East/West direction
   dir = 1;}               //...swap traffic direction to North/South
  else if(dir == 1){    //traffic direction is North/South...?
   if(leftTurn == true){   //...'car' in North Left turn 'lane'...?
     northLeft();}            //...update the 'traffic lights' for a North Left turn direction
   if(cross == true){      //...the cross walk button has been pressed...?
     cross = false;           //...set the cross walk to false
     crossWalk();}            //...update the 'traffic lights' for a North/South direction, with cross walk
   else{
     northSouth();}			  //...update the 'traffic lights' fot a North/South direction
   dir = 0;}               //...swap the traffic direction to East/West
}

void northSouth(){
  /*
  For both the North and South lights, go through:
  green, delay, yellow, delay, red, delay;
  while also checking the cross walk button during the delays
  */
 digitalWrite(grn[0], HIGH);	 //send a green light to the North direction of traffic
 digitalWrite(grn[2], HIGH);     //send a green light to the South direction of traffic
 digitalWrite(red[0], LOW);	     //turn off red light for the North direction of traffic
 digitalWrite(red[2], LOW);	     //turn off red light for the South direction of traffic
 readAndDelay(delays[0][0] * multip); //green delay for the NS direction of traffic, while checking button
 digitalWrite(grn[0], LOW);	     //turn off green light for the North direction of traffic
 digitalWrite(grn[2], LOW);	     //turn off green light for the South direction of traffic
 digitalWrite(ylw[0], HIGH);	 //send a yellow light to the North direction of traffic
 digitalWrite(ylw[2], HIGH);	 //send a yellow light to the South direction of traffic
 readAndDelay(delays[0][1] * multip);  //yellow delay for the NS direction of traffic, while checking button
 digitalWrite(ylw[0], LOW);	     //turn off yellow light for the North direction of traffic
 digitalWrite(ylw[2], LOW);	     //turn off yellow light for the South direction of traffic
 digitalWrite(red[0], HIGH);	 //send a red light to the North direction of traffic
 digitalWrite(red[2], HIGH);	 //send a red light to the North direction of traffic
 readAndDelay(delays[0][2] * multip);  //red delay for the NS direction of traffic, while checking button
}

void crossWalk(){
  /*
  For both the North and South lights, and the cross walk, go through:
  [green(NS) & green(CW)], [3/4 delay], [red(CW)], [1/4 delay], 
  [yellow(NS)], [delay], [red(NS)], [delay]
  */
 digitalWrite(grn[0], HIGH);	 //send a green light to the North direction of traffic
 digitalWrite(grn[2], HIGH);     //send a green light to the South direction of traffic
 digitalWrite(walk[1], HIGH);    //send a green light to the cross walk
 digitalWrite(walk[0], LOW);     //turn off red light for the cross walk
 digitalWrite(red[0], LOW);	     //turn off red light for the North direction of traffic
 digitalWrite(red[2], LOW);      //turn off red light for the South direction of traffic
 RDT((delays[0][0] * multip) * 0.75);     //(3/4) green delay for the NS direction of traffic (with buzzer)
 digitalWrite(walk[0], HIGH);    //send a red light to the cross walk
 digitalWrite(walk[1], LOW);     //turn off green light for the cross walk
 delay((delays[0][0] * multip) * 0.25);   //(1/4) green delay for the NS direction of traffic 
 digitalWrite(grn[0], LOW);	     //turn off green light for the North direction of traffic
 digitalWrite(grn[2], LOW);	     //turn off green light for the South direction of traffic
 digitalWrite(ylw[0], HIGH);	 //send a yellow light to the North direction of traffic
 digitalWrite(ylw[2], HIGH);	 //send a yellow light to the South direction of traffic
 delay(delays[0][1] * multip);   //yellow delay for the NS direction of traffic
 digitalWrite(ylw[0], LOW);	     //turn off yellow light for the North direction of traffic
 digitalWrite(ylw[2], LOW);	     //turn off yellow light for the South direction of traffic
 digitalWrite(red[0], HIGH);	 //send a red light to the North direction of traffic
 digitalWrite(red[2], HIGH);	 //send a red light to the North direction of traffic
 delay(delays[0][2] * multip);   //red delay for the NS direction of traffic
}

void northLeft(){
  /*
  For the North light, repeat for # of blinks (variable):
  green, delay, nothing, delay
  */
  float blinkDelay = ((delays[2][0] * multip) / blinks); //calculate the length of a FULL blink
  digitalWrite(red[0], LOW);          //turn off red light for the North direction of traffic
  for(int i = 0; i < blinks; i++){    //loop through the number of full blinks
     digitalWrite(grn[0], HIGH);         //send a green light to the North lights
     readAndDelay(blinkDelay/2);         //delay for half a blink length, while checking button
     digitalWrite(grn[0], LOW);          //turn off green light for the North lights
     readAndDelay(blinkDelay/2);}        //delay for half a blink length, while checking button
}

void eastWest(){
 /*
 Go through the process of: green light, delay, yellow light, delay,
 red light, delay; for the North and South 'traffic lights'.
 */
 digitalWrite(red[1], LOW);      //turn off red light for the EW direction of traffic
 digitalWrite(grn[1], HIGH);     //send a green light to the EW direction of traffic
 readAndDelay(delays[1][0] * multip); //green delay, while reading button
 digitalWrite(grn[1], LOW);      //turn off green light for the EW lights    
 digitalWrite(ylw[1], HIGH);     //send a yellow light to the EW lights
 readAndDelay(delays[1][1] * multip); //yellow delay while reading button
 digitalWrite(ylw[1], LOW);      //turn off yellow light for EW lights
 digitalWrite(red[1], HIGH);     //send a red light to the EW lights
 readPhoto();                    //check for 'cars' in the North left turn 'lane'
 readAndDelay(delays[1][2] * multip); //red delay while reading button
}

void readPhoto(){
  /*
  Check if any 'cars' are registered in the
  North left turn 'lane'
  */
  int photoVal = analogRead(photo);   //get the photoresistor value
  if(photoVal <= 450){                //value is less than/equal to 450 (car IS in lane)...?
    leftTurn = true;}                    //...set left turn to true
  else{                               //value is less than 450...?
    leftTurn = false;}                   //...set left turn to false
}

void readAndDelay(float wholeDelay){
  /*
  Take the given delay time and break it into 
  100 segments. In a for loop of 100 iterations
  check the button, than delay for the length of
  1 delay segment.
  */
  float partDelay = wholeDelay/100;   //calculate the delay segment
  for(int i = 0; i < 100; i++){       //loop 100 times
   if(cross == false){                  //cross is false...?
     int state = digitalRead(button);     //...read the button
     if(state == HIGH){                   //...button state is high...?
      cross = true;}                        //...set cross to true
   }
   delay(partDelay);                    //dealy for a segment
  }
}

void RDT(float wholeDelay){
  /*
  Read the button, delay, and play tone
  */
  float partDelay = wholeDelay/100;    //break the delay time into 100 segments
  
  /*for the first 70% of the delay, play tone with 
  medium delay in between*/
  for(int i = 0; i < 7; i++){
    tone(buzzer, 440);
    delay(partDelay*5);
    noTone(buzzer);
    delay(partDelay*5);}
  
  /*for the other 30% of the delay, play slighlty higher
  tone with short delay in between*/
  for(int i = 0; i < 6; i++){
    tone(buzzer, 466);
    digitalWrite(walk[1], HIGH);
    delay(partDelay*2.5);
    noTone(buzzer);
    digitalWrite(walk[1], LOW);
    delay(partDelay*2.5);
  }
}
