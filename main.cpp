#include "Spanzuratoarea.h"
#include <iostream>
#include <stdio.h>
#include <fstream>

using namespace std;

int main(){

    ifstream MyReadFile("Cuvinte.txt");
    cout << "Selecteaza modul preferat de joc!\n\n" << "Pentru a alege modul Single Player apasa 1.\n" << "Pentru modul Multiple Players, scrie numarul de jucatori dorit. \n" << "Pentru a opri jocul apasa 0.\n\n";

    int mod_joc=0;
    while(mod_joc<=0){
        cin >> mod_joc;
        if(mod_joc>0){
            Game joc(MyReadFile, mod_joc);
            joc.initialize_word();
            joc.play();
        }
        else
            if(mod_joc<0){
                cout << "Pentru a va juca, introduceti un numar > 0.\n";
            }
            else if(mod_joc==0){
                cout << "Ati ales incheierea jocului. Va multumim!\n";
            }

    }
    return 0;
}

