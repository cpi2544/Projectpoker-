#include "Project.h"
using namespace std;

int main()
{
    int num_player = 4;
    long long int moneyRef = 1000; // Money in game
    int mandatory_betRef = 10;
    srand(time(NULL));
    drawPic(); // เดินเข้ามาในร้านเห็นป้ายหน้าร้าน
    // recieveSimpleInformation(num_player, moneyRef, mandatory_betRef); // ถามข้อมูลพื้นฐานก่อนนำไปสร้าง BoardGame
    Deck dRef;                                                     // เตรียมสำรับไพ่ที่ยังไม่ได้สับ
    PokerGame poker(dRef, num_player, moneyRef, mandatory_betRef); // เข้ามานั่งในเกมโป๊กเกอร์พร้อมที่จะเล่นเกม

    poker.preflop();
    // poker.turn();
}