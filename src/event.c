#include "event.h"
#include "combat.h"
#include "utils.h"
#include "magasin.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void menuVoyage(Vaisseau *joueur) {
    int choix = 0;

    while (choix != 1) { // Tant que le joueur ne choisit pas de sauter
        effacerEcran();
        printf("--- TABLEAU DE BORD : %s ---\n", joueur->nom);
        printf("Distance parcourue : %d / 10\n", joueur->distanceParcourue);
        printf("Carburant : %d | Ferraille : %d | Missiles : %d\n", 
                joueur->carburant, joueur->ferraille, joueur->missiles);
        printf("Coque : %d/%d | Bouclier : %d/%d\n", 
                joueur->coque, joueur->coqueMax, joueur->bouclier, joueur->bouclierMax);
        printf("--------------------------------------\n");
        printf("1. Sauter vers le prochain secteur (-1 Carburant)\n");
        printf("2. Consulter le journal de bord (Stats)\n");
        printf("3. Quitter le jeu\n");
        printf("\nVotre choix : ");
        scanf("%d", &choix);

        if (choix == 1) {
            const char* baliseA = inspecterBalise();
            const char* baliseB = inspecterBalise();
            int choixSaut;

            printf("\n--- CARTE STELLAIRE ---\n");
            printf("1. %s\n", baliseA);
            printf("2. %s\n", baliseB);
            printf("Votre choix : ");
            scanf("%d", &choixSaut);

            const char* destination = (choixSaut == 1) ? baliseA : baliseB;

            if (strcmp(destination, "Nebuleuse (Inconnu - Gratuit)") == 0) {
                printf("\n[ECONOMIE] Vous utilisez les courants de la nebuleuse (0 Carburant).\n");
            } else {
                if (joueur->carburant > 0) {
                    joueur->carburant--;
                } else {
                    printf("\n[ALERTE] Plus de carburant ! Coque endommagee par la derive.\n");
                    joueur->coque -= 5;
                }
            }

            joueur->distanceParcourue++;
            executerEvenement(joueur, destination);
            
            if (joueur->coque <= 0) break;
        }
        else if (choix == 2) {
            printf("\n--- STATISTIQUES AVANCEES ---\n");
            printf("Puissance des Lasers : %d\n", joueur->armes);
            printf("Chance d'esquive : 15%%\n");
            printf("\n[Appuyez sur une touche pour revenir]");
            getchar(); getchar(); // Petite pause
        }
        else if (choix == 3) {
            printf("Extinction des systemes...\n");
            joueur->coque = 0; // Pour sortir de la boucle du main
            break;
        }
    }
}

const char* inspecterBalise() {
    int r = rand() % 100;
    if (r < 10) return "Nebuleuse (Inconnu - Gratuit)";
    if (r < 40) return "Signal Hostile (Combat)";
    if (r < 55) return "Station Commerciale (Magasin)";
    if (r < 80) return "Signal de Detresse";
    return "Secteur Vide";
}

void executerEvenement(Vaisseau *joueur, const char* type) {
    const char* evenementFinal = type;

    if (strcmp(type, "Nebuleuse (Inconnu - Gratuit)") == 0) {
        printf("\n[SYSTEME] Entree dans la nebuleuse. Capteurs brouilles...\n");
        int r = rand() % 100;
        if (r < 50) evenementFinal = "Signal Hostile (Combat)";
        else if (r < 80) evenementFinal = "Signal de Detresse";
        else evenementFinal = "Secteur Vide";
    }

    if (strcmp(evenementFinal, "Signal Hostile (Combat)") == 0) {
        lancerCombat(joueur);
    } else if (strcmp(evenementFinal, "Station Commerciale (Magasin)") == 0) {
        ouvrirMagasin(joueur);
    } else if (strcmp(evenementFinal, "Signal de Detresse") == 0) {
        evenementDeresse(joueur);
    } else {
        printf("\n[INFO] La zone est deserte.\n");
    }
}
void evenementDeresse(Vaisseau *joueur) {
    int choix;
    printf("\n[SIGNAL DE DETRESSE] Un transporteur civil est en panne.\n");
    printf("1. Aider (70%% chance de succes)\n");
    printf("2. Ignorer\n");
    printf("Votre decision : ");
    scanf("%d", &choix);

    if (choix == 1) {
        if ((rand() % 100) < 70) {
            joueur->ferraille += 20;
            printf("Succes ! +20 Ferraille.\n");
        } else {
            joueur->coque -= 3;
            printf("Piege ! L'explosion vous inflige 3 degats.\n");
        }
    } else {
        printf("Vous passez votre chemin.\n");
    }
}