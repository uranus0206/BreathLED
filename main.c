#include <msp430g2553.h>

unsigned int PWMPeriod = 0xFFFF;
unsigned int step = 200; //higher faster
volatile unsigned char direction=1;

void Timer_Init(void);

void main(void)
{   
    WDTCTL = WDTPW + WDTHOLD;
    BCSCTL1= CALBC1_8MHZ;
    DCOCTL = CALDCO_8MHZ;
    P1DIR |=BIT6;
    P1SEL |=BIT6;

    Timer_Init();
    
    __enable_interrupt();
    
    while(1)
    {
        
    }
}

void Timer_Init(void)
{
    TA0CTL =  TASSEL_2 | ID_0 | MC_2;
    TA0CCR0 = PWMPeriod;
    TA0CCTL1 = OUTMOD_7 | CCIE;
    TA0CCR1 = 0;
    
}

#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer0_A1(void)
{   
    switch(TAIV)
    {
        case TA1IV_TACCR1:
        {
            if(direction == 1)
            {
                if((TA0CCR1 < (int)(0.8*PWMPeriod)))
                {
                    TA0CCR1 += step;
                }
                else
                {
                    TA0CCR1 = (int)(0.8*PWMPeriod);
                    direction = 0;
                }
            }
            else if(direction == 0)
            {
                if((TA0CCR1 > (int)(0.01*PWMPeriod)))
                {
                    TA0CCR1 -= step;
                }
                else
                {
                    TA0CCR1 = (int)(0.01*PWMPeriod);
                    direction = 1;
                }
            }
            
            TA0CCTL1 &= ~CCIFG;
            break;
        }
        
        case TA1IV_TACCR2:
        {
            break;
        }
        
        case TA1IV_TAIFG:
        {
            TA0CTL &= ~TAIFG;
            break;
        }
    }
}