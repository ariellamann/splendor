// card.cpp
// Written by: Ariella Mann {amann04}
// Purpose: Implements the card class

#include "card.h"

// Card constructor
// Input: color of the gem that the card gives a discount for, 
//        the prestige points the card has, and a pointer to the array
//        that holds the price of the card for each gem color. 
// output: card object
Card::Card(string color, int prestigePts, int prices[5]) {
    gemColor = color;
    prestige = prestigePts;
    for (int i = 0; i < 5; i++) {
        price[i] = prices[i];
    }
}

//getters

// getPrice
// Input: index of gem color
// Description: gets the price of a card for a specified color
// Output: the price of a card for one color  
// Order of colors in array --> white, red, pink, blue, and green
int Card::getPrice(int index) {
    return price[index];
}

// getGemColor
// Input: none
// Description: gets the color of the gem that the card
// discounts 
// Output: the color of the gem 
string Card::getGemColor() {
    return gemColor;
}

// getPrestige
// Input: none
// Description: gets the number of prestige points that a card
// is worth
// Output: number of prestige points
int Card::getPrestige() {
    return prestige;
}

// setters 

// setPrice
// Input: the value the we want to set and the index where we 
//        want to set it 
// Description: sets the value give to the price at the index given 
void Card::setPrice(int value, int index) {
    price[index] = value;
}

// setGemColor
// Input: the color we want the gem to be set to
// Description: sets gemColor to the given color
void Card::setGemColor(string value) {
    gemColor = value;
}

// setPrestige
// Input: prestige value
// Description: sets prestige to the given value 
void Card::setPrestige(int value) {
    prestige = value;
}

//testing 
// int main(int argc, char *argv[]) {

//     int arr[] = {0, 0, 4, 5, 0};
//     Card mycard= Card("blue", 5, arr);
//     cout << mycard.getGemColor() << endl;
//     cout << mycard.getPrestige() << endl;
//     cout << mycard.getPrice(2) << endl;
//     mycard.setGemColor("red");
//     mycard.setPrestige(856);
//     mycard.setPrice(9, 1);
//     cout << mycard.getGemColor() << endl;
//     cout << mycard.getPrestige() << endl;
//     cout << mycard.getPrice(1) << endl;
    
//     return 0;
// }
