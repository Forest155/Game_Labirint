#include "game.h"

#include <iostream>
#include <ctime>
#include <cstdlib>


int inputIntCheck(void)
{
    int ret;
    std::cin >> ret;
    while (std::cin.fail() || std::cin.peek() != '\n')
    {
        std::cin.clear();
        std::cin.ignore(32768, '\n');
        std::cout << "wrong input (it must be integer number), repeat please: ";
        std::cin >> ret;
    }
    return ret;
}

int inputUIntCheck(void)
{
    int ret;
    while ((ret = inputIntCheck()) < 0)
        std::cout << "wrong input (it must be positive number), repeat please: ";
    return ret;
}

int Room::findItem(const std::string& name)
{
    for (unsigned i = 0; i < mItems.size(); ++i)
    {
        if (mItems[i]->name() == name)
            return i;
    }

    return -1;
}

void Game::generateLabirint()
{
    if (!mLabirint.size())
        return;

    srand(time(0));
    std::pair<int, int> basePoint{ rand() % mLabirint.size(), rand() % mLabirint[0].size() };

    mPlayerCurrentRoom = &mLabirint[basePoint.first][basePoint.second];
    Room* labBuilder = mPlayerCurrentRoom;

    int steps = (mLabirint.size() * mLabirint[0].size());

    mKeyPlaced = mChestPlaced = mChestOpened = false;
    int chanceForPlacingChest = 1;
    int chanceForPlacingKey = 1;

    int rooms = 0;
    while (steps)
    {
        char dirs[4] = { 'N', 'S', 'W', 'E' };
        char nextDir = dirs[rand() % 4];

        if (!labBuilder->isValidDirestrion(nextDir))
        {
            Room* nextRoom = getNextRoom(basePoint, nextDir);

            if (nextRoom)
            {
                if (!mChestPlaced)
                {
                    if (rand() % 101 <= chanceForPlacingChest)
                    {
                        nextRoom->putItem(new Chest);
                        mChestPlaced = true;
                    }
                    else
                        chanceForPlacingChest += 100 - steps % 100;
                }

                if (!mKeyPlaced)
                {
                    if (rand() % 101 <= chanceForPlacingChest)
                    {
                        nextRoom->putItem(new KeyForChest);
                        mKeyPlaced = true;
                    }
                    else
                        chanceForPlacingKey += 100 - steps % 100;
                }

                int itemsForRoom = rand() % 3;
                while (--itemsForRoom + 1)
                    nextRoom->putItem(new Just);

                labBuilder->makeDoor(nextDir, *nextRoom);
                labBuilder = nextRoom;
                ++rooms;
            }
        }
        --steps;
    }
    mPlayerStamina = rooms * 5;
    std::cout << "was generated " << rooms << " rooms" << std::endl;
    if (!mKeyPlaced || !mChestPlaced)
        std::cout << "You loose, sorry..." << std::endl;
}

void Game::start()
{
    std::cout << "Wellcome in the labirinth, stranger!" << std::endl;
    bool breakGame = false;
    while (!breakGame && mPlayerStamina && !mChestOpened)
    {
        std::cout << "\tIn this room you see: " << mPlayerCurrentRoom->listItems() << std::endl
            << "\tand doors to: " << mPlayerCurrentRoom->doors() << std::endl;
        std::cout << "Make you choise, stranger:" << std::endl << std::endl;

        switch (choise()) {
        case '1':
            goToDoor();
            break;
        case '2':
            pickUp();
            break;
        case '3':
            dropOrPut();
            break;
        case '4':
            std::cout << " Your items is: " << inventaryLookUp() << std::endl;
            break;
        case '5':
            tryToOpenChest();
            break;
        case '0':
            std::cout << "HA-HA! Yellow-Belly!" << std::endl;
            return;
            break;
        }
    }
}

char Game::choise()
{
    std::cout << "1. Go to door" << std::endl;
    std::cout << "2. Pick up item" << std::endl;
    std::cout << "3. Drop/put your item" << std::endl;
    std::cout << "4. Lookup in inventary" << std::endl;
    std::cout << "5. Try to open the magic chest" << std::endl;
    std::cout << "0. Run away from labirinth" << std::endl << std::endl;
    std::cout << "choise> ";
    char ret;
    std::cin >> ret;
    return ret;
}

void Game::goToDoor()
{
    std::cout << "\tWhat direction you wish? ";
    char dir;
    std::cin >> dir;
    if (mPlayerCurrentRoom->isValidDirestrion(dir))
        mPlayerCurrentRoom = mPlayerCurrentRoom->goThrough(dir);
    else
        std::cout << "A-a! It's not valid direction (door to " << dir << " is not exist)!" << std::endl;

    --mPlayerStamina;
}

void Game::pickUp()
{
    std::cout << "\tgive a name of item in this room to pick it's up: ";
    std::string name;
    std::cin.ignore();
    getline(std::cin, name);
    int index = mPlayerCurrentRoom->findItem(name);
    if (index > -1)
    {
        mPlayerInventar.push_back(mPlayerCurrentRoom->takeItem(index));
        std::cout << "now " << (*mPlayerInventar.rbegin())->name() << " is yours" << std::endl;
    }
    else
        std::cout << "no " << name << " in this room, keep searching..." << std::endl;

    --mPlayerStamina;
}

void Game::dropOrPut()
{
    std::cout << " Items in your inventary:" << std::endl;

    for (unsigned i = 0; i < mPlayerInventar.size(); ++i)
        std::cout << " " << i + 1 << ". " << mPlayerInventar[i]->name() << std::endl;
    std::cout << std::endl << "0. Abort" << std::endl << std::endl;
    std::cout << " choose what's item you want leave in this room: ";

    unsigned itemNumber = inputUIntCheck();

    if (itemNumber > 0)
    {
        mPlayerCurrentRoom->putItem(mPlayerInventar[itemNumber - 1]);
        mPlayerInventar.erase(mPlayerInventar.begin() + itemNumber - 1);
    }

    --mPlayerStamina;
}

std::string Game::inventaryLookUp()
{
    std::string ret = "[ ";
    for (unsigned i = 0; i < mPlayerInventar.size(); ++i)
        ret.append(mPlayerInventar[i]->name() + (i == mPlayerInventar.size() - 1 ? " " : "; "));
    ret.append("]"); return ret;
}

void Game::tryToOpenChest()
{
    for (auto inv : mPlayerInventar)
    {
        if (dynamic_cast<KeyForChest*>(inv))
        {
            for (auto its : mPlayerCurrentRoom->items())
            {
                if (dynamic_cast<Chest*>(its))
                {
                    mChestOpened = true;
                    return;
                }
            }
        }
    }
    --mPlayerStamina;
    std::cout << "No-No! You must find the key and room with chest before..." << std::endl;
}

Room* Game::getNextRoom(std::pair<int, int>& point, char dir)
{
    auto p = point;
    switch (dir)
    {
    case 'N':
        p.first--;
        break;
    case 'S':
        p.first++;
        break;
    case 'W':
        p.second--;
        break;
    case 'E':
        p.second++;
        break;
    default:
        p.first = -1;
        p.second = -1;
    }

    if (p.first < 0 || p.first >= mLabirint.size() || p.second < 0 || p.second >= mLabirint[0].size())
        return nullptr;

    point = p;
    return &mLabirint[point.first][point.second];
}