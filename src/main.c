#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "utils.h"
#include "vaisseau.h"
#include "combat.h"
#include "event.h"

// --- MAIN ---
int main() {
    srand(time(NULL));

    Vaisseau joueur = {
        "",  // nom
        20,  // coque
        20,  // coqueMax
        1,   // bouclier (actuel)
        1,   // bouclierMax
        2,   // armes
        3,   // missiles
        20,  // ferraille
        10,  // carburant
        0,    // distanceParcourue
        0   // moteurs
    };

    printf("--- INITIALISATION DU SYSTEME ---\n");
    printf("Commandant, comment se nomme votre vaisseau ? : ");

    fgets(joueur.nom, sizeof(joueur.nom), stdin);
    joueur.nom[strcspn(joueur.nom, "\n")] = 0;

    while (joueur.coque > 0) {
        menuVoyage(&joueur); 
    }

    if (joueur.coque <= 0) {
        afficherGameOver(&joueur);
    } else {
        afficherVictoire(&joueur);
    }

    printf("\n--- FIN DE LA PARTIE ---\n");
    return 0;
}