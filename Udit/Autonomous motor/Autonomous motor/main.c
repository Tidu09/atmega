/*
 * Autonomous motor.c
 *
 * Created: 27-03-2019 14:29:20
 * Author : TIDU
 */ 



 

#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>


int joyval;

int X;
int Y;
int joy;
int Tenbit1;
int Tenbit2;
int Tenbit3;
int Tenbit4;
int tenbitx;
int tenbity;
int hvx;
int lvx;
int hvy;
int lvy;
int joy;
int k;
int x=512,y=512;

unsigned int UBBRval=12;





void Pwm_initialize(void)
{
	DDRB|= (1<<PINB3);
	
	DDRD|=(1<<PIND7)|(1<<PIND5)|(1<<PIND4);
	
	DDRB|=(1<<PINB0);
	DDRB|=(1<<PINB1);
	
	TCCR0|= (1<<WGM00)|(1<<WGM01)|(1<<COM01)|(1<<CS00) ; // PWM setting for pinb3, timer 0
	TCCR2|= (1<<WGM20)|(1<<WGM21)|(1<<COM21)|(1<<CS20) ; // PWM setting for pind7 time
	
	TCCR1A|= (1<<WGM10)|(1<<WGM11)|(1<<COM1A1)|(1<<COM1B1);
	TCCR1B|= (1<<WGM12)|(1<<CS10);
	
}




int drive(int X,int Y)
{
	if(X==0 && Y==0)
	{
		PORTB &=~(1<<PINB0);
		PORTB &=~(1<<PINB1);
		OCR1A=0;
		OCR1B=0;
	}
	
	
	
	else  if (X >= 0 && Y >= 0)          // First quadrant
	{
		int level = X - Y;
		if (level == 0)
		{
			OCR1A=abs(X);
			OCR1B=0;
			PORTB=(1<<PINB0)|(1<<PINB1);
			
		}
		if (level > 0)
		{
			
			OCR1A=abs(X);
			OCR1B=abs(X-Y);
			PORTB=(1<<PINB0)|(0<<PINB1);
			
		}
		if (level < 0)
		{
			OCR1A=abs(Y);
			OCR1B=abs(X-Y);
			PORTB=(1<<PINB0)|(1<<PINB1);
		}
	}
	
	else if (X <= 0 && Y >= 0) // Second Quad
	{
		int level = X + Y;
		if (level == 0)
		{
			OCR1B=abs(Y);
			OCR1A=0;
			PORTB=(1<<PINB0)|(1<<PINB1);
		}
		if (level > 0)
		{
			OCR1A=abs(X+Y);
			OCR1B=abs(Y);
			PORTB=(1<<PINB0)|(1<<PINB1);

		}
		if (level < 0)
		{
			OCR1A=abs(X+Y);
			OCR1B=abs(X);
			PORTB=(0<<PINB0)|(1<<PINB1);
		}
	}
	
	
	else if (X >= 0 && Y <= 0) // Fourth quad
	{
		int level = X + Y;
		if (level == 0)
		{
			OCR1A=abs(0);
			OCR1B=abs(Y);
			PORTB=(1<<PINB0)|(0<<PINB1);
		}
		if (level > 0)
		{
			OCR1A=abs(X+Y);
			OCR1B=abs(X);
			PORTB=(1<<PINB0)|(0<<PINB1);

		}
		if (level < 0)
		{
			OCR1A=abs(X+Y);
			OCR1B=abs(Y);
			PORTB=(0<<PINB0)|(0<<PINB1);
		}
	}
	
	
	else if (X <=0 && Y <=0) // Third quad
	{
		int level = X - Y;
		if (level == 0)
		{
			OCR1A=abs(X);
			OCR1B=abs(0);
			PORTB=(0<<PINB0)|(1<<PINB1);
		}
		if (level > 0)
		{
			
			OCR1A=abs(Y);
			OCR1B=abs(X-Y);
			PORTB=(0<<PINB0)|(0<<PINB1);
		}
		if (level < 0)
		{
			OCR1A=abs(X);
			OCR1B=abs(X-Y);
			PORTB=(0<<PINB0)|(1<<PINB1);
			
		}
	}
	return 0;
}


void Uart_initialize(unsigned int UBBRval)
{
	UBRRH=(unsigned char) (UBBRval>>8);
	UBRRL=(unsigned char) (UBBRval);
	UCSRB=(1<<RXEN);
	UCSRC=(1<<URSEL)|(1<<USBS)|(1<<UCSZ1)|(1<<UCSZ0);
	//	UCSRA=1<<U2X;
	
}


void Receive(void)
{ //  PORTC^=(1<<PINC0);
	while(!(UCSRA & (1<<RXC)));
	k= UDR;
	auto_condition();
	
}



void auto_condition()
{
	if(k==1)    //front 
	{
		x=512;
		y=870;
	}
	
	else if(k==2)   // back
	{
		x=512;
		y=153;
	}
	
	else if(k==3)    // right
	{
	   x=767;
	   y=512;	
	}
	else if(k==4)    // left
	{
		x=256;
		y=512;
	}
	else if (k==5)   // stop
	{
		x=512;
		y=512;
	}
	else if(k==6)  // 30% right 
	{
		x=665;
		y=512;
	}
	
	else if(k==7)    // 30% left
	{
		x=358;
		y=512;
	}
	
	
	
	else if (k==8)   // leander 
	{
	     x=256;
		 y=0;	
	}
	
	else if (k==9)  // leander 
	{
		x=256;
		y=0;
	}
	
}




int main(void)
{
	Pwm_initialize();
	DDRD|=(1<<PINC1);
	/* Replace with your application code */
	Uart_initialize(UBBRval);
	while (1)
	{    k=5;
		Receive();
		
		X=(x*2-1023);
		Y=(y*2-1023);
		
		
		drive(X,Y);
		
		//PORTB|=(1<<PINB1);
		
	}
}
