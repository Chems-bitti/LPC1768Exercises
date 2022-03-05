/*************************** TD 1 *****************************/

/** Exercice 3 **/
/* Mettre le port 2.5 à 1 
int main() {

	// D'abord, il faut le configurer en GPIO
	// Donc mettre les bits 10 et 11 à 0 dans le registre PINSEL4
	// 3 c'est "11" en binaire,
  // le décaler de 10 puis l'inverser nous donne un nombre où tous les bits sont à 1
  // sauf les bits 10 et 11
	// On fait ça afin de conserver les valeurs des autres bits
	//LPC_PINCON->PINSEL4 &= ~(3 << 10); 
	//									^ Parce qu'on veut mettre à 0
	
	// On peut aussi l'écrire comme ça:
	//LPC_PINCON->PINSEL4 &= ~(1 << 10) & ~(1 << 11);
														
	// Ensuite, il faut le configurer en sortie
	// Ce qui correspond à mettre le bit 5 à 1 dans le registre LPC_GPIO2->FIODIR
	LPC_GPIO2->FIODIR |= (1 << 5);
	//                ^ Parce qu'on veut mettre à 1
	// Maintenant on peut écrire la valeure qu'on veut dans un le port
	LPC_GPIO2->FIOPIN |= (1 << 5); // Encore, on ne veut pas changer les valeurs des autres ports
	
	
	//Comment mettre à 1 sans changer les autres valeurs en générale :
	// *Registre* |= (1 << *rang 1*) | (1 << *rang 2*) | (1 << *rang3*) ...etc
	
	
	// Comment mettre à 0 sans changer les autres valeurs en générale :
	
	// *Registre* &= ~(1 << *rang1*) & ~(1 << *rang2*) & ~(1 << *rang3*) ...etc
	// il y'a d'autres méthodes mais bon, je trouve celle là la plus explicite
	
	
	// Maintenant on se met dans la boucle infinie
	while(1);
	
	return 0;
}
*/


/** Exercice 4 **/
/* Activer le port P0.8 si P0.5 est désactivé
 * Alors, on aura P0.8 en sortie et P0.5 en entrée
 * Par défaut, les entrées sont à 1, donc si on appuie sur un bouton
 * connecté à P0.5, le port va passer à 0.
 * On peut modifier la valeur par défaut des entrées mais j'ai jamais fait ça donc osef
 */
 
int main() {
	// D'abord, il faut mettre les ports P0.8 et P0.5 en mode GPIO
	// Donc mettre les bits 10 et 11 et 16 et 17 du registre PINSEL0 à 0
	LPC_PINCON->PINSEL0 &= ~(1 << 10) & ~(1 << 11) & ~(1 << 16) & ~(1 << 17);
	
	// Ensuite, configurer le port P0.8 en sortie et P0.5 en entrée
	LPC_GPIO0->FIODIR |= 1 << 8; // Mettre à 1
	LPC_GPIO0->FIODIR &= ~(1 << 5); // Mettre à 0
	// On fait ça comme ça afin de ne pas cchanger les autres valeurs
	
	// Maintenant, on peut entrer dans la boucle infinie et vérifier si le bouton a été appuyé infiniment
	int res;
	while(1) {
		// On veut regarder si le bit 5 vaut 0, donc on doit masquer la valeur du registre qui est sur 32 bits
		// Ce masque est juste un 1 décalé de 5, on obtient alors la valeur du bit 5
		// Si elle est à 0, ça veut dire que le bouton a été appuyé

		if((LPC_GPIO0->FIOPIN & (1 << 5)) == 0) { // on pourra aussi écrire if(!(LPC_GPIO0->FIOPIN & (1 << 5))) tout court
			// On met à 1 notre sortie
			LPC_GPIO0->FIOPIN |= (1 << 8);
		}
		// Sinon
		else {
			// On met à 0 notre sortie
			LPC_GPIO0->FIOPIN &= ~(1 << 8);
		}
	}
	return 0;
}
/* Comment masquer un registre pour obtenir la valeur d'un ou plusieurs bits: 
	int res = *Registre* & ((1 << *rang1*) | (1 << *rang2*) | (1 << *rang3*)) ...etc
 * Par exemple, pour obtenir la valeur du bit 3 du registre LPC_GPIO2->FIOPIN :
	int res = LPC_GPIO2->FIOPIN & (1 << 3);
 * Mais attention, ici res sera égale à 0000000000000000000000000000X000
																																		^ La valeur du bit 3
 * Si on veut la valeur non décalée, on doit re-décaler notre variable res :
	res = res >> 3;
 * Ou, si on veut le faire dans une seule instruction : 
  int res = (LPC_GPIO2->FIOPIN & (1 << 3)) >> 3;
*/
