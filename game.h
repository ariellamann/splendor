/*
 * game.h
 * CS11 Splendor
 */

#ifndef GAME_H
#define GAME_H
#include <iostream>
#include <fstream>
#include <string>
#include "termfuncs.h"
#include "card.h"
#include "player.h"
#include "vector.h"
using namespace std;

class Game {
  public:
    //constructor
    Game(string filename, string play_nobles);
    //destructor
    ~Game();

    //main gameplay
    void playGame();

    //utility functions
    string indexToColor(int index);

  private:
    //drawing functions
    void drawBoard();
    void drawNoble(int vpos, int hpos, Card *c);
    void drawPlayer(int vpos, int hpos, Player *p);
    void drawDeck(int vpos, int hpos, string type);
    void drawCard(int vpos, int hpos, Card *c);
    void drawGem(int vpos, int hpos, string color);
    void clearBoard();
    void read_file(string filename, Vector *m, Vector *t, Vector *v);
    void read_deck(ifstream &infile, Vector *vec, int numCards);
    
    void setCards();
    bool prompt_player(bool player_turn, int timesCalled);
    bool query(Player *currentPlayer);
    int colorIndex(string color);
    bool takeTwo(Player *currentPlayer);
    bool takeThree(Player *currentPlayer);
    bool reserve (Player *currentPlayer);
    bool doReserve(Vector *deck, int r, int c, Player *currentPlayer);
    bool checkBuyReserve(Player *currentPlayer);
    bool buyReserve(Player *currentPlayer, int place, int gold);
    bool checkBuyInput(Player *currentPlayer);
    bool checkBuyCard(Vector *deck, int r, int c, Player *currentPlayer);
    bool buyCard(Vector *deck, int r, int c, Player *currentPlayer, int gold, Card *card);
    int totalGems(bool player_turn);
    bool returnGems(bool player_turn);
    bool winning(bool player_turn);

    static const int BOARD_HEIGHT = 41;
    static const int BOARD_WIDTH = 120;
    static const int CARD_WIDTH = 13;
    static const int CARD_HEIGHT = 8;
    static const int CARD_ROWS = 3;
    static const int CARD_COLS = 4;
    static const int NUM_NOBLES = 3;
    static const int GEMS = 6;
    static const int NUM_M = 40;
    static const int NUM_T = 30;
    static const int NUM_V = 20;

    //nobles activated true/false
    bool play_nobles;

    //cards in play
    Card *grid[CARD_ROWS][CARD_COLS];

    //display
    char board[BOARD_HEIGHT][BOARD_WIDTH];
    string board_color[BOARD_HEIGHT][BOARD_WIDTH];

    Vector vendors;
    Vector transport; 
    Vector mines;
    Vector nobles;
    Vector discard;

    Player player_1;
    Player player_2;

    int gem_bank[GEMS];

    //Gem colors in the game
    string colors[GEMS];
};

#endif
