#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <string>
using namespace std;
bool findFreq(vector<std::pair<int, char>> hand, int &maxVar, int &max2ndPairVar, int num)
{
    std::reverse(hand.begin(), hand.end());
    unordered_map<int, int> freqRank;
    bool found = false;
    for (const auto &card : hand)
    {
        if (freqRank.find(card.first) == freqRank.end())
        {
            freqRank[card.first] = 1;
        }
        else
        {
            freqRank[card.first]++;
        }
    }

    for (const auto &pair : freqRank)
    {
        if (pair.second == num)
        {
            maxVar = pair.first;
            found = true;
        }
        if (pair.second == 2 && pair.first != maxVar && pair.first > max2ndPairVar) // ต้อง != maxVar เนื่องจาก ถ้า TwoPair เจอ TwoPair && เอาค่ามากสุดเผื่อมีซ้ำ 2 อีก
        {
            max2ndPairVar = pair.first;
        }
    }
    if (found)
        return true;
    return false;
}
void convertToPairVector(vector<std::pair<int, char>> &split, vector<string> &combined)
{
    split.resize(combined.size());
    for (size_t i = 0; i < combined.size(); i++)
    {
        if (combined[i][0] == 'A')
        {
            combined[i].replace(0, 1, "14");
        }
        else if (combined[i][0] == 'K')
        {
            combined[i].replace(0, 1, "13");
        }
        else if (combined[i][0] == 'Q')
        {
            combined[i].replace(0, 1, "12");
        }
        else if (combined[i][0] == 'J')
        {
            combined[i].replace(0, 1, "11");
        }
        else if (combined[i][0] == 'T')
        {
            combined[i].replace(0, 1, "10");
        }
    }
    for (size_t i = 0; i < combined.size(); i++)
    {
        split[i].first = stoi(combined[i].substr(0, combined[i].length() - 1));
        split[i].second = combined[i].back();
    }
    for (size_t i = 0; i < combined.size(); i++)
    {
        cout << split[i].first << split[i].second << "\n";
    }
}

bool hasRoyalFlush(vector<std::pair<int, char>> hand)
{
    int cntAQKJT = 0;
    char suit;
    for (size_t i = 0; i < hand.size(); i++)
    {
        if ((hand[i].first == 14 || hand[i].first == 13 || hand[i].first == 12 || hand[i].first == 11 || hand[i].first == 10))
        {
            if (cntAQKJT == 0)
                suit = hand[i].second;
            if (hand[i].second == suit)
                cntAQKJT++;
        }
    }
    if (cntAQKJT == 5)
        return true;
    else
        return false;
}
bool hasStraight(vector<std::pair<int, char>> hand, int &maxVar)
{
    for (size_t i = hand.size() - 1; i > 0; i--)
    {
        if (i - 3 >= 0 && (hand[i].first - hand[i - 1].first) == 1 && (hand[i].first - hand[i - 2].first) == 2 && (hand[i].first - hand[i - 3].first) == 3)
        {
            maxVar = hand[i].first;
            return true;
        }
    }
    return false;
}
bool hasFlush(vector<std::pair<int, char>> hand, int &maxVar)
{
    
}
bool hasStraightFlush(vector<std::pair<int, char>> hand, int &maxVar)
{
    if (hasStraight(hand, maxVar) && hasFlush(hand, maxVar))
        return true;
    else
        return false;
}
bool hasFourOfKind(vector<std::pair<int, char>> hand, int &maxVar, int &max2ndPairVar)
{
    return findFreq(hand, maxVar, max2ndPairVar, 4);
}
bool hasTreeOfKind(vector<std::pair<int, char>> hand, int &maxVar, int &max2ndPairVar)
{
    return findFreq(hand, maxVar, max2ndPairVar, 3);
}
bool hasTwoPair(vector<std::pair<int, char>> hand, int &maxVar, int &max2ndPairVar)
{

    return findFreq(hand, maxVar, max2ndPairVar, 2);
}
bool hasFullHouse(vector<std::pair<int, char>> hand, int &maxVar, int &max2ndPairVar)
{
    if (hasTwoPair(hand, maxVar, max2ndPairVar) && hasTreeOfKind(hand, maxVar, max2ndPairVar) && max2ndPairVar != 0)
        return true;
    else
        return false;
}

void hasHighCard(vector<std::pair<int, char>> hand, int &maxVar)
{
    for (size_t i = 0; i < hand.size(); i++)
    {
        if (i == 0 || hand[i].first > maxVar)
            maxVar = hand[i].first;
    }
}
// bool hasFullhouse()
void checkHand(const vector<string> &h, const vector<string> &b)
{ // Check กรณีดังต่อไปนี้ 2 5 6 7 ใบ
    std::pair<string, double> rankOfHand;
    vector<std::pair<int, char>> split;
    int maxVar = 0;
    int max2ndPairVar = 0;
    char maxChar;
    vector<string> combined(h.begin(), h.end());
    combined.insert(combined.end(), b.begin(), b.end());
    sort(combined.begin(), combined.end(), [](const string &a, const string &b)
         {
        string ranks = "23456789TJQKA";
        return ranks.find(a[0]) < ranks.find(b[0]); });
    convertToPairVector(split, combined);
    if (split.size() >= 5)
    {
        if (hasRoyalFlush(split))
            cout << "RoyalFlush!!";
        else if (hasStraightFlush(split, maxVar))
            cout << "StraightFlush";
        else if (hasFourOfKind(split, maxVar, max2ndPairVar))
            cout << "FourOfkind";
        else if (hasFullHouse(split, maxVar, max2ndPairVar))
            cout << "FullHouse";
        else if (hasFlush(split, maxVar))
            cout << "Flush";
        else if (hasStraight(split, maxVar))
            cout << "hasStraight";
        else if (hasTreeOfKind(split, maxVar, max2ndPairVar))
            cout << "ThreeOfKind";
    }
    if (rankOfHand.first == "")
    {
        // if (hasTwoPair(split, maxVar, max2ndPairVar))
        //     cout << "TwoPair";
        // else
        // {
        //     hasHighCard(split, maxVar);
        //     cout << "HighCard";
        // }
    }
    cout << maxVar << "\n";
    cout << max2ndPairVar;
}
int main()
{
    // Create a vector of poker cards
    vector<string> hand = {"Q\3", "2\3", "9\3", "3\5"};
    vector<string> board = {"5\5", "9\3", "3\3"};
    // Sort the hand by rank
    checkHand(hand, board);
    return 0;
}
