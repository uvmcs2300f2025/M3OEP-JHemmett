
#include <vector>
#include <string>
#include "Item.h"
#include "ItemIn.h"
#include <iostream>
#include "PaymentPortal.h"
#include "include/json.hpp"



#include "customer.h"
#ifndef TRANSACTION_H
#define TRANSACTION_H


class Transaction {
private:
    vector<ItemIn> items;
    int totalCost;
    bool completed;
    int id;
    Customer* customer;
    PaymentPortal* portal;


public:
  Transaction(PaymentPortal* portal, Customer* customer, int id);
  Transaction(PaymentPortal* portal, Customer* customer, int id, vector<ItemIn>& items, int totalCost);

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

  Customer* getCustomer() const;
  bool isCompleted() const;
  void completeTransaction();

  int getId() const;

  // string archive() const;

  string toJson() const;
  friend std::ostream& operator<<(std::ostream& os, const Transaction& obj);

};
void to_json(nlohmann::json& j, const Transaction& t);
#endif //TRANSACTION_H
