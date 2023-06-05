/*  INF3105 - Structures de données et algorithmes
 *  UQAM / Département d'informatique
 *  Été 2021 / TP4
 *  http://ericbeaudry.uqam.ca/INF3105/tp4/
 */
#if !defined(__DATE_H__)
#define __DATE_H__

#include <iostream>

class Date{
  public:
      Date();
    bool operator <(const Date& date) const;
    bool operator ==(const Date& date) const;
    int heure;
    int jour;
    int minute;
    double seconde;
  private:
    // À compléter.
    
  friend std::ostream& operator << (std::ostream&, const Date& date);
  friend std::istream& operator >> (std::istream&, Date& date);
};

// Il est raisonnable de laisser tous les champs debut et fin en accès public.
struct Intervalle{
    Date debut, fin;
    friend std::istream& operator >> (std::istream& is, Intervalle& itervale);
    friend std::ostream& operator << (std::ostream& os, const Intervalle& itervale);
};


#endif
