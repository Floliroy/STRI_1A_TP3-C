#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "serveur.h"

//extrait le fichier (et son chemin) d'une requete
int extraitFichier(char *requete, char *tableauNomFichier, int tailleTableauNomFichier){
	int i, cpt;
	int taille = strlen(requete);

	for(i=0 ; i<taille-1 && requete[i+1] != '\r' ; i++){
		if(requete[i] == '/' && requete[i+1] == ' '){
			//si on a un '/' suivi d'un espace
			strcpy(tableauNomFichier, "index.html");
			return 3;
		}else if(requete[i] == '/'){
			//si on a un '/' mais pas d'espace juste après
			cpt = 0;
			while(requete[i+cpt] != ' ' && cpt < tailleTableauNomFichier){
				//tant qu'on a pas d'espace (marquant la fin du fichier)
				tableauNomFichier[cpt] = requete[i+cpt+1];
				cpt++;
			}
			tableauNomFichier[cpt] = '\0';
			return 1;
		}
	}

	return -1;
}

//retourne la taille d'un fichier en octet
unsigned long longueur_fichier(char *nomFichier){
	FILE * fichier = fopen(nomFichier, "r");

    fseek(fichier, 0, SEEK_END);
    unsigned long taille = (unsigned long)ftell(fichier);
    fclose(fichier);

    return taille;
}

//envoie le contenu d'un fichier texte
int envoyerContenuFichierTexte(char *nomFichier){
	char* ligne;

    //on ouvre le fichier en lecture seulement
    FILE* monFichier = fopen(nomFichier,"r");

    //on fait le scanf est vérifie avoir le bon nombre de retour
    while(fscanf(monFichier, "%s\n", ligne) == 1){
		//on envoit ce qui est lu
        if(Emission(ligne) != 1) {
			printf("Erreur d'emission (fichier texte)\n");
			return 0;
		}
    }

    fclose(monFichier);
	return 1;
}

//envoie une réponse 200 OK en http
int envoyerReponse200HTML(char *nomFichier){
	char contentLenght[17] = "Content-length: ";
	int size = longueur_fichier(nomFichier);
	//on concatène le content-length avec la taille du fichier
	sprintf(contentLenght, "%d",size);
	strcat(contentLenght, "\n\n");

	//on emet la réponse 200
	if (Emission("HTTP/1.1 200 OK\n") != 1){
		printf("Erreur d'emission (réponse 200)\n");
		return 0;
	}

	//on emet les entêtes d'une page html
	if(Emission("Content-Type: text/html; charset=UTF-8\n") != 1){
		printf("Erreur d'emission (content-type)\n");
		return 0;
	}

	//on emet le content-length
	if (Emission(contentLenght) == -1){
		printf("Erreur d'emission (content-length)\n");
		return 0;
	}
	return 1;
}


int main() {
	//char* test = (char*) malloc(100 * sizeof(char));

	char *message = NULL;

	Initialisation();

	while(1) {
		int fini = 0;
	
		AttenteClient();
	
		while(!fini) {
			message = Reception();

			if(message != NULL) {
				printf("J'ai recu: %s\n", message);
				extraitFichier(message, test, 100);
				printf("Test (requete reelle) : %s\n", test);
				free(message);
		
				if(Emission("Test de message serveur.\n")!=1) {
					printf("Erreur d'emission\n");
				}
			} else {
				fini = 1;
			}
		}

		TerminaisonClient();
	}

	return 0;
}
