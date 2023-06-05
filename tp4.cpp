/*  INF3105 - Structures de données et algorithmes
 *  UQAM / Département d'informatique
 *  Été 2021 / TP4
 *  http://ericbeaudry.uqam.ca/INF3105/tp4/
 *  Jules Hauchecorne
 *  HAUJ21049307
 */
#include <fstream>
#include <iostream>
#include "journal.h"

using namespace std;

int tp4(istream& entree);

int main(int argc, const char** argv){
    if(argc>1){
        // Lecture depuis un fichier.
        std::ifstream entreefichier(argv[1]);
        if(entreefichier.fail()){
            cerr << "Erreur d'ouverture du fichier '" << argv[1] << "'!" << std::endl;
            return 1;
        }
        return tp4(entreefichier);
    }else{
        // Lecture depuis stdin (console).
        return tp4(cin);
    }
}

int tp4(istream& entree)
{
    Journal journal;
    
    while(entree){
        string cmd;
        entree >> cmd;
        if(cmd=="") break;
        
        if(cmd=="service"){
            Date instant;
            char deuxpoints, pointvirgule;
            string nompersonne;
            entree >> instant >> nompersonne >> deuxpoints;
            while(entree){
                string nomcocktail;
                entree >> nomcocktail;
                if(nomcocktail == ";") break;
                journal.service(instant, nompersonne, nomcocktail);
            }
            cout << "OK" << endl;
        }
        
        else if(cmd=="qui"){
            Intervalle intervalle;
            set<string> nomscocktails;
            entree >> intervalle;
            while(entree){
                string nomcocktail;
                entree >> nomcocktail;
                if(nomcocktail == ";") break;
                nomscocktails.insert(nomcocktail);
            }
            map<string, int> rapport = journal.requeteQui(intervalle.debut, intervalle.fin, nomscocktails);
            if(!rapport.empty()){
                
                for(map<string, int>::iterator irapport=rapport.begin();irapport != rapport.end();++irapport)
                    cout << irapport->first << " (" << irapport->second << ") ";
                
            }else
                cout << "--";
            cout << endl;
        }
        
        else if(cmd=="quoi"){
            Intervalle intervalle;
            set<string> nomspersonnes;
            entree >> intervalle;
            while(entree){
                string nompersonne;
                entree >> nompersonne;
                if(nompersonne == ";") break;
                nomspersonnes.insert(nompersonne);
            }
            map<string, int> rapport = journal.requeteQuoi(intervalle.debut, intervalle.fin, nomspersonnes);
            if(!rapport.empty()){
                
                for(map<string, int>::iterator irapport=rapport.begin();irapport != rapport.end() ;++irapport)
                    cout << irapport->first << " (" << irapport->second << ") ";
                
            }else
                cout << "--";
            cout << endl;
        }
        
        else if(cmd=="presences"){
            Intervalle intervalle;
            char pointvirgule;
            entree >> intervalle >> pointvirgule;
            ArbreAVL<string> listenoms = journal.requetePresences(intervalle.debut, intervalle.fin);
            if(!listenoms.vide()){
                for(ArbreAVL<string>::Iterateur i=listenoms.debut();i;++i)
                    cout << *i << " ";
            }else
                cout << "--";
            cout << endl;
        }
        
        else if(cmd=="max"){
            Intervalle intervalle;
            char pointvirgule;
            entree >> intervalle >> pointvirgule;
            int nbmax = journal.requeteNbMaxPersonnes(intervalle.debut, intervalle.fin);
            cout << nbmax << endl;
        }
        
        else{
            cout << "Commande invalidate : " << cmd << endl;
            return 1;
        }
    }
    
    return 0;
}
