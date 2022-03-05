#include <LPC17xx.h>
/*************************** TD 2 *****************************/

/** Exercice 1 : Scrutation **/
/* Faire clignoter les LED*/
/* qst 1-2 : par appui continu sur le bouton connecté à P2.13 

// On commence par notre fonction d'initialisation des ports GPIO

void init_GPIO(){
	// On sait que les ports P2.2 à P2.6 sont des GPIO
	// Donc les bits 4 à 13 du PINSEL4 doivent être à 0
	LPC_PINCON->PINSEL4 &= ~(0x3FF << 4); // ou comme montré dans le TD1 : ~(1 << 4) & ~(1 << 5) & ~(1 << 6) & ~(1 << 7) & ~(1 << 8) & ~(1 << 9) & ~(1 << 10) & ~(1 << 11) & ~(1 << 12) & ~(1 << 13);
	// Et ils sont en sortie
	LPC_GPIO2->FIODIR |= (0x1F << 2);
	// Et on veut qu'ils soient éteint par défaut (bouton pas appuyé)
	LPC_GPIO2->FIOPIN &= ~(0x1F << 2);
	
	// De la même façon, P2.13 est un GPIO
	LPC_PINCON->PINSEL4 &= ~(3 << 26);
	// Et c'est une entrée
	LPC_GPIO2->FIODIR &= ~(1 << 13);
}

int main() {
	// On appelle notre fonction d'initialisation GPIO
	init_GPIO();
	
	// On scrute dans notre boucle infinie
	while(1) {
		// N'oublions pas de masquer
		if(LPC_GPIO2->FIOPIN & (1 << 13)) {
			LPC_GPIO2->FIOPIN &= ~(0x1F << 2);
		}			
		else {
			LPC_GPIO2->FIOPIN |= (0x1F << 2);
		}
	}
}
*/

/* Allumer les leds comme un chenillard 
// On garde la même fonction d'initialisation

void init_GPIO(){
	// On sait que les ports P2.2 à P2.6 sont des GPIO
	// Donc les bits 4 à 13 du PINSEL4 doivent être à 0
	LPC_PINCON->PINSEL4 &= ~(0x3FF << 4); // ou comme montré dans le TD1 : ~(1 << 4) & ~(1 << 5) & ~(1 << 6) & ~(1 << 7) & ~(1 << 8) & ~(1 << 9) & ~(1 << 10) & ~(1 << 11) & ~(1 << 12) & ~(1 << 13);
	// Et ils sont en sortie
	LPC_GPIO2->FIODIR |= (0x1F << 2);
	// Et on veut qu'ils soient éteint par défaut (bouton pas appuyé)
	LPC_GPIO2->FIOPIN &= ~(0x1F << 2);
	
	// De la même façon, P2.13 est un GPIO
	LPC_PINCON->PINSEL4 &= ~(3 << 26);
	// Et c'est une entrée
	LPC_GPIO2->FIODIR &= ~(1 << 13);
}

// On aura besoin d'une fonction qui va introduire un delay entre chaque allumage d'une LED
// N'oubliez pas de changer le niveau d'optimisation du compilateur afin que ça marche
// Clique droite sur Target 1 à gauche -> Options for Target -> l'onglet C/C++ -> Optimization : choisir -O0
void delay() {
	for(int i = 0; i < 100000; i++);
}

// et on modifie un peu notre fonction main

int main() {
	// On appelle notre fonction d'initialisation GPIO
	init_GPIO();
	// On déclare une variable
	int LED = 1, flag = 1;
	// On scrute dans notre boucle infinie
	while(1) {
		// on appelle notre fonction delay au début ou à la fin, ça change pas grand chose
		delay();
		// On met notre variable sur les LED P2.2 à P2.6
		LPC_GPIO2->FIOPIN = (LED << 2); // On se permet de le mettre directement car les ports configurés en entrées ne seront pas changé
		
		// Maintenant pour le décalage
		// On veut décaler LED vers la gauche si flag == 1
		// Quand ça arrive à sa valeur max (1 << 4), on inverse flag
		// On décale vers la droite si flag == -1
		// Quand ça arrive à sa valeur min (1), on inverse flag
		// et ainsi de suite
		if (LED == (1 << 4)) {
			flag = -flag;
		}
		if(flag == 1) {
			LED = LED << 1;
		}		
		if (LED == 1) {
			flag = -flag;
		}		
		if(flag == -1) {
			LED = LED >> 1;
		}
	}
}
*/

/** Exercice 2 : Interruption par GPIO **/
/* Commuter les LEDs à chaque appui (front descendant) */
// On garde la même fonction d'initialisation qu'avant
//void init_GPIO(){
//	// On sait que les ports P2.2 à P2.6 sont des GPIO
//	// Donc les bits 4 à 13 du PINSEL4 doivent être à 0
//	LPC_PINCON->PINSEL4 &= ~(0x3FF << 4); // ou comme montré dans le TD1 : ~(1 << 4) & ~(1 << 5) & ~(1 << 6) & ~(1 << 7) & ~(1 << 8) & ~(1 << 9) & ~(1 << 10) & ~(1 << 11) & ~(1 << 12) & ~(1 << 13);
//	// Et ils sont en sortie
//	LPC_GPIO2->FIODIR |= (0x1F << 2);
//	// Et on veut qu'ils soient éteint par défaut (bouton pas appuyé)
//	LPC_GPIO2->FIOPIN &= ~(0x1F << 2);
//	
//	// De la même façon, P2.13 est un GPIO
//	LPC_PINCON->PINSEL4 &= ~(3 << 26);
//	// Et c'est une entrée
//	LPC_GPIO2->FIODIR &= ~(1 << 13);
//}

// On initialise nos interruptions
//void init_IR() {
//	// On va utiliser une interruption GPIO sur le front descendant du port P2.13
//	// En lisant la documentation, ceci corresponds à la mise à 1 du bit 13 du registre LPC_GPIOINT->IO2IntEnf
//	LPC_GPIOINT->IO2IntEnF |= 1 << 13;
//	// Ensuite, il faut clear les interruptions en attente "just to be safe"
//	LPC_GPIOINT->IO2IntClr |= (1 << 13);
//	// Et enfin, l'activer chez le NVIC
//	// Les interruptions GPIO sont tous activés avec le EINT3_IRQn
//	NVIC_EnableIRQ(EINT3_IRQn);
//}

//// Maintenant pour notre interruption handler, on veut commuter l'état des leds
//void EINT3_IRQHandler(){
//	// D'abord, on oublie pas d'acquitter, pour une raison ou une autre, il faut le faire au début du handler
//	LPC_GPIOINT->IO2IntClr |= (1 <<13 );
//	// Facile, on xor à chaque fois qu'on rentre dans la fonction, donc à chaque front descendant, donc à chaque appui
//	LPC_GPIO2->FIOPIN ^= (0x1F << 2);
//}

//int main() {
//	
//	init_GPIO();
//	init_IR();
//	
//	while(1);
//	
//	return 0;
//}

/* Qst 2 : éteindre si on appuie, allumer si on relache */
// Plusieurs façons de faire ça, je propose d'activer un interruption sur un front descendant *et* un font montant
// Donc, modifier init_IR seulement, n'oubliez pas de le mettre avant le main !
// Aussi, il faut changer la valeur par défaut des LEDs, on doit alors les mettre à 1 à la fin du init_GPIO()
//void init_IR() {
//	// On va utiliser une interruption GPIO sur le front descendant et montant du port P2.13
//	// En lisant la documentation, ceci corresponds à la mise à 1 du bit 13 du registre LPC_GPIOINT->IO2IntEnF et LPC_GPIOINT->IO2IntEnR
//	LPC_GPIOINT->IO2IntEnF |= 1 << 13;
//	LPC_GPIOINT->IO2IntEnR |= 1 << 13;
//	// Ensuite, il faut clear les interruptions en attente "just to be safe"
//	LPC_GPIOINT->IO2IntClr |= (1 << 13);
//	// Et enfin, l'activer chez le NVIC 
//	// Les interruptions GPIO sont tous activés avec le EINT3_IRQn
//	NVIC_EnableIRQ(EINT3_IRQn);
//}


/** Exercice 3 : Interruption Externe **/
/* Qst 1 : Commuter les leds à chaque appui et relachement (front montant) */

// comme d'hab, on commence par notre init_GPIO(), mais cette fois notre bouton (P2.11) ne sera plus un GPIO, on va le configurer dans une autre fonction
void init_GPIO(){
	// On sait que les ports P2.2 à P2.6 sont des GPIO
	// Donc les bits 4 à 13 du PINSEL4 doivent être à 0
	LPC_PINCON->PINSEL4 &= ~(0x3FF << 4); // ou comme montré dans le TD1 : ~(1 << 4) & ~(1 << 5) & ~(1 << 6) & ~(1 << 7) & ~(1 << 8) & ~(1 << 9) & ~(1 << 10) & ~(1 << 11) & ~(1 << 12) & ~(1 << 13);
	// Et ils sont en sortie
	LPC_GPIO2->FIODIR |= (0x1F << 2);
	// Et on veut qu'ils soient éteint par défaut (bouton pas appuyé)
	LPC_GPIO2->FIOPIN &= ~(0x1F << 2);
	
}
// initialisation de l'interruption externe
void init_EXT() {
	// D'abord, il faut configurer le port P2.11 en External interrupt 1
	// Ceci correspond à mettre les bits 23 et 22 du PINSEL4 à 01
	LPC_PINCON->PINSEL4 |= 1 << 22;
	// On peut maintenant activer les detection du front
	LPC_SC->EXTMODE |= 1 << 1;
	// Et ensuite, activer l'activation sur front montant
	LPC_SC->EXTPOLAR |= 1 << 1;
	
	// On oublie pas de clear les interruption en attente
	LPC_SC->EXTINT |= 1 << 1;
	// On peut maintenant activer l'interruption
	NVIC_EnableIRQ(EINT1_IRQn);
}

// Pour le handler, le même que l'exo précédent mais avec un changement de nom
void EINT1_IRQHandler(){
	// D'abord, on oublie pas d'acquitter, pour une raison ou une autre, il faut le faire au début du handler
	LPC_SC->EXTINT |= 1 << 1;
	// Facile, on xor à chaque fois qu'on rentre dans la fonction, donc à chaque front descendant, donc à chaque appui
	LPC_GPIO2->FIOPIN ^= (0x1F << 2);
}

int main() {
	init_GPIO();
	init_EXT();
	
	while(1);
	
	return 0;
}

/* Qst 2 :Faire clignoter à chaque appui*/
// Je n'ai absolument aucune idée comment faire ça sans scruter au moins un flag
