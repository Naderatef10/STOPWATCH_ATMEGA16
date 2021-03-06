#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
void init_timer1();
void display_segments();
void fix_watch();
void init_interrupt();
unsigned char segments[6]={0,0,0,0,0,0}; // ARRAY TO CONTROL THE VALUES OF THE 6 SEVEN  SEGMENT DISPLAYS
int main(){
	DDRC |= 0x0F;
	DDRA |= 0b00111111;
	init_timer1();
	init_interrupt();
	while(1){
		fix_watch();
		display_segments();



	}

}
// function to initalize timer1 in compare mode to count 1 second
void init_timer1(){
	SREG |= (1<<7);
	TIMSK |= (1<<OCIE1A);
	TCCR1A = (1<<FOC1A);
	TCNT1=0;
	OCR1A=1000;
	TCCR1B= (1<<WGM12) | (1<<CS12) | (1<<CS10);
}
// INTERRUPT SERVICE ROUTINE OF TIMER1 COMPARE MODE
ISR(TIMER1_COMPA_vect){
	SREG |= (1<<7);
	segments[5]++;
}
// FUNCTION TO EDIT THE DIGITS OF THE SEVEN SEGMENT DISPLAY WHEN THEY OVER FLOW
void fix_watch(){
	if(segments[5]==10){
		segments[5]=0;
		segments[4]++;
	}
	if(segments[4]==6){
		segments[4]=0;
		segments[3]++;
	}
	if(segments[3]==10){
		segments[3]=0;
		segments[2]++;

	}
	if(segments[2]==6){
		segments[2]=0;
		segments[1]++;
	}
	if(segments[1]==10){
		segments[1]=0;
		segments[0]++;
	}
	if(segments[0]==2 && segments[1]==4){
		for(int i=5;i>=0;i--){
			segments[i]=0;
		}


	}
}
// FUNCTION TO DISPLAY THE DIGITS OF THE 6 SEVEN SEGMENTS DISPLAY
void display_segments(){
	for(int i=5;i>=0;i--){
		PORTA |= (1<<i);
		PORTC= (PORTC&0xF0) | (segments[i]&0x0F);
		_delay_us(200);
		PORTA &= ~(1<<i);
	}


}
// FUNCTION TO INITIALIZE THE EXTERNAL INTERRUPTS PINS OF ATMEGA16
void init_interrupt(){
	SREG |= (1<<7);
	DDRD &= ~(1<<PD2) & ~(1<<PD3); // make pins PD2 and PD3 inputs
	DDRB &= ~(1<<PB2); // make pin PB2 input
	PORTD |= (1<<PD2); // activate internal pull up resistor of pin pd2
	PORTB |= (1<<PB2); // activate internal pull up resistor of pin pb2
	MCUCR |= (1<<ISC11)	 | (1<<ISC10) | (1<<ISC01);
	MCUCSR &= ~(1<<ISC2);
	GICR |= (1<<INT1) | (1<<INT2) | (1<<INT0);
}

ISR(INT0_vect){
	for(unsigned char i=0;i<6;i++){
		segments[i]=0;
	}

}

ISR(INT1_vect){
	TCCR1B=0;
}

ISR(INT2_vect){

	TCCR1B= (1<<WGM12) | (1<<CS12) | (1<<CS10);

}
