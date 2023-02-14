#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include "Project.h"
using namespace std;
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
void recieveSimpleInformation(int &num, long long int &money)
{
    do
    {
        cout << "Enter Number Of Player(2-4 People): ";
        cin >> num;
        if (num < 2 || num > 4)
            cout << "Invalid number of player\n";
        cin.ignore();
    } while (num < 2 || num > 4); // ถามว่ามากี่คน
    do
    {
        cout << "How much money do you want to play(1,000 - 1,000,000): ";
        cin >> money;
        if (money < 1000 || money > 1000000)
            cout << "Invalid money value\n";
        cin.ignore();
    } while (money < 1000 || money > 1000000);
}
PokerGame::PokerGame(Player *&pRef, Deck &dRef, int num, long long int moneyRef) // สร้าง 1 PokerGame ต้องมีข้อมูลพื้นฐานตาม Parameterต่อไปนี้
{
    player = pRef;         // ไล่กำหนดค่าในเกม Poker
    deck = dRef;           // ไล่กำหนดค่าในเกม Poker
    num_player = num;      // ไล่กำหนดค่าในเกม Poker
    for(int i = 0 ; i  < num ; i++){
        (player+i)->money = moneyRef;
    }
    round = 1;
    turn = 0;
    boardMoney = 0;
    betMoney = 0;
    haveBet = false;
}
PokerGame::~PokerGame()
{
    delete[] player;
}
void PokerGame::showBoard()
{
    cout << "Current Board: ";
    for (int i = 0; i < boardSize; i++)
    {
        cout << board[i] << " ";
    }
    cout << "\n";
}
void PokerGame::showMoneyBoard()
{
    cout << "Current Board Money: " << boardMoney << "\n";
}
void PokerGame::showMoneyBet()
{
    cout << "Current Bet Money: " << betMoney << "\n";
}
void PokerGame::showPlayerCard(Player *p)
{
    cout << p->name << "'s Cards: ";
    for (int i = 0; i < p->card.size(); i++)
    {
        cout << p->card[i] << " ";
    }
    cout << "\n";
}
void PokerGame::showPlayerMoney(Player *p)
{
    cout << p->name << "'s Current Money: " << p->money << "\n";
}
void PokerGame::round1()
{
    deck.shuffle(); // สับไพ่ในสำรับก่อน
    for (int i = 0; i < num_player; i++)
    {
        player[i].drawCard(deck, 2);
    } // ทุกคนยังไม่มีไพ่บนมือดังนั้นเราจะเริ่มด้วยการแจกไพ่คนละ2ใบก่อน
    while (round == 1)
    {

        Player *currentPlayer = player + turn;
        if (currentPlayer->status == "fold")
        {
            turn++;
            continue;
        }
        showBoard();
        showMoneyBoard();
        showMoneyBet();
        cout << currentPlayer->name << "'s Turn\n";
        showPlayerCard(currentPlayer);
        showPlayerMoney(currentPlayer);
        cout << "Enter Your Action\n";
        if (!haveBet)
            cout << "1.Check\n2.Bet\n";
        else
        {
            cout << "1.Call\n2.Raise\n";
        }
        cout << "3.Fold\n";
        recieveOd(currentPlayer);
        turn++;
        if (turn == num_player)
        {
            turn = 0;
            haveBet = false;
            round++;
        }
    }
}
void PokerGame::recieveOd(Player *p)
{
    do
    {
        cout << "Input Your Choice: ";
        cin >> p->order;
        if (p->order < 1 || p->order > 3)
            cout << "Invalid Order Try Again\n";
        else if (haveBet && p->order == 1 && p->money - betMoney < 0)
            cout << "You don't have enough money to call";
        else if (haveBet && p->order == 2 && p->money - betMoney <= 0)
            cout << "You don't have enough money to raise";
    } while (p->order < 1 || p->order > 3);
    doOrder(p);
}
void PokerGame::doOrder(Player *p)
{
    if (!haveBet)
    {
        switch (p->order)
        {
        case 1:
            p->status = "check";
            break;
        case 2:
            bet(p);
            break;
        default:
            p->status = "fold";
            break;
        }
    }
    else
    {
        switch (p->order)
        {
        case 1:
            p->status = "call";
            break;
        case 2:
            p->status = "raise";
            do
            {
                cout << "How much money do you want to raise? ( > ) " << betMoney << "\n";
                cin >> betMoney;
                if (betMoney > p->money)
                    cout << "You don't have enough money please try again\n";
            } while (betMoney > p->money);
            break;
        default:
            p->status = "fold";
            break;
        }
    }
}
void PokerGame::bet(Player *p)
{
    do
    {
        int od = 0;
        cout << "How much money do you want to bet?\n";
        cin >> betMoney;
        if (betMoney > p->money)
        {
            cout << "You don't have enough money\nPress 1:Try Again\nPress 2:Exit\n";
            cin >> od;
        }
        if(od == 1 ) continue;
        else if (od == 2)
        {
            recieveOd(p);
            return;
        }
        else cout << "Invalid Input Try Again";
    } while (betMoney > p->money);
    p->status = "bet";
    boardMoney += betMoney;
    haveBet = true;
    p->money -= betMoney;
}
Player::Player()
{
    cout << "Enter Your name\n";
    getline(cin, name);
}
void Player::drawCard(Deck &deck, int N)
{
    for (int i = 0; i < N; i++)
    {
        card.push_back(deck.allCardLeft.back());
        deck.allCardLeft.pop_back();
        deck.cardLeft = deck.allCardLeft.size(); // change number of card left in deck
    }
}
Deck::Deck()
{
    for (int i = 0; i < 13; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            allCardLeft.push_back(ranks[i] + suits[j]);
        }
    }
}
void Deck::shuffle()
{
    random_shuffle(allCardLeft.begin(), allCardLeft.end());
}
void Deck::showCardLeft()
{
    for (int i = 0; i < cardLeft; i++)
    {
        cout << allCardLeft[i] << " ";
    }
    cout << "\n";
}
int main()
{
    int num_player = 0;
    long long int moneyRef = 0;
    drawPic();                               // เดินเข้ามาในร้านเห็นป้ายหน้าร้าน
    recieveSimpleInformation(num_player, moneyRef);    // ถามข้อมูลพื้นฐานก่อนนำไปสร้าง BoardGame
    Player *pRef = new Player[num_player];          // สร้างผู้เล่นให้เท่ากับจำนวนคนจริงๆ // เอา Pointer ที่เก็บตัวแปร ประเภท Class มาเก็บที่อยู่ตัวแรกของ Array ที่เราได้จากการ Return ของ Dynamic Allocation แบบ Array
    Deck dRef;                               // เตรียมสำรับไพ่ที่ยังไม่ได้สับ
    PokerGame poker(pRef, dRef, num_player, moneyRef); // เข้ามานั่งในเกมโป๊กเกอร์พร้อมที่จะเล่นเกม
    poker.round1();
}