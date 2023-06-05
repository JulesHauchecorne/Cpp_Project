/* Squelette minimal pour classe générique ArbreAVL<T> du TP3.
 * TP3 -- Arbres AVL
 * UQAM / Département d'informatique
 * INF3105 - Structures de données et algorithmes
 * http://ericbeaudry.uqam.ca/INF3105/tp3/
 * Jules Hauchecorne
 * HAUJ21049307
 *
 * Suggestions:
 */
#if !defined(_ARBREAVL___H_)
#define _ARBREAVL___H_
#include <cassert>
#include <iostream>
// ------ Début bloc 1 inséré pour Lab7 ------
#include "pile.h"
using namespace std;
// ------ Fin bloc 1 inséré pour Lab7 ------

template <class T>
class ArbreAVL {
  public:
    ArbreAVL();
    ~ArbreAVL();

    bool vide() const;
    bool contient(const T&) const;
    void inserer(const T&);
    void vider();

    void enlever(const T&);

    ArbreAVL(const ArbreAVL&);

    int taille() const;
    const T&        get_ieme(int i) const; // retourne le i-ème élément
    ArbreAVL<T>&    operator = (const ArbreAVL<T>& autre);
    bool            operator == (const ArbreAVL<T>& autre) const;
    
    ArbreAVL<T>&    operator += (const ArbreAVL<T>& autre); // ajoute tous les éléments dans autre
    ArbreAVL<T>     operator +  (const ArbreAVL<T>& autre) const; // retourne l'union
    ArbreAVL<T>&    operator -= (const ArbreAVL<T>& autre); // retire tous les éléments dans autre
    ArbreAVL<T>     operator -  (const ArbreAVL<T>& autre) const; // retourne la différence
    int hauteur() const;
    void afficher() const;

    class Iterateur;
    
    // Fonctions pour obtenir un itérateur (position dans l'arbre)    
    Iterateur debut() const;
    Iterateur fin() const;
    Iterateur rechercher(const T&) const;
    Iterateur rechercherEgalOuSuivant(const T&) const;
    Iterateur rechercherSuivant(const T&) const;
    Iterateur rechercherEgalOuPrecedent(const T&) const;

    // Accès aux éléments de l'arbre via un intérateur.
    const T& operator[](const Iterateur&) const;
    T& operator[](const Iterateur&);

  private:
    struct Noeud{
        Noeud(const T&);
        T contenu;
        int equilibre;
        Noeud *gauche,
              *droite;
        bool enleve;
    };
    Noeud* racine;

    // Fonctions internes
    bool inserer(Noeud*&, const T&);
    const T* recherche( Noeud*, const T&) const;
    void rotationGaucheDroite(Noeud*&);
    void rotationDroiteGauche(Noeud*&);
    void vider(Noeud*&);
    const T* enlever(Noeud*, const T&);

    void copier(const Noeud*, Noeud*&);

    // Fonctions internes pour certains tests
    int hauteur(const Noeud*) const;
    int compter(const Noeud*) const;
    void preparerafficher(const Noeud* n, int profondeur, int& rang, T* elements, int* profondeurs) const;

    Iterateur min( Noeud* , Iterateur ) const;
  public:
    class Iterateur{
      public:
        Iterateur(const ArbreAVL& a);
        Iterateur(const Iterateur& a);
        Iterateur(const ArbreAVL& a, Noeud* c);

        operator bool() const;
        bool operator!() const;
        bool operator==(const Iterateur&) const;
        bool operator!=(const Iterateur&) const;
        
        const T& operator*() const;

        Iterateur& operator++();
        Iterateur operator++(int);
        Iterateur& operator = (const Iterateur&);
      private:
        const ArbreAVL& arbre_associe;
        Noeud* courant;
        Pile<Noeud*> chemin;
        
      friend class ArbreAVL;
    };
};


//-----------------------------------------------------------------------------

template <class T>
ArbreAVL<T>::Noeud::Noeud(const T& c)
 : contenu(c), equilibre(0), gauche(nullptr), droite(nullptr) , enleve(false)
{
}

template <class T>
ArbreAVL<T>::ArbreAVL() 
 : racine(nullptr)
{
}

template <class T>
ArbreAVL<T>::ArbreAVL(const ArbreAVL<T>& autre)
 : racine(nullptr)
{
    copier(autre.racine, racine);
}

template <class T>
ArbreAVL<T>::~ArbreAVL()
{
    vider(racine);
}

template <class T> 
const T* ArbreAVL<T>::recherche(Noeud* noeud, const T& element) const{
if( noeud == nullptr ) return nullptr;
if( element == noeud->contenu ){
    if( noeud->enleve ){ return nullptr;} else  {  return &(noeud->contenu) ; } 
}
if( element < noeud->contenu ){ return recherche(noeud->gauche, element); }
else { return recherche( noeud->droite, element ); }
}

template <class T>
bool ArbreAVL<T>::contient(const T& element) const
{
    return recherche(racine, element);
}

template <class T>
void ArbreAVL<T>::inserer(const T& element)
{
    inserer(racine, element);
}

template <class T>
// retourne true si l'arbre gagne en hauteur après l'insertion
bool ArbreAVL<T>::inserer(Noeud*& noeud, const T& element)
{
    if(noeud==nullptr)
    {
        noeud = new Noeud(element);
        return true;
    }
    if( (element == noeud->contenu) && noeud->enleve ){ 
        noeud->enleve = false; 
        return false;
        }
    if(element < noeud->contenu){
        if(inserer(noeud->gauche, element))
        {
            noeud->equilibre++;
            if(noeud->equilibre == 0)
                return false;
            if(noeud->equilibre == 1)
                return true;
            assert(noeud->equilibre==2);
            if(noeud->gauche->equilibre == -1){
                rotationDroiteGauche(noeud->gauche);
            }
            rotationGaucheDroite(noeud);
        }
        return false;
    }
    else if(noeud->contenu < element){
        if( inserer(noeud->droite, element) ){
            noeud->equilibre--;
            if( noeud->equilibre == 0 ){ return false; }
            if( noeud->equilibre == -1 ){ return true; }
            assert(noeud->equilibre== -2);
            if( noeud->droite->equilibre == 1 ){
                rotationGaucheDroite(noeud->droite);
            }
            rotationDroiteGauche(noeud);
        }
        return false;
    }

    noeud->contenu = element;  // Mise à jour
    return false;
}

template <class T>
void ArbreAVL<T>::rotationGaucheDroite(Noeud*& racinesousarbre)
{
    Noeud *temp = racinesousarbre->gauche;
    int  ea = temp->equilibre;
    int  eb = racinesousarbre->equilibre;
    int  neb = -(ea>0 ? ea : 0) - 1 + eb;
    int  nea = ea + (neb < 0 ? neb : 0) - 1;

    temp->equilibre = nea;
    racinesousarbre->equilibre = neb;
    racinesousarbre->gauche = temp->droite;
    temp->droite = racinesousarbre;
    racinesousarbre = temp;
}

template <class T>
void ArbreAVL<T>::rotationDroiteGauche(Noeud*& racinesousarbre)
{
    Noeud *temp = racinesousarbre->droite;
    int eb = temp->equilibre;
    int ea = racinesousarbre->equilibre;
    // nouveau équillibre
    int nea = -(eb<0 ? eb : 0) + 1 + ea;
    int neb =  (nea > 0 ? nea : 0) + 1 + eb;

    temp->equilibre = neb;
    racinesousarbre->equilibre = nea;
    racinesousarbre->droite = temp->gauche;
    temp->gauche = racinesousarbre;
    racinesousarbre = temp;
}

template <class T>
bool ArbreAVL<T>::vide() const
{
    if( racine == nullptr || racine->enleve ){ return true; } else {
        return false;
    }
}

template <class T>
void ArbreAVL<T>::vider(){
  vider(racine);
}

template <class T>
void ArbreAVL<T>::vider(Noeud*& noeud)
{
    if( noeud != nullptr){ 
        vider( noeud->gauche );
        vider ( noeud->droite );
       delete( noeud->gauche );
       delete( noeud->droite );
       noeud->equilibre = 0;
       noeud->droite = nullptr;
       noeud->gauche = nullptr;
       if( noeud == racine ){ noeud = nullptr; }
    } 
}

template <class T>
void ArbreAVL<T>::copier(const Noeud* source, Noeud*& noeud) 
{
    if( source != nullptr  ){
        inserer( noeud, source->contenu );
        noeud->equilibre = source->equilibre;
        noeud->enleve = source->enleve;
        if( source->droite != nullptr ){
            copier( source->droite, noeud->droite );
        }
        if( source->gauche != nullptr ){
            copier( source->gauche, noeud->gauche );
        }
    } else {
        noeud = nullptr;
    }
}

template <class T>
int  ArbreAVL<T>::hauteur() const{
    return hauteur(racine);
}

template <class T>
int  ArbreAVL<T>::taille() const{
    return compter(racine);
}

template <class T> // retourne le i-ème élément
const T& ArbreAVL<T>::get_ieme(int i) const{
    Iterateur iter = debut();
    if( taille() == 0 ){
        assert( iter.courant->enleve != true );
    }
    for ( int a = 0; a < i ; a++){
        if( iter.courant->enleve ){i++ ; }
        iter++;
    }
    while( iter.courant->enleve){ iter++; }
    return iter.courant->contenu;
}

template <class T>
int  ArbreAVL<T>::hauteur(const Noeud* n) const{
    if(n==nullptr)
        return 0;
    return 1 + max(hauteur(n->gauche), hauteur(n->droite));
}

template <class T>
int ArbreAVL<T>::compter(const Noeud* n) const{
    if(n==nullptr) return 0;
    if(n->enleve) return compter( n->gauche ) + compter(n->droite);
    return 1 + compter(n->gauche) + compter(n->droite);
}

template <class T>
ArbreAVL<T>& ArbreAVL<T>::operator=(const ArbreAVL& autre) {
    if(this==&autre) return *this;
    vider();
    copier(autre.racine, racine);
    return *this;
}

template <class T>
bool ArbreAVL<T>::operator == (const ArbreAVL<T>& autre) const{
   Iterateur iter(*this);
   Iterateur iterAutre(autre);
    iter = debut();
    iterAutre = autre.debut();
    if ( !iterAutre.courant ){ return false; }
    if (iter.courant->contenu == iterAutre.courant->contenu  && (taille() == autre.taille() )){
        while( iter++ ){
            if (iter.courant->contenu != iterAutre.courant->contenu  ){ 
                return false;
            }
               iter++;
               iterAutre++;
        }     
        return true;
   }
    return false;
}
    
template <class T> // ajoute tous les éléments dans autre
ArbreAVL<T>& ArbreAVL<T>::operator += (const ArbreAVL<T>& autre){
    Iterateur iterAutre(autre);
    iterAutre = autre.debut();
    assert( iterAutre.courant );
    for ( int i = 0 ; i < autre.taille(); i++ ){
        inserer(iterAutre.courant->contenu);
        iterAutre++;
    }        
    return *this;
}

template <class T>  // retourne l'union
ArbreAVL<T>  ArbreAVL<T>::operator +  (const ArbreAVL<T>& autre) const{
    ArbreAVL<T> temp = *this;
        temp += autre;
    return temp;
}
template <class T> // retire tous les éléments dans autre
ArbreAVL<T>& ArbreAVL<T>::operator -= (const ArbreAVL<T>& autre){
    Iterateur iterAutre(autre);
    iterAutre = autre.debut();
    assert( iterAutre.courant );
    for ( int i = 0 ; i < autre.taille(); i++ ){
        if( contient( iterAutre.courant->contenu ) ){
            enlever(iterAutre.courant->contenu  );
        }
        iterAutre++;
    }        
    return *this;
}

template <class T> // retourne la différence
ArbreAVL<T> ArbreAVL<T>::operator -  (const ArbreAVL<T>& autre) const{
    ArbreAVL<T> temp = *this;
        temp -= autre;
    return temp;
}

template <class T>
void ArbreAVL<T>::enlever(const T& element)
{
    enlever( racine , element );
}

template <class T>
const T* ArbreAVL<T>::enlever(Noeud *noeud, const T& element)
{
    if ( noeud == nullptr ){ return nullptr; }
    if( noeud->enleve && taille() == 0 ){ return nullptr; }
    if ( element==noeud->contenu ){
        noeud->enleve = true;
        return &(noeud->contenu);
    }
    if ( element < noeud->contenu ){
        return enlever(noeud->gauche, element);
    } else {
        return enlever(noeud->droite, element);
    }
}
//-----------------------------

// Code fourni pour afficher l'arbre :
#include <iostream>
template <class T>
void ArbreAVL<T>::afficher() const{
    std::cout << "Contenu de l'arbre (";
    int n = taille();
    std::cout << n << " noeuds)\n";
    T* elements = new T[n];
    int* profondeurs = new int[n];
    n=0;
    preparerafficher(racine, 0, n, elements, profondeurs);
    for(int p=0;;p++){
        bool derniereprofondeur = true;
        for(int i=0;i<n;i++){
            if(profondeurs[i]==p){
                std::cout << elements[i];
                derniereprofondeur = false;
            }
            std::cout << '\t';
        }
        std::cout << '\n';
        if(derniereprofondeur) break;
    }
    delete[] profondeurs;
    delete[] elements;
    std::cout << "-------------" << std::endl;
}

template <class T>
void ArbreAVL<T>::preparerafficher(const Noeud* n, int profondeur, int& rang, T* elements, int* profondeurs) const{
    if(n==nullptr) return;
    preparerafficher(n->gauche, profondeur+1, rang, elements, profondeurs);
    elements[rang] = n->contenu;
    profondeurs[rang] = profondeur;
    rang++;
    preparerafficher(n->droite, profondeur+1, rang, elements, profondeurs);
}


template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::debut() const
{
    Iterateur iter(*this);
    iter.courant = racine;
    if( iter.courant == nullptr ) { return iter; }
    return min( racine, iter );
}
template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::min(Noeud * n, Iterateur iter) const{
assert(n);
while( n->gauche ){
    iter.chemin.empiler(n);
    n = n->gauche;
}
iter.courant = n;
return iter;

}

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::fin() const
{
    return Iterateur(*this);
}

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::rechercher(const T& e) const
{
    Iterateur iter(*this);
    Noeud * n = racine;
    while( n ){
        if( e < n->contenu ){
            iter.chemin.empiler(n);
            n = n->gauche;
        } else if ( n->contenu < e ){
            n = n->droite;
        } else {
            iter.courant = n;
            return iter;
        }
    }
    iter.chemin.vider();
    return iter;
}

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::rechercherSuivant(const T& e) const
{
    Iterateur iter(*this);
    Noeud* n = racine, *suivant= nullptr;
    while( n ){
        if( e <= n->contenu ){
            suivant = n;
            n = n->gauche;
        } else if( n->contenu < e ){
            n = n->droite;
        }
    }
        if( suivant != nullptr ){
            return rechercher( suivant->contenu );
        }
    return iter;
}
template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::rechercherEgalOuSuivant(const T& e) const
{
    Iterateur iter(*this);
    Noeud* n = racine, *suivant= nullptr;
    while( n ){
        if( e < n->contenu ){
            suivant = n;
            n = n->gauche;
        } else if( n->contenu < e ){
            n = n->droite;
        } else {
            return rechercher(e);
        }
    }
        if( suivant != nullptr ){
            return rechercher( suivant->contenu );
        }
    return iter;
}

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::rechercherEgalOuPrecedent(const T& e) const
{
    Noeud* n = racine, *dernier = nullptr;
    while( n ){
        if( e < n->contenu ){
            n = n->gauche;
        } else if ( n->contenu < e ){
            dernier = n;
            n = n->droite;
        } else {
            return rechercher(e);
        }
    } 
    if( dernier != nullptr){
        return rechercher( dernier->contenu );
    }
    return Iterateur(*this);
}

template <class T>
const T& ArbreAVL<T>::operator[](const Iterateur& iterateur) const
{
    assert(&iterateur.arbre_associe == this);
    assert(iterateur.courant);
    return iterateur.courant->contenu;
}

template <class T>
T& ArbreAVL<T>::operator[](const Iterateur& iterateur)
{
    assert(&iterateur.arbre_associe == this);
    assert(iterateur.courant);
    return iterateur.courant->contenu;
}


//-----------------------
template <class T>
ArbreAVL<T>::Iterateur::Iterateur(const ArbreAVL& a)
 : arbre_associe(a), courant(nullptr)
{
}

template <class T>
ArbreAVL<T>::Iterateur::Iterateur(const ArbreAVL<T>::Iterateur& a)
: arbre_associe(a.arbre_associe)
{
    courant = a.courant;
    chemin = a.chemin;
}

// Pré-incrément
template <class T>
typename ArbreAVL<T>::Iterateur& ArbreAVL<T>::Iterateur::operator++()
{
    assert(courant);    
    Noeud* suivant = courant->droite;
    while(suivant){
        chemin.empiler(suivant);
        suivant = suivant->gauche;
    }
    if( !chemin.vide() ){
        courant = chemin.depiler();
    } else {
        courant = nullptr;
    }
    return *this;
}

// Post-incrément
template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::Iterateur::operator++(int)
{
    Iterateur copie(*this);
    operator++();
    return copie;
}

template <class T>
ArbreAVL<T>::Iterateur::operator bool() const
{
    return courant!=nullptr;
}

template <class T>
bool ArbreAVL<T>::Iterateur::operator!() const{
    return courant==nullptr;
}

template <class T>
bool ArbreAVL<T>::Iterateur::operator==(const Iterateur& o) const{
    assert(&arbre_associe==&o.arbre_associe);
    return courant==o.courant;
}

template <class T>
bool ArbreAVL<T>::Iterateur::operator!=(const Iterateur& o) const{
    assert(&arbre_associe==&o.arbre_associe);
    return courant!=o.courant;
}

template <class T>
const T& ArbreAVL<T>::Iterateur::operator *() const
{
    assert(courant!=nullptr);
    return courant->contenu;
}

template <class T>
typename ArbreAVL<T>::Iterateur& ArbreAVL<T>::Iterateur::operator = (const Iterateur& autre){
    assert(&arbre_associe==&autre.arbre_associe);
    courant = autre.courant;
    chemin = autre.chemin;
    return *this;
}

#endif
