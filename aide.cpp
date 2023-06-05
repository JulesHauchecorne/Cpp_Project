// INF3105 - Structures de données et algorithmes
// Aide pour le TP4
// ===============================================


/* Option 1 : Tout mettre dans des maps */
#include <iostream>
#include "date.h"
#include "arbremap.h"
#include "journal.h"

using namespace std;
// Représentation:
class Journal{
// servicesauxpersonnes[nompersonne][date][nomcocktail] ==> nombre
ArbreMap<string, ArbreMap<Date, ArbreMap<string, int> > >  servicesauxpersonnes;
...
};

// Exemple d'insertion (à adapter pour enregistrerService
servicesauxpersonnes["Eric"][Date(0, 17, 30, 0)]["Amarante"]++;


// Exemple de parcours (à adapter pour requeteQuoi et requeteQui) :
// Il faut considérer des IterateurConst plutôt que Iterateur si la fonction est const
ArbreMap<string, ArbreMap<Date, ArbreMap<string, int> > >::IterateurConst iter1=servicesauxpersonnes.debut();
for(;iter1;++iter1){
    string nompersonne = iter1.cle();
    ArbreMap<Date, ArbreMap<string, int> >::IterateurConst iter2 = iter1.valeur.debut();
    for(;iter2;++iter2){
        Date d = iter2.cle();
        ArbreMap<string, int>::IterateurConst iter3 = iter2.valeur.debut();
        for(;iter3;++iter3){
            string nomcocktail = iter3.cle();
            int quantite = iter3.valeur();
        }
    }
    
}

// Pour répondre efficacement aux différentes requêtes, la redondance peut être nécessaire
// servicesauxpersonnes[nomcocktail][date][nompersonne] ==> nombre
ArbreMap<string, ArbreMap<Date, ArbreMap<string, int> > >  servicesdecocktails;

// Insertion automatique
servicesauxpersonnes["Eric"][Date(0, 17, 30, 0)]["Amarante"]++;
servicesdecocktails["Amarante"][Date(0, 17, 30, 0)]["Eric"]++;





/* Option 2 : classe Quantites */
class Quantites : public ArbreMap<string, int> {
    Quantites& operator += (const Quantites& autre);
    Quantites operator + (const Quantites& autre) const;
};

class Journal{
  struct Personne{
    ArbreMap<Date, Quantites> quantites;
  };
  struct Cocktail{
    ArbreMap<Date, Quantites> quantites;
  };
  ArbreMap<string, Personne> personnes;
  ArbreMap<string, Cocktail> cocktails;
  ...
};

// Exemple d'insertion
personnes["Eric"].quantites[Date(0, 17, 30, 0)]["Amarante"]++;
cocktails["Amarante"].quantites[Date(0, 17, 30, 0)]["Eric"]++;

// Exemple de requête (à adapter pour requêtes)
Quantites resultat;
const Personne& p = personnes["Eric"];
ArbreMap<Date, Quantite>::IterateurConst iter = p.quantite.rechercherEgalOuSuivant(debut);
while(iter && iter.cle() <= fin){
  resultat += iter.valeur();
  ++iter;
}
return resultat;



