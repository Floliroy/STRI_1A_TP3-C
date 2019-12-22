#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "serveur.h"


//extrait le fichier (et son chemin) d'une requete
int extraitFichier(char *requete, char *tableauNomFichier, int tailleTableauNomFichier){
	int i, cpt;
	int taille = strlen(requete);

	for(i=0 ; i<taille-1 && requete[i+1] != '\n' ; i++){
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
			tableauNomFichier[cpt-1] = '\0';
			return 1;
		}
	}

	return -1;
}


//retourne la taille d'un fichier en octet
unsigned long longueur_fichier(char *nomFichier){
	FILE *fichier = fopen(nomFichier, "r");

    fseek(fichier, 0, SEEK_END);
    unsigned long taille = (unsigned long)ftell(fichier);
    fclose(fichier);

    return taille;
}


//envoie le contenu d'un fichier texte
int envoyerContenuFichierTexte(char *nomFichier){
	char ligne[511];

    //on ouvre le fichier en lecture seulement
    FILE *monFichier = fopen(nomFichier,"r");

    //on fait le scanf est vérifie avoir le bon nombre de retour
    while(fscanf(monFichier, "%s\n", ligne) == 1){
		//on envoit ce qui est lu
        if(Emission(strcat(ligne,"\n")) != 1) {
			printf("Erreur d'emission (fichier texte)\n");
			return 0;
		}
    }

    fclose(monFichier);
	return 1;
}


//envoie le contenu d'un fichier binaire
int envoyerContenuFichierBinaire(char *nomFichier){
	FILE *fichier;
	unsigned long taille = longueur_fichier(nomFichier);

	//on ouvrele fichier en lecture binaire
	fichier = fopen(nomFichier, "rb");
	char *buffer = (char*)malloc(taille);

	//on se place au début du fichier
	fseek(fichier, 0, SEEK_SET);
	fread(buffer, taille, 1, fichier);

	// On envoie le fichier binaire au client
	EmissionBinaire(buffer, taille);

	free(buffer);
	fclose(fichier);

	return 1;
}


//emet le content type puis length
int envoyerContentTypeLength(char *contentLength, char *contentType){
	//on emet les entêtes d'une page html
	if(Emission(contentType) != 1){
		printf("Erreur d'emission (content-type)\n");
		return 0;
	}

	//on emet le content-length
	if(Emission(contentLength) != 1){
		printf("Erreur d'emission (content-length)\n");
		return 0;
	}

	return 1;
}


//envoie une réponse 200 OK pour un fichier HTTP
int envoyerReponse200HTML(char *nomFichier){
	printf("200 HTML\n");
	char contentLength[31] = "Content-length: ";
	int size = longueur_fichier(nomFichier);
	//on concatène le content-length avec la taille du fichier
	sprintf(contentLength, "%d",size);
	strcat(contentLength, "\n\n");

	//on emet la réponse 200
	if(Emission("HTTP/1.1 200 OK\n") != 1){
		printf("Erreur d'emission (réponse 200)\n");
		return 0;
	}

	//on envoie le content type et length
	if(envoyerContentTypeLength(contentLength, "Content-Type: text/html; charset=UTF-8\n") != 1){
		return 0;
	}

	//on envoie le contenu de notre fichier
	envoyerContenuFichierTexte(nomFichier);
	
	return 1;
}


//envoie une réponse 200 OK pour un fichier JPG
int envoyerReponse200JPG(char *nomFichier){
	printf("200 JPG\n");
	char contentLength[31] = "Content-length: ";
	int size = longueur_fichier(nomFichier);
	//on concatène le content-length avec la taille du fichier
	sprintf(contentLength, "%d", size);
	strcat(contentLength, "\n\n");

	//on emet la réponse 200
	if(Emission("HTTP/1.1 200 OK\n") != 1){
		printf("Erreur d'emission (réponse 200)\n");
		return 0;
	}

	//on envoie le content type et length
	if(envoyerContentTypeLength(contentLength, "Content-Type: image/jpeg; charset=UTF-8\n") != 1){
		return 0;
	}

	//on envoie le contenu de notre fichier
	envoyerContenuFichierBinaire(nomFichier);
	
	return 1;
}


//envoie une réponse 200 OK pour un fichier ICO
int envoyerReponse200ICO(char *nomFichier){	
	printf("200 ICO\n");
	char contentLength[31] = "Content-length: ";
	int size = longueur_fichier(nomFichier);
	//on concatène le content-length avec la taille du fichier
	sprintf(contentLength, "%d", size);
	strcat(contentLength, "\n\n");

	//on emet la réponse 200
	if(Emission("HTTP/1.1 200 OK\n") != 1){
		printf("Erreur d'emission (réponse 200)\n");
		return 0;
	}

	//on envoie le content type et length
	if(envoyerContentTypeLength(contentLength, "Content-Type: image/x-icon; charset=UTF-8\n") != 1){
		return 0;
	}

	//on envoie le contenu de notre fichier
	envoyerContenuFichierBinaire(nomFichier);
	
	return 1;
}


//envoie une réponse 404 Not FOund en http
int envoyerReponse404(char *nomFichier){
	printf("404\n");
	char contentLength[31] = "Content-length: ";
	int size = longueur_fichier(nomFichier);
	//on concatène le content-length avec la taille du fichier
	sprintf(contentLength, "%d", size);
	strcat(contentLength, "\n\n");

	//on emet la réponse 200
	if(Emission("HTTP/1.1 404 Not Found\n") != 1){
		printf("Erreur d'emission (réponse 404)\n");
		return 0;
	}

	//on envoie le content type et length
	if(envoyerContentTypeLength(contentLength, "Content-Type: text/html; charset=UTF-8\n") != 1){
		return 0;
	}

	//on envoie le contenu de notre fichier
	envoyerContenuFichierTexte(nomFichier);
	
	return 1;
}


//envoie une réponse 404 Not FOund en http
int envoyerReponse500(char *nomFichier){
	printf("500\n");
	char contentLength[31] = "Content-length: ";
	int size = longueur_fichier(nomFichier);
	//on concatène le content-length avec la taille du fichier
	sprintf(contentLength, "%d", size);
	strcat(contentLength, "\n\n");

	//on emet la réponse 200
	if(Emission("HTTP/1.1 500 Server Error\n") != 1){
		printf("Erreur d'emission (réponse 500)\n");
		return 0;
	}

	//on envoie le content type et length
	if(envoyerContentTypeLength(contentLength, "Content-Type: text/html; charset=UTF-8\n") != 1){
		return 0;
	}

	//on envoie le contenu de notre fichier
	envoyerContenuFichierTexte(nomFichier);
	
	return 1;
}


//est ce que la requete est une requete GET
int isRequeteGet(char *requete){
	if(requete[0] == 'G' && requete[1] == 'E' && requete[2] == 'T'){
		return 1;
	}
	return 0;
}


//serveur sur localhost:13214
int main() {
	char *message = NULL;
	char *nomFichier = malloc(511*sizeof(char));
	char *extension = malloc(15*sizeof(char));
	FILE *fichier;

	Initialisation();

	while(1) {
		AttenteClient();
		message = Reception();

		if(message != NULL) {
			printf("J'ai recu: %s\n", message);

			//on verifie que la requete soit une requete GET et on recupere le nom de la ressource demandée
			if(isRequeteGet(message) == 1 && extraitFichier(message, nomFichier, 511) != -1){

				//on verifie que le fichier existe
				if((fichier = fopen(nomFichier, "r"))){

					//on récupère l'extension du fichier en cherchant le DERNIER '.' de notre chaine
					extension = strrchr(nomFichier, '.');

					//suivant l'extension on choisit si on execute la fonction JPG, ICO, ou HTML
					if(strcmp(extension, ".jpg") == 0 || strcmp(extension, ".jpeg") == 0){
						envoyerReponse200JPG(nomFichier);
					}else if(strcmp(extension, ".ico") == 0){
						envoyerReponse200ICO(nomFichier);
					}else{
						envoyerReponse200HTML(nomFichier);
					}

					//on ferme notre fichier qu'on a ouvert pour tester s'il existait
					fclose(fichier);

				}else{
					//la ressource n'existe pas ou envoi un 404 Not Found
					envoyerReponse404("404NotFound.html");
				}

			}else{
				//ce n'est pas une requete GET ou on a eu une erreur sur le serveur
				envoyerReponse500("500ServerError.html");
				TerminaisonClient();
			}
			free(message);

		}

		TerminaisonClient();
	}

	free(nomFichier);
	free(extension);

	return 0;
}
