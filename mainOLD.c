#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define GROUND1 PB0
#define GROUND2 PB1

#define A PA0
#define B PA1
#define C PA2
#define D PA3

#define E PA4
#define F PA5
#define G PA6
#define DP PA7

#define delay 500

uint8_t DICT[10] = 
{	
	~(1 << G), // 0
	(1 << B) | (1 << C), // 1
	~((1 << C) | (1 << F)), // 2
	~((1 << E) | (1 << F)), // 3
	~((1 << A) | (1 << D) | (1 << E)), // 4
	~((1 << B) | (1 << E)), // 5
	~(1 << B), // 6
	((1 << A) | (1 << B) | (1 << C)), // 7
	0xFF, // 8
	~(1 << E) // 9
};

uint8_t LEFT_VAL = 8, RIGHT_VAL = 8, CURRENT = 0;

ISR(TIMER1_OVF_vect) 
{
	if(CURRENT)
	{
		PORTB &= ~(1 << GROUND2);
		PORTB |= (1 << GROUND1);
		PORTA = DICT[LEFT_VAL];
		CURRENT = 0;
	}
	else
	{
		PORTB &= ~(1 << GROUND1);
		PORTB |= (1 << GROUND2);
		PORTA = DICT[RIGHT_VAL];
		CURRENT = 1;
	}
}

uint16_t adc_read(uint8_t ch)
{
	ADMUX = (ADMUX & 0b1110000) | (ch & 0x1F); 
	
	ADCSRA |= (1 << ADSC);
	//~ cli();
	while(ADCSRA & (1 << ADSC));
	//~ sei();
	return (ADC);
}


int main(void)
{
	DDRB |= (1 << GROUND1) | (1 << GROUND2);
	DDRA = 0xFF;
	
	//~ TCCR1B |= (1 << CS13);
	//~ TCCR1B |= (1 << CS13) | (1 << CS11) | (1 << CS10);
	
	TCCR1B |= (1 << CS12) | (1 << CS11);
	TIMSK |= (1 << TOIE1);
	
	//~ ADMUX |= (1 << REFS0);
	//~ ADCSRA |= (1 << ADEN);
	//~ ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	
	//~ ADCSRB |= (1 << MUX5);
	
	//~ DIDR1 |= (1 << ADC10D) | (1 << ADC9D) | (1 << ADC8D) | (1 << ADC7D);
	
	sei();
	while(1)
	{
		//~ LEFT_VAL = (uint8_t) (adc_read(7)/(uint16_t) 113);
		//~ RIGHT_VAL = (uint8_t) (adc_read(8)/(uint16_t)113);
		
		for(uint8_t i = 0; i < 10; i++)
		{
			for(uint8_t j = 0; j < 10; j++)
			{
				LEFT_VAL = i;
				RIGHT_VAL = i;
				_delay_ms(100);
			}
		}
	}
	
	return 1;
}
