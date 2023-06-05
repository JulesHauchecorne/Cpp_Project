#!/bin/bash
###############################################################################
# UQAM - Département d'informatique
# INF3105 - Structures de données et algorithmes
# Été 2021
# TP4 - Analyse de cocktails
# http://ericbeaudry.uqam.ca/INF3105/tp4/
#
# Script d'évaluation
#
# Instructions:
# 1. Déposer ce script avec les fichiers tests dans un répertoire 
#    distinct (ex: tests).
# 2. Se placer dans le répertoire contenant votre programme `tp4`.
#    Le correcteur se placera dans le répertoire contenant les soumissions.
# 3. Lancer ce script (ex: ../tests/evaluer.sh).
##########################################################################

# Obtenir le chemin du répertoire contenant le présent script et les fichiers tests
pushd `dirname $0` > /dev/null
repertoire_tests=`pwd`
# Lister les fichier test disponibles
tests=`ls exemple.txt test??.txt`
popd  > /dev/null

echo "UQAM | Département d'informatique"
echo "INF3105 | Structures de données et algorithmes"
echo "Évaluation du TP4"
echo

if [ `pwd` -ef $repertoire_tests ];
then
    echo "Ce script doit être dans un répertoire différent de celui contenant votre tp4."
    echo "Ce script a été arrêté afin de ne pas écraser les fichiers testXX+.txt!"
    exit -2;
fi

########### Détection du valideur de résultats #######
# Valideur : g++ -o valideur valideur.cpp
valideur="${repertoire_tests}/valideur"

# Détection si l'utilitaire time sous Linux est disponible (peut afficher 0.00)
echo "Détection de /usr/bin/time..."
/usr/bin/time -f %U echo 2>&1 > /dev/null
souslinux=$?

# Fixer les limites : 180s, 2048Mo mémoire et 20Mo fichier
ulimit -t 180 -v 2097152 -f 20480
echo "ulimit (limites courantes des ressources) :"
ulimit -t -v -f
echo "-----"

# Détection du CPU
if [ -e /proc/cpuinfo ] ; then
    cpuinfo=`grep "model name" /proc/cpuinfo | sort -u | cut -d ":" -f 2`
else
    cpuinfo="?"
fi

function Nettoyer
{
    echo "Nettoyage"
    # Au cas où le Makefile des étudiants ne ferait pas un «make clean» correctement.
    #make clean

    rm -f *.o* *.gch tp[1-5] *+.txt
    # Au besoin, nettoyer les précédents fichiers logs
    rm -f log*.txt
}


##### Fonction d'évaluation d'un TP ######
function EvaluerTP
{
    date2=`date +%Y%m%d_%H%M%S`
    #logfile="log-${date2}.txt"
    logfile="log.txt"
    echo "Les résultats seront déposés dans $logfile..."
    echo "Date: $date2" > $logfile
       
    #datesoumis=`grep Date lisezmoi.txt| cut -c 7-`
    #coequipier=`grep Coéquipier lisezmoi.txt| cut -d : -f 2`
    #heures=`grep Heures lisezmoi.txt| cut -d : -f 2`
    #autoeval=`grep Auto-Évaluation lisezmoi.txt| cut -d : -f 2`

    ## ZIP
    #if [ -f sources.zip ]; then
    #    echo "Unzip: sources.zip"
    #    unzip -n sources.zip
    #    rm sources.zip
    #fi
    
    echo "Fichiers:" > $logfile
    ls -l 2>&1 | tee -a $logfile

    ## Forcer la recompilation.
    #rm tp4 *.o *.gch
    #make clean
    
    logfile="log-${date}.txt"
	echo "Les résultats sont déposés dans $logfile."

#    echo "#Machine : " `hostname`  | tee -a $logfile
    echo "#CPU :$cpuinfo"  | tee -a $logfile
    echo "#Date début : ${date2}"  | tee -a $logfile
    echo "#Limite de `ulimit -t` secondes par test"  | tee -a $logfile
    echo | tee -a $logfile

    # Pour statistiques : nombre de lignes de code...
    echo "Taille des fichiers source :" | tee -a ${logfile}
    wc *.{c*,h*}  | tee -a ${logfile}

    taille=`wc *.{c*,h*} | grep total`
    sommaire="$taille"
    sommaire="$datesoumis\t$coequipier\t$autoeval\t$heures\t$taille\t"
    
    #echo "Compilation ..." | tee -a $logfile 
    #make tp4 >> $logfile 2>&1
    
    # Vérification de l'existance du programme.
    if [ ! -e tp4 ]
        then
        echo "  Erreur : le fichier tp4 est inexistant!" | tee -a $logfile 
        return
    fi
    if [ ! -x tp4 ]
        then
        echo "  Erreur : le fichier tp4 n'est pas exécutable!" | tee -a $logfile 
        return
    fi

    echo | tee -a $logfile

    echo -e "Fichier test\tTotal\tMém.(k)\tService\tQui\tQuoi\tPres.\tMax\tService\tQui\tQuoi\tPres.\tMax" | tee -a $logfile

    for test in $tests;
    do
      fichier=$repertoire_tests/$test

      if [ $souslinux -eq 0 ]; then
       # Linux
       tG="`                                         (/usr/bin/time -f "%U\t%Mk" ./tp4 < $fichier > ${test%.txt}+.txt) 2>&1 | tail -n 1`"
       tS=`   egrep "^service"            $fichier | (/usr/bin/time -f "%U" ./tp4 > /dev/null) 2>&1 | tail -n 1`
       tQui=` egrep "^service|^qui"       $fichier | (/usr/bin/time -f "%U" ./tp4 > /dev/null) 2>&1 | tail -n 1`
       tQuoi=`egrep "^service|^quoi"       $fichier | (/usr/bin/time -f "%U" ./tp4 > /dev/null) 2>&1 | tail -n 1`
       tPres=`egrep "^service|^presences" $fichier | (/usr/bin/time -f "%U" ./tp4 > /dev/null) 2>&1 | tail -n 1`
       tMax=` egrep "^service|^max"       $fichier | (/usr/bin/time -f "%U" ./tp4 > /dev/null) 2>&1 | tail -n 1`
      else
       # macOS
       tG=`                                          (time -p ./tp4 < $fichier > ${test%.txt}+.txt) 2>&1 | egrep user | cut -f 2 -d " "`
       tS=`   egrep "^service"            $fichier | (time -p ./tp4 > /dev/null) 2>&1 | egrep user | cut -f 2 -d " "`
       tQui=` egrep "^service|^qui"       $fichier | (time -p ./tp4 > /dev/null) 2>&1 | egrep user | cut -f 2 -d " "`
       tQuoi=`egrep "^service|^quoi"       $fichier | (time -p ./tp4 > /dev/null) 2>&1 | egrep user | cut -f 2 -d " "`
       tPres=`egrep "^service|^presences" $fichier | (time -p ./tp4 > /dev/null) 2>&1 | egrep user | cut -f 2 -d " "`
       tMax=` egrep "^service|^max"       $fichier | (time -p ./tp4 > /dev/null) 2>&1 | egrep user | cut -f 2 -d " "`
      fi
      
      if ( [ -x ${valideur} ] && [ -e "${fichier%.txt}+.txt" ] )
      then
          verif=`$valideur -q $fichier "${fichier%.txt}+.txt" ${test%.txt}+.txt`
      else
          verif=""
      fi

      echo -e "$test\t$tG\t$tS\t$tQui\t$tQuoi\t$tPres\t$tMax\t$verif" | tee -a $logfile
      sommaire="${sommaire}\t$tG\t$tS\t$tQui\t$tQuoi\t$tPres\t$tMax\t$verif\t"
     
    done
}


if [ -f Makefile ];
then
tps="."
EvaluerTP

else
# Lister les répertoires
tps=`ls -1`
tps=`for x in $tps; do if [ -d $x ] ; then echo $x; fi; done`

date=`date +%Y%m%d_%H%M%S`
echo "#Rapport de correction INF3105 / TP4" > "rapport-${date}.txt"
echo -e "#Date:\t${date}" >> "rapport-${date}.txt"
echo -e "#Machine :\t" `hostname` >> "rapport-${date}.txt"
echo -e "#CPU :\t$cpuinfo" >> "rapport-${date}.txt"
echo >> "rapport-${date}.txt"

    echo -e -n "\t\t" >> "rapport-${date}.txt"
    for test in ${tests}; do
       echo -e -n "\t${test}\t\t\t\t\t\t\t\t\t\t\t\t" >> "rapport-${date}.txt"
    done
    echo >> "rapport-${date}.txt"
    echo -e -n "Soumission\tTaille sources\t" >> "rapport-${date}.txt"
    for test in ${tests}; do
       echo -e -n "\tTotal\tMém.(k)\tService\tQui\tQuoi\tPres.\tMax\tService\tQui\tQuoi\tPres.\tMax\t" >> "rapport-${date}.txt"
    done
    echo >> "rapport-${date}.txt"
    echo 

    for tp in $tps;
    do
        sommaire=""
        echo "------------------------------------------"
        echo "Correction de: $tp ..."

        # changer de répertoire pour celui de la soumission
        pushd $tp

        EvaluerTP

        # revenir dans le répertoire précédent.
        echo
        popd

        # Ajout d'une ligne au rapport...
        echo -e "${tp}\t${sommaire}" >> "rapport-${date}.txt"
    done


fi


