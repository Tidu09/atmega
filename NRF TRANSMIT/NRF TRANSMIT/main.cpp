/*
 * NRF TRANSMIT.cpp
 *
 * Created: 06-03-2019 16:57:06
 * Author : AYUSH
 */ 

#include <avr/io.h>
#include "C:\Users\AYUSH\Desktop\lib\nrf24lo1+\nrf24lo1+\library.cpp"
#include <avr\delay.h>



uint8_t status;



void spi(void)
{  
	DDRB |=(1<<PB5)|(1<<PB7)|(1<<PB4)|(1<<PB1); // MOSI SCK SS CE
	DDRB&=~(1<<PB6);//MISO
	SPCR|=(1<<SPE)|(1<<MSTR)|(1<<SPR0); //FOSC/32
	DDRB|=(1<<PB4); // SS HIGH
	PORTB &=~(1<<PB1);//CE
	
}

uint8_t  spi_transceiver ( uint8_t  val )  //uint8t transfer
{   SPDR=val;
	asm volatile("nop");
	while(!(SPSR&(1<<SPIF)));
	return (SPDR);
}


uint8_t read_byte(uint8_t reg)
{
	PORTB&=~(1<<PB4);
	//spi_transceiver(R_REGISTER|(REGISTER_MASK&reg));
	spi_transceiver(R_REGISTER|reg);
	reg=spi_transceiver(0xFF);
	PORTB|=(1<<PB4);
	return reg;
}


uint8_t writenrf(uint8_t reg, uint8_t data)
{
	PORTB&=~(1<<PB4);
    uint8_t status=	spi_transceiver(W_REGISTER|(REGISTER_MASK&reg));
	spi_transceiver(data);
	PORTB|=(1<<PB4);
    return status ;
}

void send( uint8_t a)
{    char  abc;
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
	UDR= '\t';
	
	k=(status)&0b11110000;
	k=k>>4;
	send(k);
	
	k=(status)&0b00001111;
	send(k);
	
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



void nrf24l01_int(void)
{
	writenrf(EN_AA,0x00);
	writenrf(EN_RXADDR,0x01);
	writenrf(SETUP_AW,0x03);
	writenrf(RF_CH,0x09);
	writenrf(RF_SETUP,0x26); 
	writenrf(FEATURE ,0x01);
	
	PORTB&=~(1<<PB4);
	spi_transceiver(W_REGISTER+RX_ADDR_P0);
	spi_transceiver(0X01);
	spi_transceiver(0x01);	
	spi_transceiver(0x01);
	spi_transceiver(0x01);
    spi_transceiver(0x01);
	PORTB|=(1<<PB4);
	
	
	PORTB&=~(1<<PB4);
	spi_transceiver(W_REGISTER+TX_ADDR);
	spi_transceiver(0x01);
	spi_transceiver(0x01);
	spi_transceiver(0x01);
	spi_transceiver(0x01);
	spi_transceiver(0x01);
	PORTB|=(1<<PB4);
	
	writenrf(RX_PW_P0,0x01);
	writenrf(CONFIG,0x7E);
	writenrf(SETUP_RETR,0x00);
	writenrf(STATUS,0x7E)	;
	
   	
}


void transmit(uint8_t data)
{   
	status=read_byte(STATUS);
	stc(status,'a') ;
	PORTB |=(1<<PB1);
    flush_tx();
    
	PORTB&=~(1<<PB4);
	spi_transceiver(W_TX_PAYLOAD);
	spi_transceiver(data);
	PORTB|=(1<<PB4);
	_delay_us(200);
	status=read_byte(STATUS);
	stc(status,'b') ;
	
//	PORTB |=(1<<PB1);
	//_delay_us(20);
	//PORTB &= ~(1<<PINB1);
//	_delay_us(20);
//	PORTB |=(1<<PINB1);
	
}

/*void receive(void)
{
	PORTD|=(1<<PD6); // start listening
	_delay_us(20);
	data =writenrf(0,R_RX_PAYLOAD,data,4);
	reset();
    PORTD&=~(1<<PD6);
}*/

int main(void)
{   UCSRC &=~(1<<URSEL);
	unsigned int  h = 51;
	UBRRH=(unsigned char)(h>>8);
	UBRRL=(unsigned char)h;
	
	UCSRC |=(1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);
	UCSRB &=~(1<<UCSZ2);
	UCSRC &=~(1<<UMSEL)& ~(1<<UPM1)& ~(1<<UPM0)&~(1<<USBS);
	UCSRB |=(1<<TXEN);
	
	spi();
	
	DDRD|=(1<<PD7);
   	
	
    while (1) 
  {
	  nrf24l01_int();
	  transmit('G');
	  status=read_byte(FIFO_STATUS);
	  stc(status,'C') ;
	  status=read_byte(STATUS);
	  stc(status,'d') ;
	 
	     
  
    }
}