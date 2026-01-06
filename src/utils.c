#include "utils.h"
#include "vaisseau.h"
#include <stdio.h>
#include <stdlib.h>

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
    printf(" Secteurs parcourus: %d / 20\n", joueur->distanceParcourue);
    printf(" Fortune restante  : %d ferrailles\n", joueur->ferraille);
    
    // Message personnalisé selon la progression
    printf("\n" COLOR_YELLOW " Verdict du Haut-Commandement : " COLOR_RESET);
    if (joueur->distanceParcourue < 5) 
        printf("Une recrue qui n'a pas survécu au premier saut.\n");
    else if (joueur->distanceParcourue < 15)
        printf("Un pilote prometteur, emporté par l'immensité du vide.\n");
    else if (joueur->distanceParcourue < 20)
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
    printf(" Secteurs parcourus: %d / 20\n", joueur->distanceParcourue);
    printf(" Fortune restante  : %d ferrailles\n", joueur->ferraille);
}

void attendreJoueur() {
    printf(COLOR_CYAN "\n[ Appuyez sur ENTREE pour continuer ]" COLOR_RESET);
    int c;
    while ((c = getchar()) != '\n' && c != EOF); // Nettoie le buffer
    getchar(); // Attend la pression de Entrée
}