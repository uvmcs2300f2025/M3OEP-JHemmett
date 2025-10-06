#include <string>
#include "Item.h"
#include <iostream>
using namespace std;
#ifndef ITEMIN_H
#define ITEMIN_H

/*
  Used as the base for items in a transaction.
  The methods and variables here are just per transaction, E.G. If you subtract 1 banana the transaction has 1 less banana.
 */
struct ItemIn
{
    Item* item;
    int purchasePrice = 0;
    int purchaseQuantity = 0;

    ItemIn(Item* item) : item(item) {}

    bool operator==(const ItemIn& other) const {
        return (item->getId() == other.item->getId());
    }

};
#endif //ITEMIN_H
