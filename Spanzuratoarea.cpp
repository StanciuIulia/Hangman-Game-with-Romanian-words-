#include "Spanzuratoarea.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <iostream>
#include <fstream>
#include <cctype>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>

using namespace std;

//
// Hangman functions
//

//constructor HangmanWord
//Initializeaza cunvantu si cuvantul criptat, lasand vizibile prima litera, ultima litera si literele din interiorul cuvantului care sunt identice cu prima si/sau ultima litera.
//Adauga in vectorul de litere corecte si in cel de litere folosite prima si ultima litera (doar o data daca acestea coincid).
HangmanWord::HangmanWord(string word):word(word){
    int lungime_cuvant = word.length();

    crypted_word.push_back(word[0]);

    for(int i=1; i<lungime_cuvant-1; ++i){
        if( tolower(word[i]) == tolower(word[0]) ){
            crypted_word.push_back(tolower(word[0]));
        }
        else
            if( tolower(word[i]) == tolower(word[lungime_cuvant-1]) ){
                crypted_word.push_back(tolower(word[lungime_cuvant-1]));
            }
            else{
                crypted_word.push_back('*');

            }
    }
    crypted_word.push_back(word[lungime_cuvant-1]);

    add_right_letter(word[0]);
    add_used_letter(word[0]);

    if( tolower(word[0]) != tolower(word[lungime_cuvant-1]) ){
        add_right_letter( word[lungime_cuvant-1] );
        add_used_letter( word[lungime_cuvant-1] );
    }

}

//Destructor HangmanWord
HangmanWord::~HangmanWord(){
    wrong_letters.clear();
    right_letters.clear();
    used_letters.clear();
}

//Adauga litera gresita in vectorul de litere care nu apartin cuvantului si au fost incercate de jucatori
void HangmanWord::add_wrong_letter(char letter){
    this->wrong_letters.push_back(tolower(letter));
}

//Adauga litera corecta in vectorul de litere care apartin cuvantului si au fost incercate de jucatori
void HangmanWord::add_right_letter(char letter){
    this->right_letters.push_back(tolower(letter));
}
//Adauga litera in vectorul de litere incercate de jucatori
void HangmanWord::add_used_letter(char letter){
    this->used_letters.push_back(tolower(letter));
}

//Verifica daca litera introdusa face parte din cuvant. Daca da, returneaza true si decripteaza litera introdusa in cuvantul criptat. Daca nu, returneaza false.
bool HangmanWord::decrypt(char letter){
    bool is_there = false;

    for(int i=1; i<word.length()-1; ++i){
        if(tolower(word[i]) == tolower(letter)){
            crypted_word[i] = word[i];
            is_there = true;
        }
    }

    return is_there;
}

//Returneaza cuvantul necriptat.
string HangmanWord::get_word(){
    return this->word;
}

//Returneaza cuvantul criptat.
string HangmanWord::get_crypted_word(){
    return this->crypted_word;
}

//Afiseaza cuvantul la finalul jocului.
void HangmanWord::show_word(){
    cout << "Cuvantul era "<< this->word << "!\n";
}

//Afiseaza cuvantul criptat in timpul jocului, dupa fiecare incercare de a ghici o litera.
void HangmanWord::show_crypted_word(){
    cout << "Ghiceste cuvantul... " << this->crypted_word << "\n";
}

//Afiseaza literele corecte ca litere mici.
void HangmanWord::show_wrong_letters(){
    for(char c:wrong_letters){
        cout << c << " ";
    }
    cout << "\n";
}

//Afiseaza literele incorecte ca litere mici.
void HangmanWord::show_right_letters(){
    for(char c:right_letters){
        cout << c << " ";
    }
    cout << "\n";
}

//Afiseaza literele incercate de jucatori ca litere mici.
void HangmanWord::show_used_letters(){
    cout << "Literele deja folosite sunt: ";
    for(char c:used_letters){
        cout << c << " ";
    }
    cout << "\n\n";
}



//
// Player functions
//

//constructor Player
//Initializeaza numele jucatorului si viata acestuia ca 5(maximul posibil)-fiecare jucator poate gresi de 5 ori inainte sa piarda.
Player::Player(string name):name(name){
    this->life = 5;
}

//destructor Player
Player::~Player(){}

//Returneaza numele jucatorului.
string Player::get_name(){
    return this->name;
}

//Returneaza viata jucatorului (cate greseli mai poate sa faca inainte sa piarda).
int Player::get_life(){
    return this->life;
}

//Micsoreaza viata jucatorului.
void Player::decrease_life(){
    this->life--;
}


//Afiseaza un mesaj informativ despre viata jucatorului cu un desen cu spanzuratoarea (statusul vietii).
void Player::show_life(){
    cout << "Jucatorul " << this->name << " mai are " << this->life << " vieti.\n";

    if(this->life == 5){
        cout << " __\n|  |\n|  \n| \n|  \n";
    }

    if(this->life == 4){
        cout << " __\n|  |\n|  O\n| \n|  \n";
    }

    if(this->life == 3){
        cout << " __\n|  |\n|  O\n|  | \n|  \n";
    }

    if(this->life == 2){
        cout << " __\n|  |\n|  O\n| ~|\n|  \n";
    }

    if(this->life == 1){
        cout << " __\n|  |\n|  O\n| ~|~\n|  \n";
    }

    if(this->life == 0){
        cout << " __\n|  |\n|  O\n| ~|~\n|  () ";
        cout << "\n";
        cout << "Jucatorul " << this->name << " a pierdut. De acum, nu mai poate ghici pana la finalul turei.\n";
    }

}

//Verifica daca litera a mai fost incercata.
//Daca da, stare o tura pentru jucatorul care a gresit.
//Daca nu, daca litera este corecta o adauga in vectorul de litere corecte si in cel de litere incercate si returneaza true pentru a continua randul jucatorului.
//daca nu, iar litera este incorecta, o dacauga in vectorul de litere gresite si in cal de litere incercate si returneaza false pentru a opri randul jucatorului si a trece la urmatorul jucator.
bool Player::check_letter(char letter, HangmanWord *hw){
    bool is_good = false;

    for(int i=0; i<hw->used_letters.size(); ++i){
        if(tolower(letter) == hw->used_letters[i]){
            cout << "NU AI FOST ATENT SI TI-AI PIERDUT RANDUL!\n";
            return false;
        }
    }

    if(hw->decrypt(letter)){
        hw->add_right_letter(letter);
        cout << "Jucatorul " << this->name << " a ghicit litera " << letter << "!\n\n";
        is_good = true;
    }
    else {
        hw->add_wrong_letter(letter);
        this->decrease_life();
        this->show_life();
    }
    hw->add_used_letter(letter);
    return is_good;
}

//Verifica daca cuvantul a fost ghicit de jucator si afiseaza un mesaj de castig.
bool Player::show_winning_status(HangmanWord *hw){
    bool is_winner = false;
    if(this->life !=0 && hw->get_word()==hw->get_crypted_word()){
        cout << "Jucatorul " << this->name << " a ghicit cuvantul si a castigat. Felicitari, " << this->name << "!\n";
        is_winner = true;
    }

    return is_winner;
}


//
// Game Functions
//

//constructor joc
Game::Game(ifstream& MyReadFile, int nr_players):nr_of_players(nr_players){

    string word = "";
    while (getline (MyReadFile, word)){
        HangmanWord *hw = new HangmanWord(word);
        words.push_back(hw);
    }

    if(nr_players == 1){
        cout << "Ai ales modul Single Player!\n\n";
        cout << "Adauga un nume jucatorului tau...";

        string nume_jucator;
        cin.ignore();
        getline(cin, nume_jucator);
        Player *jucator = new Player(nume_jucator);

        players.push_back(jucator);
    }
    else {
        cout << "Ai ales modul Multiple Players cu " << nr_players << " jucatori!\n";
        string nume_jucator;
        cin.ignore();

        for(int i=0; i<nr_players; ++i){
            cout << "Adauga un nume jucatorului " << i+1 << "...";
            getline(cin, nume_jucator);

            Player *jucator = new Player(nume_jucator);
            players.push_back(jucator);
        }
    }
}


//destructor joc
Game::~Game(){
//to do
}

void Game::initialize_word(){

    switch(nr_of_players){
        case 1:{
            cout << "Te rog asteapta. Cuvantul este ales aleator de catre programul nostru...\n";
            break;
        }
        default:{
            cout << "Va rog asteptati. Cuvantul este ales aleator de catre programul nostru...\n";
            break;
        }
    }

    Sleep(3000);

    unsigned seed = chrono::system_clock::now()
                        .time_since_epoch()
                        .count();
    shuffle (words.begin(), words.end(), default_random_engine(seed));

    random_device random_device;
    mt19937 engine{random_device()};
    uniform_int_distribution<int> dist(0, words.size() - 1);

    int random_index = dist(engine);

    cout << "Gata! Start joc!\n\n";
    word_in_play = words[random_index];
}

void Game::play(){
    char letter = ' ';
    int nr = nr_of_players;
    while(word_in_play->get_word() != word_in_play->get_crypted_word() && nr> 0){
        for(int i=0; i<nr_of_players; ++i){
            if(players[i]->get_life()>0){
                cout << "Este randul jucatorului " << players[i]->get_name() << ".\n";
                do{
                    word_in_play->show_crypted_word();
                    word_in_play->show_used_letters();
                    cout << "\n" << "Ghiceste o litera: ";
                    cin >> letter;
                }
                while(players[i]->check_letter(letter, word_in_play) && word_in_play->get_word() != word_in_play->get_crypted_word());

            if(players[i]->show_winning_status(word_in_play)){
                word_in_play->show_word();
                return;
                exit;
            }
            if(players[i]->get_life() == 0){
                --nr;
            }
            }
        }
    }
    if(nr==0){
        cout << "Jocul nu a fost castigat de nimeni.\n";
        word_in_play->show_word();
        return;
        exit;
    }
}
