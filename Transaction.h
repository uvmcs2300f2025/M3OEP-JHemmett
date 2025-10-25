
#include <vector>
#include <string>
#include "Item.h"
#include "ItemIn.h"
#include <iostream>
#include "PaymentPortal.h"
#include "include/json.hpp"



#ifndef TRANSACTION_H
#define TRANSACTION_H


class Transaction {
private:
    vector<ItemIn> items;
    int totalCost;
    bool completed;
    int id = -2'
    int customerId;
    PaymentPortal* portal;


public:
  Transaction(PaymentPortal* portal, int customerId);
  Transaction(PaymentPortal* portal);

  // Transaction(PaymentPortal* portal, int customerId, int id, vector<ItemIn>& items, int totalCost);
  // Transaction(PaymentPortal* portal, int id, vector<ItemIn>& items, int totalCost);

  // If an uncompleted transaction is deleted, items are restocked
  ~Transaction();

  //Getters and setters
  vector<ItemIn> getTransaction() const;
  int getTotalCost() const;

  bool addItem(Item& item, int quantity);
  bool addItem(Item& item);
  bool removeItem(Item& item, int quantity);
  bool removeItem(Item& item);
  bool setItems(vector<ItemIn>& items);

  int getCustomerId() const;
  bool isCompleted() const;
  void completeTransaction();

  int getId() const;

  // string archive() const;

  string toJson() const;
  friend std::ostream& operator<<(std::ostream& os, const Transaction& obj);

  bool setId(int id);

};
void to_json(nlohmann::json& j, const Transaction& t);
#endif //TRANSACTION_H
