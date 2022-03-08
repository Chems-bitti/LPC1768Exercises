#include "LPC17xx.h"

/* Le Handler de l'interruption : On y accède à chaque nouvel échantillon 
 * Donc à chaque front descendant du signal "new"
 * On vérifie la valeur de la température
 * On fait ce qu'on doit faire en fonction de sa valeur
 */
void EINT3_IRQHandler() {
  LPC_GPIOINT->IO0IntClr |= 1 << 2; // On acquitte l'interruption
	unsigned int temp = ((LPC_GPIO0->FIOPIN & (0xFF << 16)) >> 16); // On lit la valeur de la temperature (bits 16 à 23);
	// On vérifie si la température est supérieure à 35°C
  if((temp > 35) && (temp < 45 ) && (temp < 50)) { 
    LPC_PWM1->MR2 = 25E6/60E3 * 0.5; // On fixe le rapport cyclique du PWM1.2 à 50%
		LPC_PWM1->LER |= 1 << 2; // On met à 1 le bit 2 du LER afin de mettre à jour le Match Register 2
		LPC_PWM1->TCR |= 1; // On démarre le PWM
	}
  // On vérifie si la température est supérieure à 45°C   
  else if((temp > 45) && (temp < 50)) { 
		LPC_PWM1->MR4 =25E6/60E3*0.9;// On fixe le rapport cyclique du PWM1.4 et PWM1.2 à 90%
		LPC_PWM1->MR2 =25E6/60E3*0.9;
		LPC_PWM1->LER |= 3 << 2;// On active les bits 2 et 3 du LER pour mettre à jour le Match Register 2 et Match Register 3
	}
  // On vérifie si la température est supérieure à 50°C
  else if(temp > 50) {
		// Démarrer les timers 0 et 2
		LPC_TIM0->TCR |= 1;
		LPC_TIM2->TCR |= 1;
	}
  // Sinon, donc si la température est inférieure à 35°C
  else {
    // On remet le rapport cyclique à 0
		LPC_PWM1->MR4 =0;
		LPC_PWM1->MR2 = 0;
		LPC_PWM1->LER |= 3 << 2;// Mettre à jour les MR3 et MR2
		LPC_PWM1->TCR &= ~(9 << 2); // Arrêter les PWM
    // Arrêter les timers
		LPC_TIM0->TCR &= ~1; 
		LPC_TIM2->TCR &= ~1;
		LPC_GPIO2->FIOPIN &= ~(1<<6); // Mettre à 0 la sortie qui contrôle l'alarme
		
	}
}
/* Timer0 Interrupt Handler
 * Genère un signal de rapport cyclique de 50%
 * Permute l'état du P2.6 à chaque 1/30KHz
 */
void TIMER0_IRQHandler() {
	LPC_TIM0->IR |= 1; // Acquitter
	LPC_GPIO2->FIOPIN ^= (1 << 6); // Permuter
}
/* Timer2 Interrupt Handler
 * Arrête le timer 0 après 30 secondes
 */
void TIMER2_IRQHandler() {
	LPC_TIM0->IR |= 1; // Acquitter
	LPC_TIM0->TCR &= ~(1); // Arrêter le timer
	LPC_GPIO2->FIOPIN &= ~(1<<6); // Mettre à 0 le P2.6
}
/* Initialiser Timer 0
 * Définir les valeurs du Match Register 0
 * Activer les interruption dans le NVIC
 */
void init_TIMER0(){
  LPC_TIM0->MR0 = 25E6/30E3;// à chaque demi période (1/(15KHz*2)), on doit générer une interruption pour changer la valeur du P2.6
  LPC_TIM0->MCR |= 3; // Générer une interruption et reset le timer à chaque fois qu'on arrive à MR0
	NVIC_EnableIRQ(TIMER0_IRQn); // Activer l'interruption
}
/* Initialize TIMER 2
 * Même concept que Timer 0
 */
void init_TIMER2() {
	LPC_TIM2->MR0 = 25E6*30; // Période de 30 secondes
	LPC_TIM2->MCR |= 7; // Générer une interruption et reset le timer à chaque fois qu'on arrive à MR0
  NVIC_EnableIRQ(TIMER2_IRQn); // Activer l'interruption
}
     
/* Initialiser PWM   */
void init_PWM() {
	LPC_PINCON->PINSEL3 |= (1 << 9) | (1 << 15); // Configurer P1.20 and P1.23 à PWM1.2 and PWM1.3 
	LPC_PWM1->PCR |= (5 << 10); // Activer PWM1.2 and PWM1.4
	LPC_PWM1->MR0 = 25E6/60E3; // Fixer la fréquence à 60KHz
  LPC_PWM1->LER |= 1; // Mettre à jour MR0	
	LPC_PWM1->MR2 = 0; // Rapport cyclique de 0% pour PWM1.2
	LPC_PWM1->MR4 = 0; // Rapport cyclique de 0% pour PWM1.4
	LPC_PWM1->MCR |= 2; // Reset timer quand on arrive à MR0
	LPC_PWM1->TCR |= 1 << 3; // Activer le mode PWM
}
     
/* Initialiser les GPIO */
void init_GPIO() {
	LPC_PINCON->PINSEL0 &= ~(3 << 4); // Mettre P0.2 en mode GPIO, ça sera notre ligne "new"
	LPC_GPIO0->FIODIR &= ~(1 << 2); // Mettre P0.2 en entrée
	LPC_PINCON->PINSEL1 &= ~( 0xFFFF ); // P0.16->P0.23 en GPIO
	LPC_GPIO0->FIODIR &= ~(0xFF << 16); // P0.16->P0.23 en entrée
	
	LPC_PINCON->PINSEL4 &= ~(3<<12); // P2.6 en GPIO
	LPC_GPIO2->FIODIR |= 1 << 6; // P2.6 en sortie
	LPC_GPIO2->FIOPIN &= ~(1<<6); // Reset P2.6 à 0
	
}
/* Initialiser les interruption GPIO
 * On veut qu'à chaque front descendant de P0.2 ("new" passe à 0)
 * Générer une interruption
 */
void init_IR() {
	LPC_GPIOINT->IO0IntEnF |= 1 << 2; // générer une interrutpion sur front descendant
	LPC_GPIOINT->IO0IntClr |= 1 << 2; // Acquitter les interruptions en attente
	NVIC_EnableIRQ(EINT3_IRQn); // Activer l'interruption
}
int main() {
	// Initialisation
	init_GPIO();
	init_IR();
	init_PWM();
	init_TIMER0();
	init_TIMER2();
	// Attente
	while(1);
	
	return 0;
}
