/*
 * game.cpp
 * CS11 Splendor
 */

#include "game.h"

//constructor
Game::Game(string filename, string play_nobles){
   //store gem colors for easy reference in drawing functions 
   colors[0] = "white";
   colors[1] = "red";
   colors[2] = "pink";
   colors[3] = "blue";
   colors[4] = "green";
   colors[5] = "gold";

   //include nobles or not
    if(play_nobles == "true"){
        this->play_nobles = true;
    }else{
        this->play_nobles = false;
    }

   //initialize the board
    for(int i = 0; i < BOARD_HEIGHT; i++){
        for(int j = 0; j < BOARD_WIDTH; j++){
            board[i][j] = ' ';
            board_color[i][j] = "white";
        }
    }     
    //initialize the card rows and columns
    for (int i = 0; i < CARD_ROWS; i++) {
        for (int j = 0; j < CARD_COLS; j++) {
            grid[i][j] = nullptr;
        }
    }
    //initialize the gem bank
    for (int i = 0; i < 5; i++) {
        gem_bank[i] = 4;
    }
    gem_bank[5] = 5;
    
    read_file(filename, &mines, &transport, &vendors); 
    setCards();
}

//destructor
Game::~Game() {
    for (int i = 0; i < mines.size(); i++) {
        delete mines.at(i);
    }
    for (int i = 0; i < transport.size(); i++) {
        delete transport.at(i);
    }
    for (int i = 0; i < vendors.size(); i++) {
        delete vendors.at(i);
    }
    for (int i = 0; i < discard.size(); i++) {
        delete discard.at(i);
    }
}

//main game loop
void Game::playGame(){
    bool game_over = false;
    bool player1_turn = true;

    while(!game_over){
        //reset the screen
        clearBoard();
        screen_clear();
        screen_home();
        //draw the board
        drawBoard();
        


        game_over = prompt_player(player1_turn, 0);
        while (totalGems(player1_turn) > 10) {
            returnGems(player1_turn);
        }
        if (winning(player1_turn)) {
            game_over = true;
        }
        //change turns
        player1_turn = !player1_turn;
        
    } 
    // ~Game();
    /* PRINT AN APPROPRIATE MESSAGE HERE WHEN THE GAME IS OVER */
}

//Get the gem color associated with the given index 
string Game::indexToColor(int index){
    return colors[index];
}

//draw all of the elements onto the game board
void Game::drawBoard(){
    int vpos = 0;
    int hpos = 1;

    //draw the card decks
    if(!vendors.empty()){
        drawDeck(vpos, hpos, "Vendors");
    }
    vpos += CARD_HEIGHT + 1;
    if(!transport.empty()){
        drawDeck(vpos, hpos, "Transport");
    }
    vpos += CARD_HEIGHT + 1;
    if(!mines.empty()){
        drawDeck(vpos, hpos, "Mines");
    }

    //draw the in play cards
    vpos = 0;
    hpos = CARD_WIDTH + 2;
    for(int i = 0; i < CARD_ROWS; i++){
        for(int j = 0; j < CARD_COLS; j++){
            if(grid[i][j] != nullptr){
                drawCard(vpos, hpos, grid[i][j]);
            }
            hpos += CARD_WIDTH + 1;
        }
        vpos += CARD_HEIGHT + 1;
        hpos = CARD_WIDTH + 2;
    }

    //draw the gems
    int gemOffset = ((CARD_WIDTH + 1) * 5) + 3;
    vpos = 1;
    hpos = gemOffset;
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 2; j++){
            int index = (i * 2) + j;
            if(gem_bank[index] > 0){
                board[vpos][hpos + 5] = gem_bank[index] + 48;
                board_color[vpos][hpos + 5] = indexToColor(index);
                drawGem(vpos + 1, hpos, indexToColor(index));
            }
            hpos += CARD_WIDTH;
        }
        vpos += CARD_HEIGHT + 1;
        hpos = gemOffset;
    }
    
    //draw players
    drawPlayer(vpos + 1, 1, &player_1);
    drawPlayer(vpos + 1, ((CARD_WIDTH + 1) * 4), &player_2);

    //draw nobles
    if(play_nobles){
        hpos = ((CARD_WIDTH + 1) * 7) + 2;
        vpos = 2;
        for(int i = 0; i < NUM_NOBLES; i++){
            if(nobles.at(i) != nullptr){
                drawNoble(vpos, hpos, nobles.at(i));
            }
            vpos += CARD_HEIGHT + 1;
        }
    }

    //print out the finished board
    for(int i = 0; i < BOARD_HEIGHT; i++){
        for(int j = 0; j < BOARD_WIDTH; j++){
            if(board_color[i][j] != "white"){
                screen_fg(board_color[i][j]);
            }
            cout << board[i][j];
            screen_fg("white");
        }
        cout << endl;
   } 
   cout << endl; 
}

//draws a noble at the specified position of the board
void Game::drawNoble(int vpos, int hpos, Card *c){
    string top    = "|\\  /*\\  /|";
    string second = "| \\/   \\/ |";
    string blank  = "N         N";
    string bottom = "~~~~~~~~~~~";

    //draw the basic card outline
    int length = top.length();
    for(int i = 0; i < length; i++){
        board[vpos][hpos + i] = top[i];
        board[vpos + 1][hpos + i] = second[i];

        for(int j = 2; j < 5; j++){
            board[vpos + j][hpos + i] = blank[i];
        }
        board[vpos + 5][hpos + i] = bottom[i];
    }

    //draw the prices
    vpos += 2;
    for(int i = 0; i < GEMS-1; i++){
        int price = c->getPrice(i);
        if(price > 0){
            board_color[vpos][hpos + 4] = indexToColor(i);
            board_color[vpos][hpos + 5] = indexToColor(i);
            board_color[vpos][hpos + 6] = indexToColor(i);
            board[vpos][hpos + 4] = '|';
            board[vpos][hpos + 5] = price + 48;
            board[vpos][hpos + 6] = '|';
            vpos++;
        }
    }
}

//draws a player at the specified position of the board
void Game::drawPlayer(int vpos, int hpos, Player *p){
    //player string
    string prestige = to_string(p->totalPrestige());
    string player_s;
    if(hpos < 10){
        player_s = " PLAYER 1: " + prestige + " prestige point(s)";
    }else{
        player_s = " PLAYER 2: " + prestige + " prestige point(s)";
    }
    int length = player_s.length();
    for(int i = 0; i < length; i++){
        board[vpos][hpos + i] = player_s[i];
    }

    //bonus string
    vpos++;
    string bonus_s = "Discounts: ";
    length = bonus_s.length();
    for(int i = 0; i < GEMS-1; i++){
        int bonus = p->getDiscount(i);
        bonus_s += "|" + to_string(bonus) + "| ";

        board_color[vpos][hpos + length] = indexToColor(i);
        board_color[vpos][hpos + length + 1] = indexToColor(i);
        board_color[vpos][hpos + length + 2] = indexToColor(i);

        length += 4;
        if(bonus >= 10){
            length += 1;
        }
    }
    for(int i = 0; i < length; i++){
        board[vpos][hpos + i] = bonus_s[i];
    }

    //gem string
    vpos++;
    string gems_s = "     Gems: ";
    length = gems_s.length();
    int different_gems = GEMS;
    for(int i = 0; i < different_gems; i++){
        int gems = p->getGemCount(i);
        gems_s += "(" + to_string(gems) + ") ";

        board_color[vpos][hpos + length] = indexToColor(i);
        board_color[vpos][hpos + length + 1] = indexToColor(i);
        board_color[vpos][hpos + length + 2] = indexToColor(i);

        length += 4;
    }
    for(int i = 0; i < length; i++){
        board[vpos][hpos + i] = gems_s[i];
    }

    //reserve string
    vpos++;
    string reserve_s = " Reserved:";
    length = reserve_s.length();
    for(int i = 0; i < length; i++){
        board[vpos][hpos + i] = reserve_s[i];
    }

    hpos += reserve_s.length();
    for(int i = 0; i < 3; i++){
        Card *c = p->getReserve(i);
        if(c == nullptr){
            drawDeck(vpos, hpos, "");
        }else{
            drawCard(vpos, hpos, c);
        }
        hpos += CARD_WIDTH + 1;
    }
}

//draws a card deck at the specified position of the board
void Game::drawDeck(int vpos, int hpos, string type){
    string top = "  _________  ";
    string second = " /         \\ ";
    string blank = "|           |";
    string bottom = " \\_________/ ";

    //draw the basic card outline
    int length = top.length();
    for(int i = 0; i < length; i++){
        board[vpos][hpos + i] = top[i];
        board[vpos + 1][hpos + i] = second[i];
        for(int j = 2; j < CARD_HEIGHT; j++){
            board[vpos + j][hpos + i] = blank[i];
        }
        board[vpos + CARD_HEIGHT][hpos + i] = bottom[i];
    }

    int start_pos = hpos + (blank.length() / 2) - (type.length() / 2);
    length = type.length();
    for(int i = 0; i < length; i++){
        board[vpos + (CARD_HEIGHT/2)][start_pos + i] = type[i];
    }
}

//draws a gem at the specified position of the board
void Game::drawGem(int vpos, int hpos, string color){
    string gem_image[6];
    gem_image[0] = "  _______  ";
    gem_image[1] = ".'_/_|_\\_'.";
    gem_image[2] = "\\`\\  |  /`/";
    gem_image[3] = " `\\  | //' ";
    gem_image[4] = "   `\\|/`   ";
    gem_image[5] = "     `     ";

    for(int i = 0; i < GEMS; i++){
        int length = gem_image[i].length();
        for(int j = 0; j < length; j++){
            board[vpos + i][hpos + j] = gem_image[i][j];
            board_color[vpos + i][hpos + j] = color;
        }
    }
}

//draws a card at the specified position of the board
void Game::drawCard(int vpos, int hpos, Card *c){
    string top = "  _________  ";
    string second = " /         \\ ";
    string blank = "|           |";
    string bottom = " \\_________/ ";

    //draw the basic card outline
    int length = top.length();
    for(int i = 0; i < length; i++){
        board[vpos][hpos + i] = top[i];

        if(i == 8 || i == 9){
            board_color[vpos + 1][hpos + i] = c->getGemColor();
        }
        board[vpos + 1][hpos + i] = second[i];

        for(int j = 2; j < CARD_HEIGHT; j++){
            board[vpos + j][hpos + i] = blank[i];
        }
        board[vpos + CARD_HEIGHT][hpos + i] = bottom[i];
    }

    //draw the prestige number and gem
    board[vpos + 2][hpos + 3] = c->getPrestige() + 48;
    board_color[vpos + 2][hpos + 8] = c->getGemColor();
    board_color[vpos + 2][hpos + 9] = c->getGemColor();
    board[vpos + 2][hpos + 8] = '\\';
    board[vpos + 2][hpos + 9] = '/';
    board_color[vpos + 1][hpos + 8] = c->getGemColor();
    board_color[vpos + 1][hpos + 9] = c->getGemColor();
    board[vpos + 1][hpos + 8] = '_';
    board[vpos + 1][hpos + 9] = '_';

    //draw the prices
    vpos += 4;
    for(int i = 0; i < GEMS-1; i++){
        int price = c->getPrice(i);
        if(price > 0){
            board_color[vpos][hpos + 2] = indexToColor(i);
            board_color[vpos][hpos + 3] = indexToColor(i);
            board_color[vpos][hpos + 4] = indexToColor(i);
            board[vpos][hpos + 2] = '(';
            board[vpos][hpos + 3] = price + 48;
            board[vpos][hpos + 4] = ')';
            vpos++;
        }
    }

}

//clear the board display
void Game::clearBoard(){
    for(int i = 0; i < BOARD_HEIGHT; i++){
        for(int j = 0; j < BOARD_WIDTH; j++){
            board[i][j] = ' ';
            board_color[i][j] = "white";
        }
   }  
}

// read_file
// Input: the filename with all the information, and three vector pointers,
//        one for each deck of cards 
// Description: creates an ifstream to read in the file, prints out an error
//              if it does not work, and then calls the read_deck method
//              on each type of card 
void Game::read_file(string filename, Vector *m, Vector *t, Vector *v) {
    ifstream infile(filename);
    if (infile.fail()) {
        cerr << "ERROR: Error opening file "; 
        exit(EXIT_FAILURE);
    }
    string read;
    getline(infile, read); 
    read_deck(infile, m, 40);
    read_deck(infile, t, 30);
    read_deck(infile, v, 20);
}

// read_deck
// input: the ifstream from the read_file function, a vector pointer, and 
//        the number of cards that are in that deck 
// description: reads over the first line of the file then reads each piece 
//              of info for each card, creates the new card, and then 
//              inserts it into the deck vecotr for the correct type of card 
void Game::read_deck(ifstream &infile, Vector *vec, int numCards) {
    string read;
    int prestige;
    string color;
    int price[5];

    for (int i = 0; i < numCards; i++) {
        infile >> read >> prestige >> color;
        for (int j = 0; j < 5; j++) {
            infile >> price[j];
        }
        Card *newCard = new Card(color, prestige, price);
        vec->push_back(newCard);
        //pop back for setting up game 
    }
}

// setCards
// Input: none
// description: draws the top four cards from each deck and puts in on the grid
void Game::setCards() {
    for (int i = 0; i < 4; i++) {
        grid[0][i] = vendors.back();
        discard.push_back(vendors.back());
        vendors.pop_back();

    }
    for (int i = 0; i < 4; i++) {
        grid[1][i] = transport.back();
        discard.push_back(transport.back());
        transport.pop_back();
    }
    for (int i = 0; i < 4; i++) {
        grid[2][i] = mines.back();
        discard.push_back(mines.back());
        mines.pop_back();
    }
}

//promptPlayer
//input: a bool if it is player1's turn and an integer that tells it if this is a
//       new turn or they are redoing a move due to something invalid
//description: prompts the player to enter a move and then calls the query function
//output: a false bool will be returned all the way through the methods to then be 
//        returned to continue to the game in playGame
bool Game::prompt_player(bool player_turn, int timesCalled) {
    Player *currentPlayer;
    if (timesCalled == 0) {
        if (player_turn) {
            cout << "PLAYER 1, enter your next move: ";
            currentPlayer = &player_1;
        }
        else {
            cout << "PLAYER 2, enter your next move: ";
            currentPlayer = &player_2;
        }
    }
    return (query(currentPlayer));
}

//query
//input: a player pointer to the player currently playing
//description: reads in the query from the player and calls the correct function 
//             to respond to the query
//output: a bool that will be returned to the playGame function to let it know 
//        if the game has ended
bool Game::query(Player *currentPlayer) {
    string move;
    cin >> move;
    if (move == "q") {
        return true;
    }
    else if (move == "p2") {
        return takeTwo(currentPlayer);
    }
    else if (move == "p3") {
        return takeThree(currentPlayer);
    }
    else if (move == "r") {
        return reserve(currentPlayer);
    }
    else if (move == "br") {
        return checkBuyReserve(currentPlayer);
    }
    else if (move == "b") {
        return checkBuyInput(currentPlayer);
    }
    else {
        cout << "Invalid move ";
        return query(currentPlayer);
    }
    return true;
}

//colorIndex
//input: a color
//description: takes in a color and returns its corresponding index in the 
//             colors array
//output: index in the colors array
int Game::colorIndex(string color) {
    for (int i = 0; i < 5; i++) {
        if (color == colors[i]) {
            return i;
        }
    }
    return -1;
}

//totalGems
//input: a bool of which player's turn it is
//description: adds up all of a player's gems
//output: number of total gems a player has 
int Game::totalGems(bool player_turn) {
    Player *player;
    if (player_turn) {
        player = &player_1;
    }
    else {
        player = &player_2;
    }
    int total = 0;
    for (int i = 0; i < 6; i++) {
        total += player->getGemCount(i); 
    }
    return total;
}

//returnGems
//input: a bool of which player's turn it is
//description: prompts player to return gems to bank if they have too many
//output: a bool if they now have less than 10 gems
bool Game::returnGems(bool player_turn) {
    clearBoard();
    screen_clear();
    screen_home();
    drawBoard();
    // while (totalGems(player_turn) > 10) {
    Player *curr_p;
    if (player_turn) {
        curr_p = &player_1;
        cout << "PLAYER 1, you have more than 10 gems! ";
    }
    else {
        curr_p = &player_2;
        cout << "PLAYER 2, you have more than 10 gems! ";
    }
    cout << "Name one gem you would like to return: ";
    string color;
    cin >> color;
    int index = colorIndex(color);
    // if (index == -1 || curr_p->getGemCount(index) < 1) {
    //     cout << "Invalid color. ";
    //     return returnGems(player_turn, 1);
    // }
    curr_p->setGemCount(-1, index);
    gem_bank[index]++;
    // }
    // clearBoard();
    // screen_clear();
    // screen_home();
    // drawBoard();
    return true;
}


//takeTwo
//input: a player pointer to the player currently playing
//description: checks if the requested move is valid and if it is, it 
//             executes the take two move 
//output: a bool that will be returned to the playGame function to let it know 
//        if the game has ended
bool Game::takeTwo(Player *currentPlayer) {
    string color;
    cin >> color;
    int index = colorIndex(color);
    if (gem_bank[index] == 4) {
        currentPlayer->setGemCount(2, index);
        gem_bank[index]-=2; 
        return false;
    }
    else {
        cout << "Invalid move (unavailable gems) - enter a valid move: ";
        return query(currentPlayer);
    }
    return true;
}

//takeThree
//input: a player pointer to the player currently playing
//description: checks if the requested move is valid and if it is, it 
//             executes the take three move 
//output: a bool that will be returned to the playGame function to let it know 
//        if the game has ended
bool Game::takeThree(Player *currentPlayer) {
    string choices[3];
    cin >> choices[0] >> choices[1] >> choices[2];
    if (choices[0] == choices[1] || choices[0] == choices[2] || 
    choices[1] == choices[2]) {
            cout << "Invalid move (duplicate color) - enter a valid move: ";
            return query(currentPlayer);
    }
    for (int i = 0; i < 3; i++){
        int index = colorIndex(choices[i]);
        if (index == -1) {
            cout << "Invalid move (too few colors) - enter a valid move: ";
            return query(currentPlayer);
        }
    }
    for (int i = 0; i < 3; i++) {
        int index = colorIndex(choices[i]);
        if (gem_bank[index] == 0) {
            cout << "Invalid move (unavailable gems) - enter a valid move: ";
            return query(currentPlayer);
        }
        else {
            gem_bank[index]--;
            currentPlayer->setGemCount(1, index);
        }
    }
    return false;
}
    
//reserve
//input: a player pointer to the player currently playing
//description: checks if the requested move is valid and if it is, it 
//             calls the doReserve function which executes the reserve
//             for the specifc deck on which it is called
//output: a bool that will be returned to the playGame function to let it know 
//        if the game has ended
bool Game::reserve(Player *currentPlayer) {
    string row;
    int col;
    cin >> row >> col;
    if (col < 5) {
        int c = col - 1;
        if (row == "m") {
            return doReserve(&mines, 2, c, currentPlayer);
        }
        else if (row == "t") {
            return doReserve(&transport, 1, c, currentPlayer);
        }
        else if (row == "v") {
            return doReserve(&vendors, 0, c, currentPlayer);
        }
        else {
            cout << "Invalid move (invalid card type) - enter a valid move: ";
            return query(currentPlayer);
        }
    }
    else {
        cout << "Invalid move (invalid card colomn) - enter a valid move: ";
        return query(currentPlayer);
    }
    return true;
}

//doReserve
//input: a player pointer to the player currently playing
//description: checks if the requested move is valid and if it is, it 
//             executes the reserve move by setting the first place in the player's
//             reserved array to the card they chose from the grid, adding 2 gold gem
//             to their gems, and removing both of those things from the grid
//output: a bool that will be returned to the playGame function to let it know 
//        if the game has ended
bool Game::doReserve(Vector *deck, int r, int c, Player *currentPlayer) {
    for (int i = 0; i < 3; i++) {
        if (currentPlayer->getReserve(i) == nullptr) {
            currentPlayer->setReserve(grid[r][c], i); 
            currentPlayer->setGemCount(1, 5);
            gem_bank[5]--;
            grid[r][c] = deck->back();
            discard.push_back(deck->back());
            deck->pop_back();
            return false;
        }
    }
    cout << "Invalid move (reserve is already full) - enter a valid move: ";
    return query(currentPlayer);
}

//checkBuyReserve
//input: a player pointer to the player currently playing
//description: checks if the player inputted currect input to buy a reserved card and 
//             if they have enough gems to buy it. if the move was valid it calls
//             the buyReserve funciton to execute the move
//output: a bool that will be returned to the playGame function to let it know 
//        if the game has ended
bool Game::checkBuyReserve(Player *currentPlayer) {
    int index = -1;
    cin >> index;
    int place = index - 1;
    if (index > 3 || currentPlayer->getReserve(place) == nullptr) {
        cout << "Invalid move (no card at that index) - enter a valid move: ";
        return query(currentPlayer);
    }
    else {
        int gold = 0;
        for (int i = 0; i < 5; i++) {
            int cardPrice = currentPlayer->getReserve(place)->getPrice(i);
            int pGems = currentPlayer->getGemCount(i) + currentPlayer->getDiscount(i);
            int dif = cardPrice - pGems;
            if (dif > 0) {
                gold += dif;
            }
        }
        if (currentPlayer->getGemCount(5) >= gold) {
            return buyReserve(currentPlayer, place, gold);
        }
        else {
            cout << "Invalid move (cannot afford card) - enter a valid move: ";
            return query(currentPlayer);
        }
    }
    return true;
}

//buyReserve
//input: a player pointer to the player currently playing
//description: executes the buy reserve move if it is valid. ad2ds one to the discout
//             at the index of the card color, returns gems to gem bank, and
//             puts a nullptr in the reserve spot that was just bought
//output: a bool that will be returned to the playGame function to let it know 
//        if the game has ended
bool Game::buyReserve(Player *currentPlayer, int place, int gold) {
    //adds discount to correct color
    string color = currentPlayer->getReserve(place)->getGemColor();
    int in = colorIndex(color);
    currentPlayer->setDiscount(in);

    //returns gems to gem bank
    for (int i = 0; i < 5; i++) {
        int price = currentPlayer->getReserve(place)->getPrice(i);
        // int pGems = currentPlayer->getGemCount(i) + currentPlayer->getDiscount(i);
        if (currentPlayer->getGemCount(i) > price) {
            currentPlayer->setGemCount(-price, i);
            gem_bank[i] = gem_bank[i] + price;
        }
        else {
            gem_bank[i] = gem_bank[i] + currentPlayer->getGemCount(i);
            currentPlayer->setGemCount(-currentPlayer->getGemCount(i), i);
        }
    }
    int prest = currentPlayer->getReserve(place)->getPrestige();
    currentPlayer->setPrestige(prest);
    gem_bank[5] += gold;
    currentPlayer->setGemCount(-gold, 5);
    currentPlayer->setReserve(nullptr, place);
    return false;
}

//checkBuyInput
//input: a player pointer to the player currently playing
//description: checks to see if the input provided by the user is correct
//output: a bool that will be returned to the playGame function to let it know 
//        if the game has ended
bool Game::checkBuyInput(Player *currentPlayer) {
    string row;
    int col;
    cin >> row >> col;
    if (col < 5) {
        int c = col - 1;
        if (row == "m") {
            return checkBuyCard(&mines, 2, c, currentPlayer);
        }
        else if (row == "t") {
            return checkBuyCard(&transport, 1, c, currentPlayer);
        }
        else if (row == "v") {
            return checkBuyCard(&vendors, 0, c, currentPlayer);
        }
        else {
            cout << "Invalid move (invalid card type) - enter a valid move: ";
            return query(currentPlayer);
        }
    }
    else {
        cout << "Invalid move (invalid card colomn) - enter a valid move: ";
        return query(currentPlayer);
    }
    return true;
}

//checkBuyCard
//input: a player pointer to the player currently playing, a pointer to the deck 
//       of the card the user wants to buy, and the colomn and row where the card is 
//       on the board
//description: checks to see if the move requested by the user is plausible,
//             and if it is, it calls the buyCard function to execute
//output: a bool that will be returned to the playGame function to let it know 
//        if the game has ended
bool Game::checkBuyCard(Vector *deck, int r, int c, Player *currentPlayer) {
    Card *card = grid[r][c];

    int gold = 0;
    for (int i = 0; i < 5; i++) {
        int cardPrice = card->getPrice(i);
        int pGems = currentPlayer->getGemCount(i) + currentPlayer->getDiscount(i);
        int dif = cardPrice - pGems;
        if (dif > 0) {
            gold += dif;
        }
    }
    if (currentPlayer->getGemCount(5) >= gold) {
        return buyCard(deck, r, c, currentPlayer, gold, card);
    }
    else {
        cout << "Invalid move (cannot afford card) - enter a valid move: ";
        return query(currentPlayer);
    }
}

//checkBuyCard
//input: a player pointer to the player currently playing, a pointer to the deck 
//       of the card the user wants to buy, the colomn and row where the card is 
//       on the board, a pointer to that card, and how much gold gems the user would
//       need to complete the buy
//description: executes the buyCard move by subtracting the correct number of gems from
//             the players gem count, adding them back to the gem bank, placing a new
//             card in that space on the grid, adding the correct number of prestige 
//             points to the player etc. 
//output: a bool that will be returned to the playGame function to let it know 
//        if the game has ended
bool Game::buyCard(Vector *deck, int r, int c, Player *currentPlayer, int gold, Card *card) {
    string color = card->getGemColor();
    int in = colorIndex(color);
    currentPlayer->setDiscount(in);

    //returns gems to gem bank
    for (int i = 0; i < 5; i++) {
        int price = card->getPrice(i);
        // int pGems = currentPlayer->getGemCount(i) + currentPlayer->getDiscount(i);
        if (currentPlayer->getGemCount(i) > price) {
            currentPlayer->setGemCount(-price, i);
            gem_bank[i] = gem_bank[i] + price;
        }
        else {
            gem_bank[i] = gem_bank[i] + currentPlayer->getGemCount(i);
            currentPlayer->setGemCount(-currentPlayer->getGemCount(i), i);
        }
    }
    gem_bank[5] += gold;
    currentPlayer->setGemCount(-gold, 5);
    int prest = card->getPrestige();
    currentPlayer->setPrestige(prest);
    grid[r][c] = deck->back();
    discard.push_back(deck->back());
    deck->pop_back();
    return false;
}

//winning
//input: a boolean of which player is playing
//description: checks to see if either player has more than 15 prestige points
//             to see if they are winning
//output: a bool that will be returned to the playGame function to let it know 
//        if the game has ended
bool Game::winning(bool player_turn) {
    Player *curr_p;
    Player *other;
    if (player_turn) {
        curr_p = &player_1;
        other = &player_2;
    }
    else {
        curr_p = &player_2;
        other = &player_1;
    }
    if (curr_p->totalPrestige() >= 15) {
        if (other->totalPrestige() >= 15) {
            cout << "You losers tied - BORING!!!";
            return true;
        }
        cout << "Congratulations ";
        if (player_turn) {
            cout << "PLAYER 1: ";
        }
        else {
            cout << "PLAYER 2: ";
        }
        cout << "YOU WIN!!!"; 
        return true;
    }
    return false;
}
