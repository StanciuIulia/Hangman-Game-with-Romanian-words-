#ifndef SPANZURATOAREA_H_INCLUDED
#define SPANZURATOAREA_H_INCLUDED

#include <string>
#include <vector>
using namespace std;


class HangmanWord{
private:
    string word;
    string crypted_word;
    vector <char> wrong_letters;
    vector <char> right_letters;
    vector <char> used_letters;

public:
    HangmanWord(string);
    ~HangmanWord();

    void add_wrong_letter(char);
    void add_right_letter(char);
    void add_used_letter(char);

    bool decrypt(char);

    void show_wrong_letters();
    void show_right_letters();
    void show_used_letters();

    string get_word();
    string get_crypted_word();

    void show_word();
    void show_crypted_word();

friend class Player;
};

class Player{
private:
    string name;
    int life;

public:
    Player(string);
    ~Player();

    string get_name();
    int get_life();

    void decrease_life();
    void show_life();

    bool check_letter(char, HangmanWord*);
    bool show_winning_status(HangmanWord*);
};

class Game{
private:
    vector <HangmanWord *> words;
    int nr_of_players;
    vector <Player *> players;
    HangmanWord* word_in_play;
    Player* winner;
public:
    Game(ifstream&, int );
    ~Game();
    void initialize_word();
    void play();
};
#endif // SPANZURATOAREA_H_INCLUDED
