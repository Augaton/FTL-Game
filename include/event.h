#ifndef EVENEMENTS_H
#define EVENEMENTS_H

#include "vaisseau.h"

void executerEvenement(Vaisseau *joueur, const char *destination);
const char* inspecterBalise();
void evenementDeresse(Vaisseau *joueur);
void menuVoyage(Vaisseau *joueur);

#endif