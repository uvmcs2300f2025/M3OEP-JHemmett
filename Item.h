//
// Created by gresh on 9/26/2025.
//
#include <string>
#include <iostream>
#include "include/json.hpp"


#ifndef ITEM_H
#define ITEM_H

/*
  Used as the base for all items in the store.
  The methods and variables here are for all items of a certain type, E.G. If you subtract 1 banana the store has one less banana in stock.
  I was debating if I should create a class for each item type, as this would allow for easier tracking of quantities, but I decided this was better as I could keep the number of files reasonable and create and remove items at runtime instead of having to write a whole new class.
 */
using std::string;

class Item {
private:
  int id = -2;
  string name;
  int price;
  int quantity;
  int holdQuantity;
  friend std::ostream& operator<<(std::ostream& os, const Item& obj);


public:
  Item(int id, string name);
  Item(int id, string name, int price);
  Item(int id, string name, int price, int quantity);
  Item(string name, int price, int quantity);

  // Getters and setters.
  string getName() const;
  void setName(string name);

  int getId() const;
  void setId(int id);

  int getPrice() const;
  void setPrice(int price);

  int getQuantity() const;
  void setQuantity(int quantity);
  bool changeQuantity(int quantity);

  int getHoldQuantity() const;
  void setHoldQuantity(int holdQuantity);
  void changeHoldQuantity(int holdQuantity);

  int getAvailableQuantity() const;


  bool operator==(const Item& other) const;
};

void to_json(nlohmann::json& j, const Item& t);

#endif //ITEM_H
