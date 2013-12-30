/*********************************
This code provide you to print an
analog value on ADC channel 0
to the USART of an ATmega 328p

by Mathieu Souchet
*********************************/

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
 
#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)
 
//Declaration of our functions
void USART_init(void);
void adc_init(void);
unsigned char USART_receive(void);
void USART_send( unsigned char data);	//Send a char
void USART_putstring(char* StringPtr);	//Send a string
uint16_t adc_value;            //Variable used to store
uint16_t read_adc(uint8_t channel);
char buffer[5];
 
int main(void){
USART_init();        //Call the USART initialization code
adc_init();
 
while(1){        //Infinite loop
 adc_value=read_adc(0);
 itoa(adc_value,buffer,10);
 USART_putstring(buffer);    //Pass the string to the USART_putstring function and sends it over the serial
 USART_send('\r');			//Carriage return
 USART_send('\n');			//Linefeed
 _delay_ms(500);
 }
 
return 0;
}
 
//Initialise USART 
void USART_init(void){
 
 UBRR0H = (uint8_t)(BAUD_PRESCALLER>>8);
 UBRR0L = (uint8_t)(BAUD_PRESCALLER);
 UCSR0B = (1<<RXEN0)|(1<<TXEN0);
 UCSR0C = (3<<UCSZ00);
}
 
unsigned char USART_receive(void){
 
 while(!(UCSR0A & (1<<RXC0)));
 return UDR0;
 
}

//Send a char via USART 
void USART_send( unsigned char data){
 
 while(!(UCSR0A & (1<<UDRE0)));
 UDR0 = data;
 
}
 
//Send a String via USART
void USART_putstring(char* StringPtr){
 
while(*StringPtr != 0x00){
 USART_send(*StringPtr);
 StringPtr++;}
 
}

//Initialise ADConverter
void adc_init(void){
 ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));    //16Mhz/128 = 125Khz the ADC reference clock
 ADMUX |= (1<<REFS0);                //Voltage reference from Avcc (5v)
 ADCSRA |= (1<<ADEN);                //Turn on ADC
 ADCSRA |= (1<<ADSC);                //Do an initial conversion because this one is the slowest and to ensure that everything is up and running
}

//REturn a value from the ADC
uint16_t read_adc(uint8_t channel){
 ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);		//Select ADC channel with safety mask
 ADCSRA |= (1<<ADSC);                //Starts a new conversion
 while(ADCSRA & (1<<ADSC));            //Wait until the conversion is done
 return ADCW;                    //Returns the ADC value of the chosen channel
}