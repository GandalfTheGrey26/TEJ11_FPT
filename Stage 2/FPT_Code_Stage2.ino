/*
Title: Traffic Light Challenge 2.0 (FPT)
Creator: Glenn M.
Date Of Creation: January 10th, 2022 
Last Update: January 13th, 2022
*/

//Store all the pin numbers under variables:
int red[] = {13, 10, 7};	//red led pins (North, EastWest, South)
int ylw[] = {12, 9, 6};	    //yellow led pins(North, EastWest, South)
int grn[] = {11, 8, 5};	    //green led pins (North, EastWest, South)
int photo = A0;             //photoresistor pin

float delays[][3] = {{5, 1.4, 1}, {5, 1.4, 1}, {3.5, 0, 0}}; 
//delay times [(direction{N and S, EW, Nleft})][(time{green, yellow, red})]

int multip = 750;	   //delay multiplier
int blinks = 6;        //amount of blinks for north left turn

int dir = 1;	         //direction of traffic (0 = NS; 1 = EW;)
bool leftTurn = false;   //wether the resistor detects a 'car' in the 'North Left lane'

void setup() {
  for(int i = 0; i < 3; i++){   //loop three times
   pinMode(red[i], OUTPUT);		//set current red pin to output
   digitalWrite(red[i], HIGH);  //set current red pin to high
   pinMode(ylw[i], OUTPUT);		//set current yellow pin to output
   digitalWrite(ylw[i], LOW);	//set current yellow pin to low
   pinMode(grn[i], OUTPUT);		//set current green pin to output
    digitalWrite(grn[i], LOW);}	//set current green pin to low
  pinMode(photo, INPUT);        //set the photoresistor pin to input
  Serial.begin(9600);	//begin the serial monitor
  delay(2500);			//wait for 2.5 seconds
}

void loop(){      
  updateLEDS();	 //update the traffic lights
}

void updateLEDS(){
  /* 
  Using the current direction of traffic (variable 'dir'), set the 
  traffic lights accordingly (using the functions: northSouth(), 
  eastWest() and northLeft();) than switch the traffic direction.
  */
  if(dir == 0){         //traffic direction is East/West...?
   eastWest();             //...update the 'traffic lights' for a East/West direction
   dir = 1;}               //...swap traffic direction to North/South
  else if(dir == 1){    //traffic direction is North/South...?
   if(leftTurn == true){   //...'car' in North Left turn 'lane'...?
     northLeft();}            //...update the 'traffic lights' for a North Left turn direction
   northSouth();           //...update the 'traffic lights' for a North/South direction
   dir = 0;}               //...swap the traffic direction to East/West
}

void northSouth(){
  /*
  Go through the process of: green light, delay, yellow light, delay,
  red light, delay; for the North and South 'traffic lights'.
  */
 digitalWrite(grn[0], HIGH);	 //send a green light to the North direction of traffic
 digitalWrite(grn[2], HIGH);     //send a green light to the South direction of traffic
 digitalWrite(red[0], LOW);	     //turn off red light for the North direction of traffic
 digitalWrite(red[2], LOW);	     //turn off red light for the South direction of traffic
 delay(delays[0][0] * multip);   //green delay for the NS direction of traffic
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
  Blinking green light (left turn) for the
  Northern direction of traffic.
  */
  float blinkDelay = ((delays[2][0] * multip) / blinks); //calculate the length of a FULL blink
  digitalWrite(red[0], LOW);          //turn off red light for the North direction of traffic
  for(int i = 0; i < blinks; i++){    //loop through the number of full blinks
     digitalWrite(grn[0], HIGH);         //send a green light to the North lights
     delay(blinkDelay/2);                //delay for half a blink length
     digitalWrite(grn[0], LOW);          //turn off green light for the North lights
     delay(blinkDelay/2);}               //delay for half a blink length
}

void eastWest(){
 /*
 Go through the process of: green light, delay, yellow light, delay,
 red light, delay; for the North and South 'traffic lights'.
 */
 digitalWrite(red[1], LOW);      //turn off red light for the EW direction of traffic
 digitalWrite(grn[1], HIGH);     //send a green light to the EW direction of traffic
 delay(delays[1][0] * multip);   //green delay for the EW direction of traffic
 digitalWrite(grn[1], LOW);      //turn off green light for the EW lights    
 digitalWrite(ylw[1], HIGH);     //send a yellow light to the EW lights
 delay(delays[1][1] * multip);   //yellow delay for the EW direction
 digitalWrite(ylw[1], LOW);      //turn off yellow light for EW lights
 digitalWrite(red[1], HIGH);     //send a red light to the EW lights
 readPhoto();                    //check for 'cars' in the North left turn 'lane'
 delay(delays[1][2] * multip);   //red delay for the EW lights
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
  
  
  