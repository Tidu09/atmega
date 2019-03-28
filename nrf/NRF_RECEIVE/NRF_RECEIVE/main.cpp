/*
 * Nrf_main.c
 *
 * Created: 14-03-2019 18:35:09
 * Author : TIDU
 */ 



#include <avr/io.h>
#include "C:\Users\AYUSH\Desktop\lib\nrf24lo1+\nrf24lo1+\library.cpp"
#include <util/delay.h>

uint16_t x,y;


void spi(void)
{  
	DDRB |=(1<<PINB5)|(1<<PINB7)|(1<<PINB4)|(1<<PINB1); // MOSI SCK SS CE
	DDRB&=~(1<<PINB6);//MISO
	SPCR|=(1<<SPE)|(1<<MSTR)|(1<<SPR0); //FOSC/32
	
	PORTB&=~(1<<PINB1);//CE
	
}

uint8_t  spi_transceiver ( uint8_t  val )  //uint8t transfer
{   SPDR=val;
	while(!(SPSR&(1<<SPIF)));
	return (SPDR);
}


uint8_t read_byte(uint8_t reg)
{
	PORTB&=~(1<<PINB4);
    spi_transceiver(R_REGISTER|reg);
	reg=spi_transceiver(0xFF);
	PORTB|=(1<<PINB4);
	return reg;
}


uint8_t writenrf(uint8_t reg, uint8_t data)
{
	PORTB&=~(1<<PINB4);
    uint8_t status=	spi_transceiver(W_REGISTER|(REGISTER_MASK&reg));
	spi_transceiver(data);
	PORTB|=(1<<PINB4);
    return status ;
}

void send( uint8_t a)
{   
	 char  abc;
	switch(a)
	{case 0: abc='0';
		while (!(UCSRA&(1<<UDRE)));
		UDR= abc;
		break;
		case 1:abc='1';
		while (!(UCSRA&(1<<UDRE)));
		UDR= abc;
		break;
		case 2:abc='2';
		while (!(UCSRA&(1<<UDRE)));
		UDR= abc;
		break;
		case 3:abc='3';
		while (!(UCSRA&(1<<UDRE)));
		UDR= abc;
		break;
		case 4:abc='4';
		while (!(UCSRA&(1<<UDRE)));
		UDR= abc;
		break;
		case 5:abc='5';
		while (!(UCSRA&(1<<UDRE)));
		UDR= abc;
		break;
		case 6:abc='6';
		while (!(UCSRA&(1<<UDRE)));
		UDR= abc;
		break;
		case 7:abc='7';
		while (!(UCSRA&(1<<UDRE)));
		UDR= abc;
		break;
		case 8:abc='8';
		while (!(UCSRA&(1<<UDRE)));
		UDR= abc;
		break;
		case 9:abc='9';
		while (!(UCSRA&(1<<UDRE)));
		UDR= abc;
		break;
		case 10:abc='A';
		while (!(UCSRA&(1<<UDRE)));
		UDR= abc;
		break;
		case 11:abc='B';
		while (!(UCSRA&(1<<UDRE)));
		UDR= abc;
		break;
		case 12:abc='C';
		while (!(UCSRA&(1<<UDRE)));
		UDR= abc;
		break;
		case 13:abc='D';
		while (!(UCSRA&(1<<UDRE)));
		UDR= abc;
		break;
		case 14:abc='E';
		while (!(UCSRA&(1<<UDRE)));
		UDR= abc;
		break;
		case 15:abc='F';
		while (!(UCSRA&(1<<UDRE)));
		UDR= abc;
		break;
	}
}

void stc(  uint8_t status , char a)
{
	uint8_t k;
	
	while (!(UCSRA&(1<<UDRE)));
	UDR= a;
	while (!(UCSRA&(1<<UDRE)));
	
	k=(status)&0b11110000;
	k=k>>4;
	send(k);
	
	k=(status)&0b00001111;
	send(k);
	
	while (!(UCSRA&(1<<UDRE)));
	UDR= '\n';
	
}



void sendint(int o,char p)
{   uint8_t count=0;
	while (!(UCSRA&(1<<UDRE)));
	UDR= p;
	
	int n=0;
	int r;
	while(o>0)
	{r=o%10;
		n=n*10+r;
		o=o/10;
		count++;
	}

	
	
	
	while(count>0)
	{  char abc;
		r=n%10;
		switch(r)
		{    case 0:abc='0';
			while (!(UCSRA&(1<<UDRE)));
			UDR= abc;
			break;
			case 1:abc='1';
			while (!(UCSRA&(1<<UDRE)));
			UDR= abc;
			break;
			case 2:abc='2';
			while (!(UCSRA&(1<<UDRE)));
			UDR= abc;
			break;
			case 3:abc='3';
			while (!(UCSRA&(1<<UDRE)));
			UDR= abc;
			break;
			case 4:abc='4';
			while (!(UCSRA&(1<<UDRE)));
			UDR= abc;
			break;
			case 5:abc='5';
			while (!(UCSRA&(1<<UDRE)));
			UDR= abc;
			break;
			case 6:abc='6';
			while (!(UCSRA&(1<<UDRE)));
			UDR= abc;
			break;
			case 7:abc='7';
			while (!(UCSRA&(1<<UDRE)));
			UDR= abc;
			break;
			case 8:abc='8';
			while (!(UCSRA&(1<<UDRE)));
			UDR= abc;
			break;
			case 9:abc='9';
			while (!(UCSRA&(1<<UDRE)));
			UDR= abc;
			break;
			
			
		}
		n=n/10;
		--count;
	}
	while (!(UCSRA&(1<<UDRE)));
	UDR= '\n';
	
}
	


void flush_tx(void)
{
	PORTB&=~(1<<PB4);
	spi_transceiver(FLUSH_TX);
	PORTB|=(1<<PB4);

	
}

void flush_Rx(void)
{
	PORTB&=~(1<<PB4);
	spi_transceiver(FLUSH_RX);
	PORTB|=(1<<PB4);
}


void receive(void)  // updates global variable x and y dont return anything just update two variables when u call it
{   
	 uint8_t result;
	 uint8_t d1, d2, d3, d4;
	
	 PORTB&=~(1<<PB4);
	 spi_transceiver(R_RX_PAYLOAD);
	 d1=spi_transceiver(0xFF);
	 d2=spi_transceiver(0xFF);
	 d3=spi_transceiver(0xFF);
	 d4=spi_transceiver(0xFF);
	 PORTB|=(1<<PB4);
	 
	 _delay_ms(20);
	 flush_Rx();
	 _delay_ms(20);
	 
	 
	 x=d1;
	 x|=d2<<8;
	 
	 y=d3;
	 y|=d4<<8;
 
	 
	 _delay_ms(20);
	 flush_Rx();
	 _delay_ms(20);
	 
	
}

void nrf24l01_int(void)
{
	    writenrf(EN_AA,0x01);
	    writenrf(EN_RXADDR,0x01);
	    writenrf(SETUP_AW,0x03);
	    writenrf(RF_CH,0x78);
	    writenrf(RF_SETUP,0x26); //0b0010 0110
	    writenrf(FEATURE ,0x00);    // could be written to 0x01;
	 
	    PORTB&=~(1<<PINB4);
	    spi_transceiver(W_REGISTER+RX_ADDR_P0);
		spi_transceiver(0x01);
		spi_transceiver(0x01);
		spi_transceiver(0x01);
		spi_transceiver(0x01);
		spi_transceiver(0x01);
		PORTB|=(1<<PINB4);
	
	
	    PORTB&=~(1<<PINB4);
		spi_transceiver(W_REGISTER+TX_ADDR);
	    spi_transceiver(0x01);
		spi_transceiver(0x01);
		spi_transceiver(0x01);
		spi_transceiver(0x01);
	    spi_transceiver(0x01);
	    PORTB|=(1<<PINB4); 
	
	writenrf(RX_PW_P0,0x04);
	writenrf(CONFIG,0x7F);  

}



int main(void)
{   DDRC|=(1<<PC7)|(1<<PC6);
	DDRD|=(1<<PD4)|(PD5);
	DDRD |=(1<<PD5);
	DDRD |=(1<<PD4);

	UCSRC &=~(1<<URSEL);
	unsigned int  h = 51;
	UBRRH=(unsigned char)(h>>8);
	UBRRL=(unsigned char)h;
	
	UCSRC |=(1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);
	UCSRB &=~(1<<UCSZ2);
	UCSRC &=(1<<UMSEL)& ~(1<<UPM1)& ~(1<<UPM0)&(1<<USBS);
	UCSRB |=(1<<TXEN);
	
	
	spi();
	
	DDRD|=(1<<PD7);
 


		
   	
    while (1) 
    {	PORTB|=(1<<PB1); 
		nrf24l01_int();
		
        receive();
	 sendint(x,'a');
	 sendint(y,'b');
 

	  
	  
	 
	  
	
	
    }
}

