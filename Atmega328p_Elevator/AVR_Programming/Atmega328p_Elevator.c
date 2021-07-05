/* Header Files or Macro Declaration */
#define F_CPU 16000000UL 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define  SWITCH_FOR_1FLOOR  0x01
#define  SWITCH_FOR_2FLOOR  0x02
#define  SWITCH_FOR_3FLOOR  0x04
#define  SWITCH_FOR_4FLOOR  0x08
#define  SWITCH_FOR_5FLOOR  0x10
#define  SWITCH_FOR_6FLOOR  0x20

/* ^^^^^^^^^^^^^^^^^^^ */

/* Global Variables */

uint8_t LEDforFloor[6] = {0x04, 0x0C, 0x1C, 0x3C, 0x7C, 0xFC};
uint8_t currentFloor, destinationFloor;

/* ^^^^^^^^^^^^^^^^^^^ */

/* Function Prototypes */

ISR(PCINT1_vect){
	if(!(PINC & SWITCH_FOR_1FLOOR)){
		destinationFloor = 1;
	}else if(!(PINC & SWITCH_FOR_2FLOOR)){
		destinationFloor = 2;
	}else if(!(PINC & SWITCH_FOR_3FLOOR)){
		destinationFloor = 3;
	}else if(!(PINC & SWITCH_FOR_4FLOOR)){
		destinationFloor = 4;
	}else if(!(PINC & SWITCH_FOR_5FLOOR)){
		destinationFloor = 5;
	}else if(!(PINC & SWITCH_FOR_6FLOOR)){
		destinationFloor = 6;
	}else{
		
	}
}

void set_Atmega328pRegister(void);
void activate_ButtonInterrupt(void);
void pause_Motor(void);
void go_MotorCounterclock(void);
void go_MotorClock(void);
void display_LEDforFloor(uint8_t floorNumber);
void initialize_Elevator(void);

/* ^^^^^^^^^^^^^^^^^^^ */

int main(void)
{	
	set_Atmega328pRegister();
	activate_ButtonInterrupt();
	initialize_Elevator();
	
    do{
		if(currentFloor < destinationFloor){
			while(currentFloor != destinationFloor){
				currentFloor++;
				go_MotorCounterclock();
				_delay_ms(100);
				display_LEDforFloor(currentFloor);
			}
		}else if(currentFloor == destinationFloor){
			pause_Motor();
			display_LEDforFloor(currentFloor);
		}else{
			while(currentFloor != destinationFloor){
				currentFloor--;
				go_MotorClock();
				_delay_ms(100);
				display_LEDforFloor(currentFloor);
			}
		}
    }while(1);
}

/* Function Body */

void set_Atmega328pRegister(void)
{
	DDRB = 0x07;
	DDRC = 0x00;
	DDRD = 0xFC;
}

void activate_ButtonInterrupt(void)
{
	PCICR = 0x02;
	PCMSK1 = 0x3F;
	sei();
}

void pause_Motor(void)
{
	PORTB = 0x00;
}

void go_MotorCounterclock(void)
{
	PORTB = 0x05;
}

void go_MotorClock(void)
{
	PORTB = 0x06;
}

void display_LEDforFloor(uint8_t floorNumber)
{
	PORTD = LEDforFloor[floorNumber - 1];
}

void initialize_Elevator(void)
{
	currentFloor = 1;
	destinationFloor = 1;
	pause_Motor();
	display_LEDforFloor(currentFloor);
}

/* ^^^^^^^^^^^^^^^^^^^ */
