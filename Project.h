#ifndef PROJECT_H
#define PROJECT_H
#include <iostream>
#include <cstring>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <map>
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

public:
    Player(string, long long int);
    string name;
    int moneyGame;
    int moneytoBet;
    long long int moneyInWeb;
    vector<string> card;
    string hand;
    string action;
    string role;
    int rankOfHand;
    int order;
};
class PokerGame
{
private:
    vector<string> cardsOnBoard;
    int num_player;
    int current;
    int dealer;
    int indexRaiseOrBet;
    int round;
    int mandatory_bet;
    long long int pot;
    long long int betMoney;
    bool restart;
    bool hasBetOrAllIn;

public:
    PokerGame(Deck &, int, long long int, int);
    ~PokerGame();
    vector<Player *> players;
    Deck deck;
    void showBoard();
    void showMoneyPot();
    void showMoneyBet();
    void showPlayerMoney(Player *);
    void showPlayerCard(Player *);
    void showHandType(Player *p);
    void holeCard();
    void communityCards(int);
    string checkHand(const vector<string> &, const vector<string> &); // ใช้ & เพราะลดการใช้เวลาใน Copy Vector
    void showChoice();
    void recieveOrder(Player *);
    void checkOrder(Player *);
    void doOrder(Player *);
    void check(Player *);
    void bet(Player *);
    void call(Player *);
    void raise(Player *);
    void allIn(Player *);
    void fold(Player *);
    void assignRole();
    void resetAction();
    void lostMoneyForMandatoryBet(Player *, Player *);
    bool findWinner();
    void updateRound();
    void preflop();
    void flop();
    void turn();
};
string convertToLower(string s)
{
    for (int i = 0; i < s.length(); i++)
    {
        s[i] = tolower(s[i]);
    }
    return s;
}
void drawPic()
{
    cout << "\n===============================================================================================";
    cout << "\n   CCCCC      A        SSSSSSSSS   IIIIIIIII  NN     NN    OOOOOOOO     ";
    cout << "\n CC          A  A      SS             III     NN N   NN   OO      OO    ";
    cout << "\nCC          A    A     SSSSSSSSS      III     NN  N  NN   OO      OO    ";
    cout << "\n CC        AAAAAAAA           SS      III     NN   N NN   OO      OO    ";
    cout << "\n   CCCCC  A        A   SSSSSSSSS   IIIIIIIII  NN     NN    OOOOOOOO     ";
    cout << "\n===============================================================================================";
    cout << "\n======================== Welcome to Poker Game=================================================\n";
}
void recieveSimpleInformation(int &num, long long int &money, int &mandatory_betRef)
{

    do
    {
        cout << "Enter Number Of Player(2-4 People): ";
        cin >> num;
        if (num < 2 || num > 4)
            cout << "Invalid number of player\n";
    } while (num < 2 || num > 4); // ถามว่ามากี่คน

    do
    {
        cout << "How much money do you want to play(1,000 - 1,000,000): ";
        cin >> money;
        if (money < 1000 || money > 1000000)
            cout << "Invalid money value\n";
        cin.ignore();
    } while (money < 1000 || money > 1000000); // ถามจะให้มีคนละกี่บาท
}
PokerGame::PokerGame(Deck &dRef, int numRef, long long int moneyRef, int mandatory_betRef) // สร้าง 1 PokerGame ต้องมีข้อมูลพื้นฐานตาม Parameterต่อไปนี้ ตำแหน่งคนจริง(&) สำรับไพ่ จำนวนคน
{
    deck = dRef;         // กำหนด *deck ให้ตรงกับสำรับไพ่ที่เราสร้างขึ้นมา
    num_player = numRef; // กำหนดจำนวนคนของเกม Poker ของเรา
    players.emplace_back(new Player("1", 0));
    players.emplace_back(new Player("2", 0));
    players.emplace_back(new Player("3", 0));
    players.emplace_back(new Player("4", 0));
    for (int i = 0; i < numRef; i++)
    {
        players[i]->moneyGame = moneyRef;
    } // กำหนดเงินเรียงคน
    current = 0;
    dealer = rand() % num_player;
    mandatory_bet = mandatory_betRef; // เงินเดิมพันขั้นต่ำ
    pot = 0;                          // กำหนดเงินใน Board ตั้งต้น
    betMoney = 0;                     // กำหนด ว่าตอนนี้ค่าเงิน Betสูงสุดเท่าไหร่ คน Call Raise จะได้รู้
    round = 1;                        // กำหนดรอบของเกม
    restart = false;                  // กำหนดให้เป็นNew game ไม่ได้ Restart อยู่
    hasBetOrAllIn = false;            // กำหนดว่ามีการ Bet || All inไปรึยัง ถ้ามีไปแล้วจะ Check ไม่ได้แล้ว แต่ Call Raise หรือ Fold ได้
}
PokerGame::~PokerGame()
{
    for (int i = 0; i < players.size(); i++)
    {
        delete players[i];
    }
}
void PokerGame::showBoard() // Show ว่า Board มีไพ่ไหนบ้างตอนนี้
{
    cout << "Current Board: ";
    for (size_t i = 0; i < cardsOnBoard.size(); i++)
    {
        cout << cardsOnBoard[i] << " ";
    }
    cout << "\n";
}
void PokerGame::showMoneyPot() // Show ว่าเงินใน Board มีเท่าไหร่แล้ว
{
    cout << "Current Board Money: " << pot << "\n";
}
void PokerGame::showMoneyBet() // Show ว่าเงิน Bet สูงสุดตอนนี้เท่าไหร่แล้ว
{
    cout << "Current Bet Money: " << betMoney << "\n";
}
void PokerGame::showPlayerMoney(Player *p) // Showว่าเงินคนปัจจุบัน(ใน Parameter ) เหลือเท่าไหร่
{
    cout << p->name << "'s Current Money: " << p->moneyGame << "\n";
}
void PokerGame::showPlayerCard(Player *p) // Showว่าไพ่คนปัจจุบัน(ใน Parameter ) มีอะไรบ้าง
{
    cout << p->name << "'s Cards: ";

    for (size_t i = 0; i < p->card.size(); i++)
    {
        if (p->card[i][0] == 'T')
            cout << "10" << p->card[i].back() << " ";
        else
            cout << p->card[i] << " ";
    }
    cout << "\n";
}
void PokerGame::holeCard()
{
    for (size_t i = 0; i < num_player; i++)
    {
        for (size_t amount = 0; amount < 2; amount++)
        {
            players[i]->card.emplace_back(deck.allCardLeft.back());
            deck.allCardLeft.pop_back();
        }
    }
}
void PokerGame::communityCards(int n)
{

    for (size_t i = 0; i < n; i++)
    {
        cardsOnBoard.emplace_back(deck.allCardLeft.back());
        deck.allCardLeft.pop_back();
    }
}
void PokerGame::resetAction()
{

    for (size_t i = 0; i < num_player; i++)
    {
        if (restart)
            players[i]->role = "";
        else if (players[i]->action != "flop")
            players[i]->role = "";
    }
}
void PokerGame::assignRole()
{
    if (restart)
    {
        dealer = (dealer + 1) % num_player;
        resetAction();
    }
    players[dealer]->role = "dealer";
    players[(dealer + 1) % num_player]->role = "small-blind";
    players[(dealer + 2) % num_player]->role = "big-blind";
}
void PokerGame::lostMoneyForMandatoryBet(Player *s, Player *b)
{

    cout << s->name << " is Small-Blind you lost money from Mandatory-bet :" << mandatory_bet / 2 << "\n";
    s->moneyGame -= mandatory_bet / 2;

    cout << b->name << " is Big-Blind you lost money from Mandatory-bet :" << mandatory_bet << "\n";
    b->moneyGame -= mandatory_bet;
}
void PokerGame::preflop() // เริ่มรอบแรกของเกม
{
    current = (dealer + 3) % num_player;                                                              // คนซ้าย Big blind index ได้เริ่มก่อน;
    deck.shuffle();                                                                                   // สับไพ่ในสำรับก่อน
    assignRole();                                                                                     // จัด Role ให้ผู้เล่นให้ครบ ว่าเป็น Dealer or small or big
    lostMoneyForMandatoryBet(players[(dealer + 1) % num_player], players[(dealer + 2) % num_player]); // ก่อนแจกไพ่ต้องมีการวางเดิมพันก่อนสำหรับ small and big
    holeCard();                                                                                       // ทุกคนยังไม่มีไพ่บนมือดังนั้นเราจะเริ่มด้วยการแจกไพ่คนละ2ใบก่อน
    while (round == 1)                                                                                // เริ่มรอบแรกอย่างเป็นทางการ
    {
        if (players[current]->action == "fold")
            continue; // เจอคนหมอบก็ข้ามได้เลย
        showBoard();
        showMoneyPot();
        showMoneyBet();
        cout << players[current]->name << "'s Turn\n";
        showPlayerCard(players[current]);
        // if (players[current]->hand == "")
        //     checkHand(players[current]->card, cardsOnBoard);
        // showHandType(players[current]);
        showPlayerMoney(players[current]);
        cout << "Enter Your Action\n";
        showChoice();
        recieveOrder(players[current]);
        if (findWinner())
            break;
        else
        {
            updateRound();
        }
    }
}
void PokerGame::showHandType(Player *p)
{
    cout << p->hand;
}
bool PokerGame::findWinner()
{
    int cntFlop = 0;
    for (auto &p : players)
    {
        if (p->action == "flop")
            cntFlop++;
    }
    if ((round == 1 || round == 2 || round == 3) && cntFlop == num_player - 1)
    { // ในรอบ preflop flop turn
        for (auto &p : players)
        {
            if (p->action != "flop")
            {
                cout << p->name << " Win this game!!!\n";
                cout << p->moneyGame << "+" << pot << "\n";
                p->moneyGame += pot;
                return true;
            }
        }
    }
    else if (round == 4)
    {
        for (auto &p : players)
        {
            if (p->action != "flop")
            {
                cout << p->name << "'s Card :";
                for (int i = 0; i < 2; i++)
                    cout << p->card[i] << " ";
                cout << "\nRank Of Hand = " << p->rankOfHand << "\n";
                return true;
            }
        }
    }
    return false;
}
void PokerGame::updateRound()
{
    for (auto &p : players)
    {
        if (p->action == "")
        {
            current = (current + 1) % num_player;
            return;
        }
    }
    round++;
}
void PokerGame::turn()
{
    int cntfold = 0;
    communityCards(3); // เปิดไพ่กองกลาง 3 ใบ
    while (round == 2) // เริ่มรอบสองอย่างเป็นทางการ
    {
        if (players[current]->action == "fold")
            continue; // เจอคนหมอบก็ข้ามได้เลย
        showBoard();
        showMoneyPot();
        showMoneyBet();
        cout << players[current]->name << "'s Turn\n";
        showPlayerCard(players[current]);
        showPlayerMoney(players[current]);
        cout << "Enter Your Action\n";
        showChoice();
        recieveOrder(players[current]);
        current = (current + 1) % num_player;
        for (auto &p : players)
        {
            if (p->action == "fold")
                cntfold++;
        }
        if (cntfold == num_player - 1)
        {
            for (auto &p : players)
            {
                if (p->action != "fold")
                    cout << "The Winner is " << p->name << "!!!!";
                p->moneyGame += pot;
            }
        }
        else
            cntfold = 0;
        if (current == (current + 1) % num_player) // ถ้าเป็นตาที่เกินจาก Dealer ไปแล้วเริ่มรอบใหม่
        {
            hasBetOrAllIn = false;
            round++;
        }
    }
}
void PokerGame::showChoice()
{
    int num = 1;
    if (!hasBetOrAllIn && round > 1)
    {

        cout << num++ << ".Check\n"; //  1
        cout << num++ << ".Bet\n";   // 2
    }
    cout << num++ << ".Call\n";   // 3
    cout << num++ << ".Raise\n";  // 4
    cout << num++ << ".Cheat\n";  // 5
    cout << num++ << ".All-In\n"; // 6
    cout << num++ << ".Fold\n";   // 7
}

void PokerGame::recieveOrder(Player *p) // รับคำสั่งมาก่อนว่าผู้เล่นต้องการทำอะไร ฟังชั่นนี้เราจะเช็คว่าเราจะทำคำสั่งนั้นได้หรือไม่ (ต้องผ่านเงื่อนไข ขั้นพื้นฐานก่อน)
{
    string order = "";
    bool passStage = true;
    do
    {
        passStage = true;
        cout << "Input Your Choice: ";
        getline(cin, order);
        p->order = stoi(order);
        if (p->order < 1 && p->order > 7 && !hasBetOrAllIn && round > 1)
        {
            cout << "Invalid Order Try Again\n";
            passStage = false;
        }
        else if (p->order < 1 && p->order > 5)
        {
            cout << "Invalid Order Try Again\n";
            passStage = false;
        }
    } while (!passStage);
    checkOrder(p);
    doOrder(p);
}
void PokerGame::checkOrder(Player *p)
{
}
void PokerGame::doOrder(Player *p) // หลังจากทำได้เราจะเริ่มทำคำสั่งนั้นโดยแยกตาม ว่าคำสั่งที่ถูก Player คนนั้นสั่งนั้นเป็นอะไร
{
    if (round == 1)
    {
        switch (p->order)
        {
        case 1:
            call(p);
            break;
        case 2:
            raise(p);
            break;
        case 3:
            // cheat(p);
            break;
        case 4:
            allIn(p);
            break;
        default:
            p->action = "fold";
            break;
        }
    }
    if (round == 1)
    {
        switch (p->order)
        {
        case 1:
            check(p);
            break;
        case 2:
            bet(p);
            break;
        case 3:
            allIn(p);
            break;
        default:
            p->action = "fold";
            break;
        }
    }
}
// string PokerGame::checkHand(const vector<string> &h, const vector<string> &b)
// { // Check กรณีดังต่อไปนี้ 2 5 6 7 ใบ
// }
void PokerGame::check(Player *p)
{
    p->action = "check";
}
void PokerGame::bet(Player *p)
{
    do
    {
        int od = 0;
        cout << "How much money do you want to bet?\n";
        cin >> p->moneytoBet;
        if (p->moneytoBet > p->moneyGame)
        {
            cout << "You don't have enough money\nPress 1:Try Again\nPress 2:Exit\n";
            cin >> od;
            if (od == 1)
                continue;
            else if (od == 2)
            {
                recieveOrder(p);
                return;
            }
            else
                cout << "Invalid Input Try Again\n";
        }
    } while (betMoney > p->moneyGame);
    resetAction();
    p->action = "bet";
    pot += p->moneytoBet;
    p->moneyGame -= p->moneytoBet;
    betMoney += p->moneytoBet;
    hasBetOrAllIn = true;
}
void PokerGame::call(Player *p)
{

    pot += betMoney;

    p->action = "call";
}
void PokerGame::raise(Player *p)
{
    do
    {
        int od = 0;
        cout << "How much money do you want to raise?\n";
        cin >> p->moneytoBet;
        if (p->moneytoBet > p->moneyGame)
        {
            cout << "You don't have enough money to raise\nPress 1:Try Again\nPress 2:Exit\n";
            cin >> od;
            if (od == 1)
                continue;
            else if (od == 2)
            {
                recieveOrder(p);
                return;
            }
            else
                cout << "Invalid Input Try Again\n";
        }
    } while (p->moneytoBet > p->moneyGame);
    resetAction();
    p->action = "raise";
    pot += (betMoney + p->moneytoBet);
    p->moneyGame -= (betMoney + p->moneytoBet);
    betMoney += p->moneytoBet;
    hasBetOrAllIn = true;
}
void PokerGame::allIn(Player *p)
{
    pot += p->moneyGame;
    betMoney += p->moneyGame;
    p->moneyGame = 0;
    p->action = "all_in";
}
void PokerGame::fold(Player *p)
{
    p->action = "fold";
}
Player::Player(string name, long long int moneyInWeb) : name(name), moneyInWeb(moneyInWeb)
{
    action = "";
    role = "";
    moneytoBet = 0;
    order = 1;
}
Deck::Deck()
{
    for (size_t i = 0; i < 13; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            allCardLeft.emplace_back(string(1, ranks[i]) + string(1, suits[j]));
        }
    }
}
void Deck::shuffle()
{
    random_shuffle(allCardLeft.begin(), allCardLeft.end());
}
void Deck::showCardLeft()
{
    for (size_t i = 0; i < allCardLeft.size(); i++)
    {
        cout << allCardLeft[i] << " ";
    }
    cout << "\n";
}
#endif