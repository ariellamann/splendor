#ifndef PLAYER_H
#define PLAYER_H
#include <iostream>
#include <fstream>
#include <string>
#include "termfuncs.h"
#include "card.h"
using namespace std;

class Player {
    public:
    //constructor 
    Player();

    //getters
    int getDiscount(int index);
    int totalPrestige();
    int getGemCount(int index);
    Card *getReserve(int index);
    
    //setters
    void setDiscount(int index);
    void setPrestige(int value);
    void setGemCount(int value, int index);
    void setReserve (Card *wanted, int index);

    private:
    int discount[5];
    int prestige;
    int gems[6];
    Card *reserved[3];
};

#endif
