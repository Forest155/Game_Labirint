#include "game.h"

#include <iostream>

void printHello(void);
char mainMenu(void);
void playNewGame(void);


int main()
{
    printHello();

    while (true)
    {
        switch (mainMenu()) {
        case '1':
            playNewGame();
            break;
        case '0':
            std::cout << "Bye!" << std::endl;
            std::cin.ignore();
            std::cin.get();
            return 0;
            break;
        default:
            std::cout << "wrong menu point!" << std::endl << std::endl;
            break;
        }
    }

    system("pause");
    return 0;
}

void playNewGame()
{

    std::cout << "Starting new game, \n\tplease input labyrinth height: ";
    int height = inputUIntCheck();
    std::cout << "\n\tplease input labyrinth width: ";
    int width = inputUIntCheck();
    Game game(height, width);
    std::cout << "Generating the labitinth...." << std::endl;
    game.generateLabirint();
    std::cout << "Done!" << std::endl << std::endl;

    game.start();

    if (game.isChestOpened() && game.isPlayerLeaved())
        std::cout << "You are WIN! Congrutilations for you, stranger!" << std::endl;
    else
        std::cout << "You are loose... but you may try again!" << std::endl;

}

char mainMenu()
{
    std::cout << "\tGame Menu" << std::endl << std::endl
        << "1. New Game" << std::endl
        << "0. Exit" << std::endl << std::endl
        << "game> ";
    char ret;
    std::cin >> ret;
    return ret;
}

void printHello()
{
    setlocale(LC_ALL, "");
    std::cout << R"(
   _____  _______ .___      .    __    _   ___   .____  .___
  (      '   /    /   \    /|    |\   |  .'   \  /      /   \
   `--.      |    |__-'   /  \   | \  |  |       |__.   |__-'
      |      |    |  \   /---'\  |  \ |  |    _  |      |  \
 \___.'      /    /   \,'      \ |   \|   `.___| /----/ /   \
 
                 _ __    _
                 | |\   |
                 | | \  |
                 | |  \ |
                 / |   \|
 
 .         .    ____   _ .___  _ __    _  _______ __  __
 /        /|    /   \  | /   \ | |\   |  '   /    |   |
 |       /  \   |,_-<  | |__-' | | \  |      |    |___|
 |      /---'\  |    ` | |  \  | |  \ |      |    |   |
 /---/,'      \ `----' / /   \ / |   \|      /    /   /
 
                 )";
    std::cout << "(THE GAME)" << std::endl;
}