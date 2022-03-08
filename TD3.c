#include <LPC17xx.h>

void TIMER0_IRQHandler() {
	LPC_TIM0->IR |= 1;
	LPC_GPIO2->FIOPIN ^= 1;
}

void TIMER1_IRQHandler() {
	LPC_TIM1->IR |= 1;
  LPC_TIM0->TCR &= ~1;
	LPC_GPIO2->FIOPIN &= ~1;
}

void TIMER2_IRQHandler() {
	LPC_TIM2->IR |= 1;
  LPC_TIM1->TCR ^= 1;
	LPC_TIM0->TCR ^= 1;
}
void EINT3_IRQHandler() {
	LPC_GPIOINT->IO2IntClr |= (1 << 10); 
	LPC_TIM2->TCR |= 1;
	LPC_TIM1->TCR |= 1;
	LPC_TIM0->TCR |= 1;
}
void init_GPIO() {
	LPC_PINCON->PINSEL4 &= ~(3);
	LPC_PINCON->PINSEL4 &= ~(3 << 20);
	LPC_GPIO2->FIODIR &= ~(1 << 10);
	LPC_GPIO2->FIODIR |= 1;
	LPC_GPIOINT->IO2IntEnF |= (1 << 10);
	LPC_GPIOINT->IO2IntClr |= (1 << 10);
	NVIC_EnableIRQ(EINT3_IRQn);
}

void init_T2() {
	LPC_TIM2->MR0 = 25E6*20E-3;
	LPC_TIM2->MCR |= 3;
}
void isr_T2() {
	NVIC_EnableIRQ(TIMER2_IRQn);
	
}
void init_T1() {
	LPC_TIM1->MR0 = 25E6*1E-3;
	LPC_TIM1->MCR |= 7;
}
void isr_T1() {
	NVIC_EnableIRQ(TIMER1_IRQn);
}
void init_T0() {
	LPC_TIM0->MR0 = 25E6/80E3;
	LPC_TIM0->MCR |= 3;
}

void isr_T0() {
	NVIC_EnableIRQ(TIMER0_IRQn);
}

int main() {
	init_GPIO();
	init_T2();
	isr_T2();
	init_T1();
	isr_T1();
	init_T0();
	isr_T0();
	
	while(1);
	
	return 0;
}
