/*  INF3105 - Structures de données et algorithmes
 *  UQAM / Département d'informatique
 *  Été 2021 / TP4
 *  http://ericbeaudry.uqam.ca/INF3105/tp4/
 */
#include "journal.h"

void Journal::service(const Date& d, const string& nompersonne, const string& nomcocktail)
{
    servicesauxpersonnes[nompersonne][d][nomcocktail]++; 
}

map<string, int> Journal::requeteQui(const Date& debut, const Date& fin, const set<string>& nomscocktails) const {
    map<string, int> resultat;
    map<string, map<Date, map<string, int> > >::const_iterator iter1 = servicesauxpersonnes.begin();
    for(;iter1 != servicesauxpersonnes.end() ;++iter1){
            int qt = 0;
        string nompersonne = iter1->first;
        map<Date, map<string, int> >::const_iterator iter2 = iter1->second.begin();
        for(; iter2 != iter1->second.end() ; ++iter2){
            Date d = iter2->first;
            map<string, int>::const_iterator iter3 = iter2->second.begin();
            for(;iter3 != iter2->second.end() ;++iter3){
                string nomcocktail = iter3->first;
                int quantite = iter3->second;
                if(quantite > 0 && ((nomscocktails.count(nomcocktail) > 0 ) || nomscocktails.empty())){
                    if( (debut < d && d < fin) || ( d == debut || d == fin ) ) {
                    qt += quantite;
                    std::map<string, int>::iterator iter = resultat.find(nompersonne);
                    if(  iter != resultat.end() ){
                       iter->second = qt;
                    }
                    }
                }
            }
            if( qt > 0 )
                    resultat.insert({nompersonne, qt});

        }
    }
    return resultat;
}

map<string, int> Journal::requeteQuoi(const Date& debut, const Date& fin, const set<string>& nomspersonnes) const
{
    map<string, int> resultat;
    map<string, map<Date, map<string, int> > >::const_iterator iter1 = servicesauxpersonnes.begin();

    for(;iter1 != servicesauxpersonnes.end() ;++iter1){
        string nomcocktail;
        string nompersonne = iter1->first;
        map<Date, map<string, int> >::const_iterator iter2 = iter1->second.begin();
        for(; iter2 != iter1->second.end() ; ++iter2){
//            int qt = 0;
            Date d = iter2->first;
            map<string, int>::const_iterator iter3 = iter2->second.begin();
            for(;iter3 != iter2->second.end() ;++iter3){
                nomcocktail = iter3->first;
                int quantite = iter3->second;
                if(quantite > 0 && ((nomspersonnes.count(nompersonne) > 0 ) || ( nomspersonnes.empty() ))){
                    if( (debut < d && d < fin) || ( d == debut || d == fin ) ){
                        std::map<string, int>::iterator iter = resultat.find(nomcocktail);
                        if( iter != resultat.end ()){
                        iter->second += quantite;    
                        }
                        resultat.insert({ nomcocktail , quantite });
//                    resultat.insert({nomcocktail, quantite});
                    }
                }
            }

        }
    }
    return resultat;
}

ArbreAVL<string> Journal::requetePresences(const Date& debut, const Date& fin) const
{
    ArbreAVL<string> noms;
    // À compléter.
    return noms;
}

int Journal::requeteNbMaxPersonnes(const Date& debut, const Date& fin) const
{
    // À compléter.
    return 0;
}
