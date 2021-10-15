#ifndef ITEMS_H 
#define ITEMS_H 
#include <string>
class Item
{
public:
    virtual std::string name(void) const = 0;
    virtual ~Item(void) { } 
};

class Chest : public Item
{
public:
    virtual std::string name(void) const override { return "chest"; } 
};

class KeyForChest : public Item
{
public:
    virtual std::string name(void) const override { return "key"; }
};

class Just : public Item
{
    std::string mName;
public:
    Just(const std::string& name = std::string("trash")) : mName(name) { }
    virtual std::string name(void) const override { return mName; }
};


#endif

