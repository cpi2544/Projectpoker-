#ifndef PROJECT_H
#define PROJECT_H
#include <string>
#include <vector>
using namespace std;
struct Deck
{
private:
    char ranks[13] = {'2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A'};
    char suits[4] = {'\3', '\4', '\5', '\6'};

public:
    Deck();
    vector<string> allCardLeft;
    void shuffle();
    void showCardLeft();
};
struct Player
{
private:
    string hand;
    int rankOfHand;

public:
    Player(string, long long int);
    string name;
    long long int moneyGame;
    long long int moneyInWeb;
    vector<string> card;
    string status;
    string role;
    int order;
};
class PokerGame
{
private:
    vector<string> board;
    int num_player;
    int round;
    int current;
    int dealer;
    int mandatory_bet;
    long long int boardMoney;
    long long int betMoney;
    bool restart;
    bool haveBetOrAllIn;

public:
    PokerGame();
    PokerGame(Deck &, int, long long int, int);
    ~PokerGame();
    vector<Player *> players;
    Deck deck;
    void showBoard();
    void showMoneyBoard();
    void showMoneyBet();
    void showPlayerMoney(Player *);
    void showPlayerCard(Player *);
    void holeCard();
    void communityCards(int);
    string checkHand(const vector<string> &, const vector<string> &); // ใช้ & เพราะลดการใช้เวลาใน Copy Vector
    void showChoice();
    void recieveOd(Player *);
    void doOrder(Player *);
    void check(Player *);
    void bet(Player *);
    void call(Player *);
    void raise(Player *);
    void allIn(Player *);
    void fold(Player *);
    void assignRole();
    void round1();
    void showLostMoneyForMandatoryBet();
};
#endif