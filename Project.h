#ifndef PROJECT_H
#define PROJECT_H
#include <string>
#include <vector>
using namespace std;
struct Deck
{
private:
    char ranks[13] = {'2','3','4','5','6','7','8','9','T','J','Q','K','A'};
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
    Player();
    vector<string> card;
    string name;
    string status;
    int order;
    long long int money;
};
class PokerGame
{
private:
    vector<string> board;
    int num_player;
    int round;
    int turn;
    long long int boardMoney;
    long long int betMoney;
    bool haveBetOrAllIn;

public:
    PokerGame();
    PokerGame(Deck &, int, long long int);
    ~PokerGame();
    Player *player;
    Deck deck;
    void showBoard();
    void showMoneyBoard();
    void showMoneyBet();
    void showPlayerMoney(Player *);
    void showPlayerCard(Player *);
    void holeCard(Player *, int);
    void communityCards(int);
    string checkHand(const vector<string> &, const vector<string> &);//ใช้ & เพราะลดการใช้เวลาใน Copy Vector
    void recieveOd(Player *);
    void doOrder(Player *);
    void check(Player *);
    void bet(Player *);
    void call(Player *);
    void raise(Player *);
    void allIn(Player *);
    void fold(Player *);
    void round1();
};
#endif