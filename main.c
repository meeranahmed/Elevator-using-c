#include <c8051f020.h> 

sbit ground = P1^4;
sbit floor1 = P1^3;
sbit floor2 = P1^2;
sbit floor3 = P1^1;
sbit floor4 = P1^0;

sbit ground_up = P0^0;
sbit floor4_down = P0^1;
sbit floor3_up = P0^2;
sbit floor3_down = P0^3;
sbit floor2_up = P0^4;
sbit floor2_down = P0^5;
sbit floor1_up = P0^6;
sbit floor1_down = P0^7;

sbit ir_sensor = P3^7;


sbit led_open = P1^6;
sbit led_closed = P1^5;
sbit motor1 = P3^0;
sbit motor2 = P3^1;
int currentFloor = 0;
unsigned int floors[5] = {0};
unsigned int floors_up[5] = {0};
unsigned int floors_down[5] = {0};

unsigned int lookUpTable[5] = {63,6,91,79,102};
int i = 0;
int state =1;

void delay() {
	int i = 0;
	int j = 0;
	for (i=0; i<500; i++){
		for (j=0; j<100; j++){

		}
	}
}
void doorDelay(){
      int i = 0;
	    int j = 0;
	    for(i=0; i<2000; i++){
		    for(j=0; j<200; j++){

				}
			}
			if(ir_sensor ==1){
			led_open = 1;
			led_closed = 0;
	    doorDelay();
    } 
}

void motorDelay(){
	int i =0;
	int j=0;
	for(i=0; i<1000; i++){
		for(j=0; j<100; j++){

		}
	}
}
void motorDelay1(){
	int i=0;
	int j =0;
	for(i=0; i<1000; i++){
		for(j=0; j<90; j++){

		}
	}
}
void getRequest(){
	if(!ground){
		floors[0] = 1;
	}
	if(!floor1){
		floors[1] = 1;
	}
	if(!floor2){
		floors[2] = 1;
	}
	if(!floor3){
		floors[3] = 1;
	}
	if(!floor4){
		floors[4] = 1;
	}
	
	if(!ground_up){
		floors_up[0] = 1;
	}
	if(!floor1_up){
		floors_up[1] = 1;
	}
	if(!floor1_down){
		floors_down[0] = 1;
	}
	if(!floor2_up){
		floors_up[2] = 1;
	}
	if(!floor2_down){
		floors_down[1] = 1;
	}
	if(!floor3_up){
		floors_up[3] = 1;
	}
	if(!floor3_down){
		floors_down[2] = 1;
	}
	if(!floor4_down){
		floors_down[3] = 1;
	}
	
}
void openDoor(){
	led_open = 1;
	led_closed = 0;
	doorDelay();
	led_closed = 1;
	led_open = 0;
}
void goingUp(){
	motor1 = 1;
	motor2 = 0;
	motorDelay();
	motor1 = 0;
	motor2 = 1;
	motorDelay1();
	motor2 = 0;
}
void goingDown(){
	motor2 = 1;
	motor1 = 0;
	motorDelay();
	motor2 = 0;
	motor1 = 1;
	motorDelay1();
	motor1 = 0;
}
void Timer_init(){
	TMOD = 0x11;
	TH0 = 0xDD;
	TL0 = 0x00;
	TR0 = 1;
}
void Timer0_ISR() interrupt 1
{
	getRequest();
}
void External_interrupt_init(){
	EA  = 1;
	EX1 = 1;
	IT1 = 1;
}
void External1_ISR() interrupt 2
{
	openDoor();
}
int main(void){
	Timer_init();
	External_interrupt_init();
	led_open = 0;
	led_closed = 1;
	P3 = 0xC0;
	EA = 1;
	ET0 = 1;
	P2 = 63;
	while(1) {
		i = 0;
		for(i=0; i<5; i++){
			if((floors[i] || floors_up[i]) && i> currentFloor){
				while(currentFloor != i){
					goingUp();
					currentFloor++;
					P2 = lookUpTable[currentFloor];
					if(floors[currentFloor] || floors_up[currentFloor]){
						openDoor();
						floors[currentFloor] = 0;
						floors_up[currentFloor] = 0;
						floors[0] = 0;
						floors_up[0] = 0;
						floors_down[0] = 0;
					}
				}
			}
			if(floors_up[i] && i < currentFloor){
				while(currentFloor != i){
					goingDown();
					currentFloor--;
					P2 = lookUpTable[currentFloor];
					if(floors_up[currentFloor]){
						openDoor();
						floors_up[currentFloor] = 0;
					}
				}
			}
		}
		i=4;
		for(i=4; i>=0; i--){
			if((floors[i] || floors_down[i-1]) && i< currentFloor){
				while(currentFloor != i){
					goingDown();
					currentFloor--;
					P2 = lookUpTable[currentFloor];
					if(floors[currentFloor] || floors_down[currentFloor - 1]){
						openDoor();
						floors[currentFloor] = 0;
						floors_down[currentFloor - 1] = 0;
						floors_up[0] =0;
						floors[0] = 0;
					}
				}
			}
			if (floors_down[i-1] && i > currentFloor){
				while( currentFloor != i){
					goingUp();
					currentFloor++;
					P2 = lookUpTable[currentFloor];
					if(floors_down[currentFloor-1]){
						openDoor();
						floors_down[currentFloor -1]=0;
					}
				}
				
		}
	}
}
}