#include<stdio.h>
#include<avr/io.h>
#include<avr/interrupt.h>
#include <util/delay.h>
char  enable=0,reset=0,pause=1;
char num[6]={0} ;

ISR(TIMER1_COMPA_vect)
	{
		enable=1;


	}
ISR(INT0_vect)
{
	reset=1;
}
ISR(INT1_vect)
{
	pause=0;
}
ISR(INT2_vect)
{
	pause=1;
}
int main(void)
{

	SREG|=(1<<7);                        /* I BIT ENABLE*/
	TIMSK|=(1<<OCIE1A);                  /* TIMER COMARE INTERRUPT ENABLE*/
	MCUCR|=(1<<ISC01)|(1<<ISC10)|(1<<ISC11);  /* SENSE CONTROL CONFIGURATION OF INT0 &INT1 */
	GICR|=(1<<5)|(1<<6)|(1<<7);



			DDRA|=(1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5); /* output configuration*/
			DDRC|=(1<<0)|(1<<1)|(1<<2)|(1<<3);				 /* output configuration*/
			DDRB&=~(1<<2);									 /*input configuration*/
			PORTB|=(1<<2);									 /* internal pull up resistor enable*/
			DDRD&=~(1<<2);									 /*input configuration*/
			PORTD|=(1<<2);									 /* internal pull up resistor enable*/
			DDRD&=~(1<<3);									 /*input configuration*/
			DDRB&=~(1<<2);
			PORTD|=(1<<2);

			TCCR1B|=(1<<WGM12);                              /*configure timer as ctc mode*/

			TCCR1B|=(1<<CS12);                              /*prescaler =256*/



			OCR1A=3906;										/* set compare value*/

	int i=0;
	while(1)
	{
		if(reset)
		{
			for(int j=0;j<6;j++)
			{
				num[j]=0;
			}
			reset=0;
		}

		if(enable&&pause)
		{
			/*seconds count*/
			num[0]++;
			if(num[0]==10)
			{
				num[0]=0;
				num[1]++;
			}
			if((num[1]==6))
			{
				num[0]=0;
				num[1]=0;
				num[2]++;
			}

			/*minutes count*/

			if(num[2]==10)
			{
				num[2]=0;
				num[3]++;
			}
			if((num[3]==6))
			{
				num[2]=0;
				num[3]=0;
				num[4]++;
			}

			/*hours count*/

			if((num[4]==10)&&(num[5]<2))
			{
				num[4]=0;
				num[5]++;
			}
			if((num[5]==2)&&(num[4]==4))
			{
				for(int j=0;j<6;j++)
				{
					num[j]=0;
				}
			}
			enable=0;
		}
		PORTA=0;
		PORTA|=(1<<i);

		PORTC=(PORTC&0xf0)|(num[i]&0x0f);
		i++;
		if(i==7)
		{
			i=0;
		}

		_delay_ms(2);
	}

}


