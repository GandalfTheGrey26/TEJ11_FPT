/*
Title: Traffic Light Challenge 2.0 (FPT)
Creator: Glenn M.
Date Of Creation: January 10th, 2022 
Last Update: January 11th, 2022
*/

int red[] = {13, 10};	//red led pins (NorthSouth, EastWest)
int ylw[] = {12, 9};	//yellow led pins(NorthSouth, EastWest)
int grn[] = {11, 8};	//green led pins (NorthSouth, EastWest)

float delays[][3] = {{5, 1.4, 1}, {5, 1.4, 1}}; // delay times [(direction{NS, EW})][(time{green, yellow, red})]
int dir = 0;	 //direction of traffic (0 = NS; 1 = EW;)
int opp = 1;	 //opposite dir of traffic (0 = NS; 1 = EW;)

int multip = 1000;	//delay multiplier

void setup()
{
  for(int i = 0; i < 2; i++){ //loop twice
   pinMode(red[i], OUTPUT);		//set current red pin to output
   digitalWrite(red[i], HIGH);  //set current red pin to high
   pinMode(ylw[i], OUTPUT);		//set current yellow pin to output
   digitalWrite(ylw[i], LOW);	//set current yellow pin to low
   pinMode(grn[i], OUTPUT);		//set current green pin to output
   digitalWrite(grn[i], LOW);	//set current green pin to low
  }
  Serial.begin(9600);	//begin the serial monitor
  delay(2500);			//wait for 2.5 seconds
}

void loop(){
  updateDir();		//update the direction of traffic
  updateLED();		//update the traffic lights (LEDs)
}

void updateDir(){
  /* 
  Using the current direction of traffic 
  (variable 'dir'), find the opposite 
  direction of traffic and set the 
  variable 'opp' to it.
  */
  if(dir==0){
   opp = 1;}
  else if(dir == 1){
   opp = 0;}
}

void updateLED(){
 digitalWrite(grn[dir], HIGH);	//send a green light to the direction of traffic
 digitalWrite(red[dir], LOW);	//turn off red light for the direction of traffic
 digitalWrite(red[opp], HIGH);	//send a red light to the opposite direction of traffic
 delay(delays[dir][0] * multip); //green delay for the direction of traffic
 digitalWrite(grn[dir], LOW);	//turn off green light for the direction of traffic
 digitalWrite(ylw[dir], HIGH);	//send a yellow light to the direction of traffic
 delay(delays[dir][1] * multip); //yellow delay for the direction of traffic
 digitalWrite(ylw[dir], LOW);	//turn off yellow light for the direction of traffic
 digitalWrite(red[dir], HIGH);	//send a red light to the direction of traffic
 delay(delays[dir][2] * multip); //red delay for the direction of traffic
 dir = opp;
}

  
  
  