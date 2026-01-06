#include "combat.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void afficherEtatCombat(Vaisseau *joueur, Vaisseau *ennemi) {
    effacerEcran();
    printf(COLOR_CYAN "╔══════════════════════════════════════════════════╗\n");
    printf("║" COLOR_RESET "   VOUS          " COLOR_YELLOW "vs" COLOR_RESET "      %-20s" COLOR_CYAN "║\n", ennemi->nom);
    printf("║" COLOR_RESET "   Coque: " COLOR_RED "%-7d" COLOR_RESET " |   Coque: " COLOR_RED "%-15d" COLOR_CYAN "║\n", 
            joueur->coque, ennemi->coque);
    printf("║" COLOR_RESET "   Shield: " COLOR_CYAN "%-6d" COLOR_RESET " |   Shield: " COLOR_CYAN "%-14d" COLOR_CYAN "║\n", 
            joueur->bouclier, ennemi->bouclier);
    printf("╚══════════════════════════════════════════════════╝" COLOR_RESET "\n");
}

void lancerCombat(Vaisseau *joueur) {
    Vaisseau ennemi = genererEnnemi(joueur->distanceParcourue);
    
    SLEEP_MS(1500);

    while (joueur->coque > 0 && ennemi.coque > 0) {
        tourCombat(joueur, &ennemi);
    }

    if (joueur->coque > 0) {
        int gain = (rand() % 20) + 15;
        printf(COLOR_GREEN "\nVICTOIRE ! " COLOR_RESET "Vous recuperez " COLOR_YELLOW "%d Ferraille" COLOR_RESET ".\n", gain);
        joueur->ferraille += gain;
    } else {
        printf(COLOR_RED "\n[DETRUITE] Votre vaisseau se desintegre dans le vide...\n" COLOR_RESET);
    }
    SLEEP_MS(2000);
}

void tourCombat(Vaisseau *joueur, Vaisseau *ennemi) {
    int choixAction, choixArme;
    afficherEtatCombat(joueur, ennemi);

    printf("\n--- VOTRE TOUR ---\n");
    printf("1. ATTAQUER\n");
    printf("2. RECHARGER BOUCLIERS (Regen +2 a +4)\n");
    printf("Choix : ");
    scanf("%d", &choixAction);

    if (choixAction == 1) {
        // --- SOUS-MENU ATTAQUE ---
        printf("\n--- CHOIX DE L'ARME ---\n");
        printf("1. Canon Laser (Bloque par bouclier)\n");
        printf("2. Missile (Ignore bouclier | Munitions: %d)\n", joueur->missiles);
        printf("Choix : ");
        scanf("%d", &choixArme);

        printf("\nFeu !");
        SLEEP_MS(600);

        if (checkEsquive(15, joueur)) {
            printf("\nL'ennemi a esquive votre tir !\n");
        } else {
            if (choixArme == 2 && joueur->missiles > 0) {
                joueur->missiles--;

                int degatsFinaux = calculerDegats(3 + (joueur->distanceParcourue / 5), joueur->moteurs);
                ennemi->coque -= degatsFinaux;


                printf("\nMISSILE : Impact direct sur la coque (" COLOR_RED "-%d" COLOR_RESET") !\n", degatsFinaux);
            } else {
                int degatsFinaux = calculerDegats(joueur->armes, joueur->moteurs);

                if (ennemi->bouclier > 0) {
                    ennemi->bouclier -= degatsFinaux;
                    if (ennemi->bouclier < 0) ennemi->bouclier = 0;
                    printf(COLOR_CYAN "\nLASER : Le bouclier ennemi absorbe le choc. (" COLOR_RED "-%d" COLOR_CYAN ")\n" COLOR_RESET, degatsFinaux);
                } else {
                    ennemi->coque -= degatsFinaux;
                    printf("\nLASER : Impact sur la coque (" COLOR_RED "-%d" COLOR_RESET") !\n", degatsFinaux);
                }
            }
        }
        // Recharge bonus
        if (joueur->bouclier < joueur->bouclierMax) {
            joueur->bouclier += 1;
            printf("[SYSTEME] Recharge automatique du bouclier (+1).\n");
        }

    } else if (choixAction == 2) {
        // --- ACTION DE RECHARGE ---
        int regen = (rand() % 3) + 2;
        joueur->bouclier += regen;
        if (joueur->bouclier > joueur->bouclierMax) {
            joueur->bouclier = joueur->bouclierMax;
        }
        printf("\n[MANOEUVRE] Vous deviez l'energie aux boucliers (" COLOR_CYAN "+%d" COLOR_RESET ") !\n", regen);
    }

    SLEEP_MS(1500);

    // --- CONTRE-ATTAQUE ENNEMIE ---
    if (ennemi->coque > 0) {
        printf("\nL'ennemi riposte...");
        SLEEP_MS(800);

        if (checkEsquive(10, joueur)) {
            printf("\nESQUIVE ! Le tir ennemi vous manque.\n");
        } else {
            if (ennemi->missiles > 0 && joueur->bouclier > 0) {
                printf("\nL'ENNEMI LANCE UN MISSILE !");
                int degatsFinaux = calculerDegats(3 + (ennemi->distanceParcourue / 5), ennemi->moteurs);
                joueur->coque -= degatsFinaux;
                ennemi->missiles--;
            } else {
                int degatsFinaux = calculerDegats(ennemi->armes, ennemi->moteurs);
                if (joueur->bouclier > 0) {
                    joueur->bouclier -= degatsFinaux;
                    if (joueur->bouclier < 0) joueur->bouclier = 0;
                    printf(COLOR_CYAN"\nVotre bouclier encaisse l'attaque ! (" COLOR_RED "-%d" COLOR_CYAN")\n" COLOR_RESET, degatsFinaux);
                } else {
                    joueur->coque -= degatsFinaux;
                    printf("\nALERTE ! Votre coque est touchee (" COLOR_RED "-%d" COLOR_RESET") !\n", degatsFinaux);
                }
            }
        }
        SLEEP_MS(1500);
    }
}

bool checkEsquive(int chanceEsquive, Vaisseau *joueur) {
    int chanceTotale = chanceEsquive + (joueur->moteurs * 5); // varible ChanceEsquive de base + 5% par niveau de moteur
    if ((rand() % 100) < chanceTotale) {
        printf("ESQUIVE ! Les moteurs ont permis d'eviter le tir.\n");
        return true; // Le tir rate
    }
    return false; // Le tir touche
}

void rechargerBoucliers(Vaisseau *v) {
    if (v->bouclier < v->bouclierMax) {
        v->bouclier++; 
        printf("[SYSTEME] Bouclier regenere : %d/%d\n", v->bouclier, v->bouclierMax);
    }
}

Vaisseau genererEnnemi(int secteur) {
    Vaisseau ennemi;

    char *nomsEnnemis[] = {
        "Le Rogue One",
        "L'Etoile du Vide", 
        "Pilleur de Ferraille",
        "Nebuleuse Noire", 
        "L'Impitoyable", 
        "Vortex Sombre",
        "Chasseur de Primes", 
        "Le Fer a Repasser", 
        "Hacker Solaire",
        "Destructeur K-9", 
        "Le Faucon de Plomb", 
        "Silence Eternel",
        "Faucon Milénium",
        "Le Fer à Repasser"
    };
    int nbNoms = 14; // Nombre total de noms dans la liste
    strcpy(ennemi.nom, nomsEnnemis[rand() % nbNoms]);

    // Logique d'apparition du Capital Ship (Rare et seulement après secteur 10)
    int chanceCapital = rand() % 100;
    
    if (secteur >= 10 && chanceCapital < 25) { // 25% de chance
        strcpy(ennemi.nom, "CAPITAL SHIP REBELLE");
        printf(COLOR_RED "\n[ALERTE] SIGNATURE MASSIVE : %s détecté !" COLOR_RESET "\n", ennemi.nom);
        ennemi.coqueMax = 35 + secteur;
        ennemi.armes = 3 + (secteur / 5);
        ennemi.bouclierMax = 3;
        ennemi.missiles = 5;
    }


    int type = rand() % 3; // 3 types d'ennemis

    if (type == 0) { // Éclaireur (Rapide mais fragile)
        ennemi.coqueMax = 6 + secteur;
        ennemi.armes = 1 + (secteur / 4);
        ennemi.bouclierMax = 0 + (secteur / 5);
        ennemi.moteurs = 2 + (secteur / 5);
        printf("\n[SCAN] Contact visuel : %s", ennemi.nom);
        printf("\n[SCAN] Type : " COLOR_RED "ECLAIREUR" COLOR_RESET " (Faible coque, rapide)\n");
    } 
    else if (type == 1) { // Chasseur (Équilibré)
        ennemi.coqueMax = 10 + secteur;
        ennemi.armes = 1 + (secteur / 3);
        ennemi.bouclierMax = 1 + (secteur / 4);
        ennemi.missiles = (secteur > 3) ? 1 : 0; // Les chasseurs ont des missiles après le secteur 3
        printf("\n[SCAN] Contact visuel : %s", ennemi.nom);
        printf("\n[SCAN] Type : " COLOR_RED "CHASSEUR" COLOR_RESET " (Equilibre)\n");
    } 
    else { // Croiseur (Lourd et blindé)
        ennemi.coqueMax = 15 + secteur;
        ennemi.armes = 1 + (secteur / 5);
        ennemi.bouclierMax = 2 + (secteur / 4);
        printf("\n[SCAN] Contact visuel : %s", ennemi.nom);
        printf("\n[SCAN] Type : " COLOR_RED "CROISEUR" COLOR_RESET " (Blindage lourd)\n");
    }

    ennemi.coque = ennemi.coqueMax;
    ennemi.bouclier = ennemi.bouclierMax;
    ennemi.missiles = (secteur > 5) ? 2 : 0; // Les ennemis ont des missiles après le secteur 5
    
    return ennemi;
}

Vaisseau genererBossFinal() {
    Vaisseau boss;
    strcpy(boss.nom, "STARK ONE");
    
    boss.coqueMax = 50;
    boss.coque = 50;
    boss.bouclierMax = 4;
    boss.bouclier = 4;
    boss.armes = 4;
    boss.missiles = 10;
    
printf(COLOR_RED "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    printf("!!! " COLOR_YELLOW "ALERTE : SIGNATURE MASSIVE DETECTEE" COLOR_RED " !!!\n");
    printf("!!!      " COLOR_BOLD "LE VAISSEAU MERE EST ICI" COLOR_RESET COLOR_RED "        !!!\n");
    printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" COLOR_RESET "\n");
    
    return boss;
}

int calculerDegats(int puissanceArme, int niveauMoteur) {
    int chanceCritique = 10 + (niveauMoteur * 2); // 10% + 2% par niveau moteur
    int r = rand() % 100;

    if (r < chanceCritique) {
        printf(COLOR_YELLOW " !!! COUP CRITIQUE !!! " COLOR_RESET "\n");
        return puissanceArme * 2; // Dégâts doublés
    }

    return puissanceArme;
}