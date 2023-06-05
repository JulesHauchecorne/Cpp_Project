/*  INF3105 - Structures de données et algorithmes
 *  UQAM / Département d'informatique
 *  Été 2021 / TP4
 *  Programme pour tester votre classe Date
 */

#include "date.h"
#include <sstream>
#include <vector>

int main(){
    std::cout << "Vérification de votre classe Date" << std::endl;
    std::stringstream str("0j_00h00m00s 0j_00h00m01s 0j_00h00m30s 0j_00h00m59s 0j_00h01m00s 0j_00h01m01s 0j_00h01m10s 0j_00h01m59s 0j_00h02m00s 0j_00h02m01s 0j_00h02m10s 0j_00h02m25s 0j_00h40m00s 0j_00h40m05s 0j_00h40m50s 0j_00h59m00s 0j_00h59m59s 0j_01h00m00s 0j_01h01m00s 0j_01h02m00s 0j_02h00m00s 0j_02h01m00s 0j_02h03m00s 0j_02h10m00s 0j_02h12m00s 1j_00h00m00s 1j_01h01m00s 2j_00h00m00s");
    std::vector<Date> dates;
    while(!str.eof()){
        Date d;
        str >> d >> std::ws;
        std::cout << d << std::endl;
        if(str) dates.push_back(d);
    }
    
    // Test du comparateur
    for(unsigned int i=0;i<dates.size();i++)
        for(unsigned int j=0;j<dates.size();j++){
             if(i<j){
                 if(!(dates[i]<dates[j]))
                     std::cout << "Erreur : dates[" << i << "]<dates[" << j << "] : " << dates[i] << "<" << dates[j] << " retourne faux!" << std::endl;
             }else if(j<i){
                 if(!(dates[j]<dates[i]))
                     std::cout << "Erreur : dates[" << j << "]<dates[" << i << "] : " << dates[j] << "<" << dates[i] << " retourne faux!" << std::endl;
             }else{ // i==j
                 if(dates[j]<dates[i])
                     std::cout << "Erreur : dates[" << j << "]<dates[" << i << "] : " << dates[j] << "<" << dates[i] << " retourne faux!" << std::endl;
             }
        }
    std::cout << " --- FIN ---" << std::endl;
    return 0;
}

