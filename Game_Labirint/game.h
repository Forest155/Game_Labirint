#ifndef GAME_H
#define GAME_H

#include "items.h"

#include <string>
#include <map>
#include <vector>

int inputIntCheck(void);
int inputUIntCheck(void);

class Room
{
    Room* mDoors[4];
    std::vector<Item*> mItems;
public:
    Room() { for (auto& d : mDoors) d = nullptr; } 
    ~Room(void) { for (auto i : mItems) delete i; }

    void makeDoor(char direction, Room& next)
    {
        int dir = getDirection(direction);
        if (dir < 0) return;
        mDoors[dir] = &next;
        next.mDoors[getDirection(reversDirection(direction))] = this;
    }

    bool isValidDirestrion(char dir) { return mDoors[getDirection(dir)]; }

    Room* goThrough(char dir) { return mDoors[getDirection(dir)]; }
    void putItem(Item* item) { mItems.push_back(item); }
    Item* takeItem(int i) { Item* ret = mItems[i]; mItems.erase(mItems.begin() + i); return ret; }

    int findItem(const std::string&);

    std::string doors(void) { std::string ret = "{ "; for (auto& dir : { "N ", "S ", "W ", "E " }) ret.append(isValidDirestrion(dir[0]) ? dir : ""); ret.append("}"); return ret; }
    std::string listItems(void) { std::string ret = "[ "; for (unsigned i = 0; i < mItems.size(); ++i) { ret.append(mItems[i]->name() + (i == mItems.size() - 1 ? " " : "; ")); }; ret.append("]"); return ret; }
    const std::vector<Item*>& items(void) const { return mItems; }

private:
    int getDirection(char dir) { switch (dir) { case 'N': return 0; case 'S': return 1; case 'W': return 2; case 'E': return 3; default: return -1; } }
    char reversDirection(char dir) { switch (dir) { case 'N': return 'S'; case 'S': return 'N'; case 'W': return 'E'; case 'E': return 'W'; default: return -1; } }
};

class Game
{
    using Labyrinth = std::vector<std::vector<Room>>;
    Labyrinth mLabirint;
    Room* mPlayerCurrentRoom;
    std::vector<Item*> mPlayerInventar;
    bool mKeyPlaced, mChestPlaced, mChestOpened;
    int mPlayerStamina;
public:
    Game(int rows, int cols) { mLabirint.resize(rows); for (auto& col : mLabirint) col.resize(cols); }

    void generateLabirint(void);
    void start(void);
    char choise(void);
    void goToDoor(void);
    void pickUp(void);
    void dropOrPut(void);
    std::string inventaryLookUp(void);
    void tryToOpenChest(void);

    bool isChestOpened(void) { return mChestOpened; }
    bool isPlayerLeaved(void) { return mPlayerStamina; }

private:
    Room* getNextRoom(std::pair<int, int>& point, char dir);
};

#endif 
