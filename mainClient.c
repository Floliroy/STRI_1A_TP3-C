#include <stdio.h> 
#include <stdlib.h> 
#include "client.h"

int main() {    
	char* message;
    //connection au serveur sur le port 80 pour envoi d'une requete http
	if(InitialisationAvecService("www.stri.ups-tlse.fr","80") != 1) {
		printf("Erreur d'initialisation\n");
		return 1;
	}
    //construction de la requete HTTP comme vu avec telnet
	if(Emission("GET / HTTP/1.1\nHost: www.stri.ups-tlse.fr\n\n")!=1) {
		printf("Erreur d'Émission\n");
		return 1;
	}   
    //affichage de ce que le serveur répond
	message = Reception();
	//si pas de réponse on affiche une erreur et on sort
	if(message == NULL){
		printf("Erreur de Réception\n");
		return 1;
	}       
	//sinon on affiche ce que le serveur renvoit
	printf("Reception :\n\n");
	while(message!=NULL){
		//tant qu'on recoit on affiche
		printf("%s", message);
		free(message);
		message = Reception();
	}

    return 0;
}