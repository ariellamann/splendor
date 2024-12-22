// player.cpp
// Written by: Ariella Mann {amann04}
// Purpose: Defines the player class

#include "player.h"

//constructor 
Player::Player() {
    prestige = 0;
    for (int i = 0; i < 5; i++) {
        discount[i] = 0;
    }
    for (int i = 0; i < 6; i++) {
        gems[i] = 0;
    }
    for (int i = 0; i < 3; i++) {
        reserved[i] = nullptr;
    }
}

//getters 

// getDicount
// Input: index we want the discount for
// Description: gets the discount for a specifc color 
// Output: an int of the discount at that index 
int Player::getDiscount(int index) {
    return discount[index];
}

// totalPrestige
// Input: none
// Description: returns the prestige of a player
// Output: int of prestige 
int Player::totalPrestige() {
    return prestige;
}

// getGemCount
// Input: index for where you want the gem count from
// Description: takes in an index refering to a color and returns the number
//              of gems the player has in that color
// Output: int of number of gems at a specific color
int Player::getGemCount(int index) {
    return gems[index];
}

// getReserve
// Input: index for where you want the reserved card from
// Description: takes in an index refering to a reserve spot and returns the number
//              of gems the player has in that color
// Output: card pointer to the reserved card
Card *Player::getReserve(int index) {
    return reserved[index];
}

//setters

// setDiscount 
// Input: index for wanted discount
// Description: adds one to the discount for a specific colors index
void Player::setDiscount(int index) {
    discount[index]++;
}

// setPrestige
// Input: the prestige from a card that the player just bought
// Description: adds the value to the player's prestige points
void Player::setPrestige(int value) {
    prestige += value;
}

// setGemCount
// Input: number of gems and index for the corresponding color
// Description: adds the value of gems to that index in the gems array
void Player::setGemCount(int value, int index){
    gems[index] += value;
}

// setReserve
// Input: card pointer to the card wanted and the index where it should go
// Description: sets the card to that index
void Player::setReserve (Card *wanted, int index) {
    reserved[index] = wanted;
}

//testing 
// int main(int argc, char *argv[]) {

//     Player myplayer = Player();
//     cout << myplayer.getDiscount(0) << endl;
//     cout << myplayer.totalPrestige() << endl;
//     cout << myplayer.getGemCount(0) << endl;
//     myplayer.setDiscount(4, 0);
//     myplayer.setPrestige(87);
//     myplayer.setGemCount(7988, 0);
//     cout << myplayer.getDiscount(0) << endl;
//     cout << myplayer.totalPrestige() << endl;
//     cout << myplayer.getGemCount(0) << endl;
// }