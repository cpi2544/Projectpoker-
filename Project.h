#ifndef PROJECT_H
#define PROJECT_H
#include <string>
#include <vector>
using namespace std;
struct Deck
{
private:
    string ranks[13] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};
    char suits[4] = {'\3', '\4', '\5', '\6'};

public:
    Deck();
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
    // string hand;
    vector<string> card;
    int order;
    // int rankOfHand;
    long long int money;
};
class PokerGame
{
private:
    vector<string> board;
    int boardSize;
    int num_player;
    int round;
    int turn;
    int cntCheck; // ถ้า == จำนวนผู้เล่น แปลว่าทุกคนเช็คหมดไปรอบถัดไปได้
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
    void communityCards(int);
    void holeCard(Player *, int);
    void showPlayerMoney(Player *);
    void showPlayerCard(Player *);
    void recieveOd(Player *);
    void doOrder(Player *);
    void check(Player *);
    void bet(Player *);
    void call(Player *);
    void raise(Player *);
    void allIn(Player *);
    void fold(Player *);
    // string checkHand(const vector<string> &, const vector<string> &);//ใช้ & เพราะลดการใช้เวลาใน Copy Vector
    void round1();
};
#endif