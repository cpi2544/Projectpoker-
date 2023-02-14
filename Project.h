#ifndef PROJECT_H
#define PROJECT_H
#include <string>
#include <vector>
using namespace std;
struct Deck
{
public:
    Deck();
    string ranks[13] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};
    char suits[4] = {'\3', '\4', '\5', '\6'};
    vector<string> allCardLeft;
    int cardLeft;
    void shuffle();
    void showCardLeft();
};
struct Player
{
public:
    Player();
    string name;
    string status;
    vector<string> card;
    int order;
    long long int money;
    void drawCard(Deck &, int);
};
class PokerGame
{
public:
    PokerGame();
    PokerGame(Player *&, Deck &, int, long long int);
    ~PokerGame();
    Player *player; // ประกาศไว้เพื่อสร้าง Object n ตัวใดๆของ ClassPlayer แบบ Dynamic Allocation
    Deck deck;
    vector<string> board;
    int boardSize;
    int num_player;
    int round;
    int turn;
    long long int boardMoney;
    long long int betMoney;
    bool haveBetOrAllin;
    void showBoard();
    void showMoneyBoard();
    void showMoneyBet();
    void showPlayerMoney(Player *);
    void showPlayerCard(Player *);
    void recieveOd(Player *);
    void doOrder(Player *);
    void bet(Player *);
    void call(Player *);
    void raise(Player *);
    void allIn(Player *);
    void round1();
};
#endif