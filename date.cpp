/*  INF3105 - Structures de données et algorithmes
 *  UQAM / Département d'informatique
 *  Été 2021 / TP4
 *  http://ericbeaudry.uqam.ca/INF3105/tp4/
 */
#include "date.h"
#include <cstdio>
#include <iomanip>
#include <assert.h>

Date::Date(){
          jour = 0;
          heure = 0;
          minute = 0;
          seconde = 0;
}

bool Date::operator ==(const Date& d) const{
    if( jour == d.jour && heure == d.heure && minute == d.minute && seconde == d.seconde )
        return true;
    return false;
}
bool Date::operator <(const Date& d) const{
    bool plutTot = false;
    if( jour < d.jour ){
        return true;
    }
    if ( jour == d.jour ){
        if( heure <= d.heure ){
            plutTot = true;
            if ((heure == d.heure) && (minute >= d.minute )){
                plutTot = false;
                if ( minute == d.minute && seconde < d.seconde ){
                    plutTot = true;
                } 
            }
        }
    }
    return plutTot;
}

std::ostream& operator << (std::ostream& os, const Date& d){
    int jours, heures, minutes, secondes;
    jours = d.jour;
    heures = d.heure;
    minutes = d.minute;
    secondes = d.seconde;
    char chaine[40];
    sprintf(chaine, "%dj_%02dh%02dm%02ds", jours, heures, minutes, secondes);
    os << chaine;
    return os;
}

std::istream& operator >> (std::istream& is, Date& d){
    char chaine[40];
    int jours, heures, minutes, secondes;
    char j, m, h, s, underscore;
    is >> jours >> j >> underscore >> heures >> h >> minutes >> m >> secondes >> s;
    assert(j=='j' && underscore=='_');
    assert(h=='h' && m=='m' && s=='s');    
    d.jour = jours;
    d.heure = heures;
    d.minute = minutes;
    d.seconde = secondes;
    return is;
}

std::istream& operator >> (std::istream& is, Intervalle& intervalle){
    char crochetgauche, crochetdroit, virgule;
    is >> crochetgauche >> intervalle.debut >> virgule >> intervalle.fin >> crochetdroit;
    assert(crochetgauche=='[' && virgule==',' && crochetdroit==']');
    return is;
}

std::ostream& operator << (std::ostream& os, const Intervalle& intervalle){
    os << '[' << intervalle.debut << ',' << intervalle.fin << ']';
    return os;
}


