// card.h
// Written by: Ariella Mann {amann04}
// Purpose: Defines the card class

#ifndef CARD_H
#define CARD_H
#include <iostream>
#include <fstream>
#include <string>
#include "termfuncs.h"
using namespace std;

class Card {
    public:
    //constructor
    Card(string color, int prestigePts, int price[5]);

    //getters
    int getPrice(int index);
    string getGemColor();
    int getPrestige();

    //setters
    void setPrice(int value, int index);
    void setGemColor(string value);
    void setPrestige(int value);

    private:
    int price[5]; // white, red, pink, blue, and green
    string gemColor;
    int prestige;
};

#endif
