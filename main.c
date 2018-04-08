#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define GROUND1 PA0
#define GROUND2 PA1

#define A PB1
#define B PB0
#define C PB2
#define D PB3
#define E PB4
#define F PB5
#define G PB6

#define MOTOR PA7

#define MIN 11
#define MAX 25

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

uint8_t PULSE;

uint8_t LEFT_VAL = 8, RIGHT_VAL = 8, CURRENT = 0;

uint16_t TICK, CLOCK, TIME;


ISR(TIMER1_OVF_vect) // 122 Hz interrupt frequency
{
	if(CURRENT)
	{
		PORTA &= ~(1 << GROUND2);
		PORTA |= (1 << GROUND1);
		PORTB = DICT[LEFT_VAL];
		CURRENT = 0;
	}
	else
	{
		PORTA &= ~(1 << GROUND1);
		PORTA |= (1 << GROUND2);
		PORTB = DICT[RIGHT_VAL];
		CURRENT = 1;
	}
}

ISR(TIMER0_COMPA_vect) // 20 KHz interrupt frequency
{
	if(TICK >= 400)	// One servo frame (20ms) completed
	{
		TICK = 0;
	}

	TICK = TICK + 1;
	if(TICK <= PULSE)	// Generate servo pulse
	{
		PORTA |= (1 << MOTOR);	// Servo pulse high
	}
	
	else
	{
		PORTA &= ~(1 << MOTOR);	// Servo pulse low
	}	
} 

ISR(TIMER1_COMPA_vect)	// 122 Hz interrupt frequency
{
	if(CLOCK > TIME)
	{
		PULSE = MIN;
		TIMSK &= ~(1 << OCIE1A);
	}
	else CLOCK += 1;
}


uint16_t adc_read(uint8_t ch)
{
	ADMUX = (ADMUX & 0b1110000) | (ch & 0x1F); 
	
	ADCSRA |= (1 << ADSC);
	while(ADCSRA & (1 << ADSC));
	
	return (ADC);
}

void setupADC(void)
{
	ADMUX |= (1 << REFS0);
	ADCSRA |= (1 << ADEN);
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
		
	DIDR0 |= (1 << ADC3D) | (1 << ADC4D);
	
	PORTA |= (1 << PA6);
}

void setupDisplay(void)
{
	DDRB = 0xFF;
	DDRA |= (1 << GROUND1) | (1 << GROUND2);
	
	//~ TCCR1B |= (1 << CS12) | (1 << CS11); // 32 prescaler
	TCCR1B |= (1 << CS13) | (1 << CS10); // 264 prescaler
	
	OCR1A = 120;
	
	TIMSK |= (1 << TOIE1);
}

void setupMotor(void)
{
	TICK = 0;
	PULSE = MIN;
	
	TCCR0A |= (1 << CTC0);
	TCCR0B |= (1 << CS01); // 8 prescaler
	
	OCR0A = 50;
	
	DDRA |= (1 << MOTOR);
	
	TIMSK |= (1 << OCIE0A);	
}

int main(void)
{
	setupDisplay();
	setupADC();
	setupMotor();
	
	sei();
	
	uint8_t ntimes;
	while(1)
	{
		ntimes = 0;
		LEFT_VAL = adc_read(4) / 113;
		RIGHT_VAL = adc_read(3) / 113;
		
		for(uint8_t i = 0; i < 5; i++)
		{
			if(adc_read(5) < 100) ntimes += 1;
			if(ntimes == 5) 
			{
				CLOCK = 0;
				TIME = (RIGHT_VAL + 1) * 31;
				
				TIMSK |= (1 << OCIE1A);
				
				PULSE = ((MAX - MIN)*LEFT_VAL)/9 + MIN;
				break;
			}
		}
		_delay_ms(50);
	}
	
	return 1;
}
