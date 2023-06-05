/* Squelette pour classe générique ArbreMap<K, V>.
 * Lab8 -- Dictionnaires et Arbres binaires de recherche
 * UQAM / Département d'informatique
 * INF3105 - Structures de données et algorithmes
 * http://ericbeaudry.uqam.ca/INF3105/lab8/
 * Jules Hauchecorne 
 * HAUJ21049307
 *
 */
#if !defined(_ARBREMAP___H_)
#define _ARBREMAP___H_
#include "arbreavl.h"

template <class K, class V>
class ArbreMap {

  public:
    bool contient(const K&) const;
    void enlever(const K&);
    void vider();
    bool vide() const;

    const V& operator[] (const K&) const;
    V& operator[] (const K&);
//
    class IterateurConst;
//    
//    // Fonctions pour obtenir un itérateur (position dans l'arbre)    
    const IterateurConst debut() const;
//    IterateurConst fin() const;
//    IterateurConst rechercher(const K&) const;
//    IterateurConst rechercherEgalOuSuivant(const K&) const;
//    IterateurConst rechercherSuivant(const K&) const;
//    IterateurConst rechercherEgalOuPrecedent(const K&) const;
//
//    // Accès aux éléments de l'arbre via un intérateur.
//    const K& operator[](const IterateurConst&) const;
 //   K& operator[](const IterateurConst&);
//

  private:
    class Entree{
        public:
        Entree(const K& c):cle(c),valeur(){}
        K cle;
        V valeur;
         bool operator < (const Entree& e) const { return cle < e.cle; }
         bool operator == (const Entree& e) const { return cle == e.cle; }
     };

    ArbreAVL<Entree> entrees;
};

template <class K, class V>
void ArbreMap<K,V>::vider(){
     entrees.vider();
}

template <class K, class V>
bool ArbreMap<K,V>::vide() const{
     return entrees.vide();
}

template <class K, class V>
void ArbreMap<K,V>::enlever(const K& c)
{
    entrees.enlever(c);
}

template <class K, class V>
bool ArbreMap<K,V>::contient(const K& c) const
{
return entrees.contient( c );
}

template <class K, class V>
const V& ArbreMap<K,V>::operator[] (const K& c)  const
{
    typename ArbreAVL<Entree>::Iterateur iter=entrees.rechercher(c);
    return entrees[iter].valeur;
}

template <class K, class V>
V& ArbreMap<K,V>::operator[] (const K& c) 
{
    typename ArbreAVL<Entree>::Iterateur iter=entrees.rechercher(Entree(c));
    if( !iter ){
        entrees.inserer(Entree(c));
        iter = entrees.rechercher(c);
    }
    return entrees[iter].valeur;
}
template <class K, class V>
const typename ArbreMap<K,V>::IterateurConst ArbreMap<K,V>::debut() const
{
    IterateurConst iter(*this);
    iter.courant = entrees.racine;
    if( iter.courant == nullptr ) { return iter; }
    return min( entrees.racine, iter );
}
#endif

