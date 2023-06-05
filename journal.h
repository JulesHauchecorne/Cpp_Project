/*  INF3105 - Structures de données et algorithmes
 *  UQAM / Département d'informatique
 *  Été 2021 / TP4
 *  http://ericbeaudry.uqam.ca/INF3105/tp4/
 */
#if !defined(__JOURNAL_H__)
#define __JOURNAL_H__

#include <string>
#include <iostream>
#include <map>
#include <iterator>
#include <set>
#include "arbremap.h"
#include "date.h"

using namespace std;

class Journal{
  public:
    void                  service(const Date& date, const string& nompersonne, const string& nomcocktail);
    map<string, int> requeteQui(const Date& debut, const Date& fin, const set<string>& cocktails) const;
    map<string, int> requeteQuoi(const Date& debut, const Date& fin, const set<string>& buveurs) const;
    ArbreAVL<string>      requetePresences(const Date& debut, const Date& fin) const;
    int                   requeteNbMaxPersonnes(const Date& debut, const Date& fin) const;

  private:
    map<string, map<Date, map<string, int> > >  servicesauxpersonnes;
     

};

#endif
