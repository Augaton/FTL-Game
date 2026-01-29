#include "utils.h"
#include "vaisseau.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void effacerEcran() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void afficherGameOver(Vaisseau *joueur) {
    effacerEcran();
    printf(COLOR_RED "\n\n");
    printf("      ############################################\n");
    printf("      #                                          #\n");
    printf("      #          VAISSEAU DETRUIT...             #\n");
    printf("      #                                          #\n");
    printf("      ############################################" COLOR_RESET "\n\n");

    printf(COLOR_BOLD "--- RAPPORT DE MISSION FINAL ---" COLOR_RESET "\n");
    printf(" Commandant        : %s\n", joueur->nom);
    printf(" Secteurs parcourus: %d / %d\n", joueur->distanceParcourue, joueur->distanceObjectif);
    printf(" Fortune restante  : %d ferrailles\n", joueur->ferraille);
    
    // Message personnalisé selon la progression
    printf("\n" COLOR_YELLOW " Verdict du Haut-Commandement : " COLOR_RESET);
    if (joueur->distanceParcourue < joueur->distanceObjectif / 4) 
        printf("Une recrue qui n'a pas survécu au premier saut.\n");
    else if (joueur->distanceParcourue < joueur->distanceObjectif / 2)
        printf("Un pilote prometteur, emporté par l'immensité du vide.\n");
    else if (joueur->distanceParcourue < joueur->distanceObjectif)
        printf("Un héros de la résistance. On se souviendra de votre bravoure.\n");
    else
        printf("INCROYABLE ! Vous avez sauvé la galaxie avant de succomber.\n");

    printf("\n\n[ Appuyez sur ENTREE pour quitter le jeu ]\n");
    
    // Nettoyage final pour s'assurer que l'utilisateur voit le message
    int c; while ((c = getchar()) != '\n' && c != EOF);
    getchar();
}

void afficherVictoire(Vaisseau *joueur) {
    effacerEcran();
    printf(COLOR_GREEN "\n\n");
    printf("      ********************************************\n");
    printf("      * *\n");
    printf("      * VICTOIRE GALACTIQUE            *\n");
    printf("      * *\n");
    printf("      ********************************************" COLOR_RESET "\n\n");

    printf(COLOR_BOLD "--- RAPPORT DE MISSION FINAL ---" COLOR_RESET "\n");
    printf(" Commandant        : %s\n", joueur->nom);
    printf(" Secteurs parcourus: %d / %d\n", joueur->distanceParcourue, joueur->distanceObjectif);
    printf(" Fortune restante  : %d ferrailles\n", joueur->ferraille);
}

void attendreJoueur() {
    printf(COLOR_CYAN "\n[ Appuyez sur ENTREE pour continuer ]" COLOR_RESET);
    int c;
    while ((c = getchar()) != '\n' && c != EOF); // Nettoie le buffer
    getchar();
}

// Sauvegarde et chargement

void sauvegarderPartie(Vaisseau *v) {
    FILE *fichier = fopen("savegame.dat", "wb"); // 'wb' pour Write Binary
    if (fichier == NULL) {
        printf("\n" COLOR_RED "[ERREUR] Impossible de créer le fichier de sauvegarde." COLOR_RESET "\n");
        return;
    }

    // On écrit toute la structure d'un coup
    fwrite(v, sizeof(Vaisseau), 1, fichier);
    
    fclose(fichier);
    printf("\n" COLOR_GREEN "💾 Progression synchronisée avec les serveurs de la Fédération." COLOR_RESET "\n");
}

int chargerPartie(Vaisseau *v) {
    FILE *fichier = fopen("savegame.dat", "rb"); // 'rb' pour Read Binary
    if (fichier == NULL) {
        return 0; // Pas de sauvegarde trouvée
    }

    fread(v, sizeof(Vaisseau), 1, fichier);
    fclose(fichier);
    return 1; // Chargement réussi
}

void supprimerSauvegarde() {
    // On vérifie si le fichier existe avant de tenter de le supprimer
    if (remove("savegame.dat") == 0) {
        printf(COLOR_RED "\n[SYSTEME] Données de vol effacées. Fin de transmission.\n" COLOR_RESET);
    } else {
        // Le fichier n'existait probablement pas, rien à faire
    }
}

void finaliserEvenement(Vaisseau *joueur) {
    strcpy(joueur->secteurActuel, "REPOS");
    joueur->ennemiPresent = 0;
    joueur->ennemiCoqueActuelle = 0;
    sauvegarderPartie(joueur);
}

// Fonction utilitaire pour lire un entier de manière robuste
int lireEntierSecurise(int min, int max) {
    int valeur;
    char buffer[100]; // On lit une ligne de texte pour éviter les bugs de scanf

    while (1) {
        // On lit toute la ligne (jusqu'à Entrée)
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            // On essaie de convertir le texte en nombre
            // %d capture le nombre, le reste est ignoré
            if (sscanf(buffer, "%d", &valeur) == 1) {
                // Si c'est un nombre valide, on vérifie les bornes
                if (valeur >= min && valeur <= max) {
                    return valeur; // C'est gagné, on renvoie la valeur
                }
            }
        }
        
        // Si on arrive ici, c'est que l'entrée était mauvaise
        printf(COLOR_RED "⚠ Entrée invalide. Veuillez taper un nombre entre %d et %d : " COLOR_RESET, min, max);
        printf(COLOR_YELLOW "> " COLOR_RESET);
    }
}

// Petite fonction utilitaire pour l'affichage coloré
void afficherDestinationColoree(const char* destination) {
    if (strstr(destination, "Hostile")) printf(COLOR_RED);
    else if (strstr(destination, "Station")) printf(COLOR_GREEN);
    else if (strstr(destination, "Detresse")) printf(COLOR_YELLOW);
    else if (strstr(destination, "Nebuleuse")) printf(COLOR_MAGENTA);
    else printf(COLOR_CYAN);
    
    printf("%s" COLOR_RESET, destination);
}