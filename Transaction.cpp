#include <vector>
#include <string>
#include "item.h"
#include "ItemIn.h"
#include "Transaction.h"
#include "include/json.hpp"
#include <iostream>

using namespace std;

/*
  Stores transaction information.
 */
Transaction::Transaction(PaymentPortal* portal, Customer* customer, int id) : completed(false), totalCost(0), portal(portal), customer(customer), id(id) {}
Transaction::Transaction(PaymentPortal* portal, Customer* customer, int id, vector<ItemIn>& items, int totalCost) : completed(true), totalCost(totalCost), portal(portal), customer(customer), id(id) {}

// If a transaction is canceled, items are restocked
Transaction::~Transaction() {
  if (!completed) {
    for (ItemIn& item : items) {
      item.item->changeHoldQuantity(item.purchaseQuantity);
    }
  }
}

int Transaction::getTotalCost() const {return totalCost;}

vector<ItemIn> Transaction::getTransaction() const {return items;}

// Items are reserved when they are added to cart.
bool Transaction::addItem(Item& item, int quantity)
{
  if (completed) return false;
  if (quantity <= 0 || item.getQuantity() < quantity) return false;

  ItemIn itemAdd(&item);

  // Something to note that if the price is changed in between things being added, the lower price is kept.
  // This updates existing quantities if the item is already in cart
  for (ItemIn& i : items)
  {
    if (i == itemAdd)
    {
      i.purchaseQuantity += quantity;
      totalCost += quantity * i.purchasePrice;

      item.changeHoldQuantity(quantity);
      return true;
    }
  }

  itemAdd.purchasePrice = item.getPrice();
  itemAdd.purchaseQuantity = quantity;

  items.push_back(itemAdd);

  totalCost += quantity * item.getPrice();

  item.changeHoldQuantity(quantity);

  return true;
}

bool Transaction::addItem(Item& item) {return addItem(item, 1);}

bool Transaction::removeItem(Item& item, int quantity)
{
  if (completed) return false;
  if (quantity <= 0) return false;

  // Removes desiered quantity, if entire quantity is removed then the item is removed.
  // https://www.w3schools.com/cpp/ref_vector_erase.asp
  for (auto it = items.begin(); it != items.end();){
    if (it->item == &item){
      if (it->purchaseQuantity <= quantity){
        totalCost -= it->purchaseQuantity * it->purchasePrice;

        item.changeHoldQuantity(it->purchaseQuantity);

        items.erase(it);

        return true;
      } else {
        totalCost -= quantity * it->purchasePrice;
        it->purchaseQuantity -= quantity;

        item.changeHoldQuantity(quantity);

        return true;
      }
    }
    it++;
  }
  return false;
}

bool Transaction::removeItem(Item& item) {return removeItem(item, 1);}


bool Transaction::setItems(vector<ItemIn>& items) {
  if (!completed) return false;
  this->items = items;
  return true;
}

Customer* Transaction::getCustomer() const {return customer;}

 bool Transaction::isCompleted()  const{return completed;}

void Transaction::completeTransaction(){
  portal->deposit(totalCost);
  for (ItemIn i : items)
  {

    i.item->changeQuantity(-i.purchaseQuantity);

  }
  completed = true;
}

int Transaction::getId() const {return id;}


std::ostream& operator<<(std::ostream& os, const Transaction& obj) {
  if (obj.completed) {
    os << "- Shopping Receipt -" << endl;
    os << "Customer: " << *obj.customer << endl;
    for (ItemIn item : obj.items){
      os << item.item->getName() << "(" << item.item->getId() << ") " << item.item->getPrice() << "x $" << item.purchasePrice << endl;
    }
    os << "Total: $" << obj.totalCost << endl;
    return os;
  }

  os << "- Shopping Cart -" << endl;
  os << "Customer: " << *obj.customer << endl;

  for (ItemIn item : obj.items){
    os << item.item->getName() << "(" << item.item->getId() << ") " << item.purchaseQuantity << "x $" << item.purchasePrice << endl;
  }
  os << "Total: $" << obj.totalCost << endl;
  return os;
  os << "- Shopping Cart -" << endl;

}

void to_json(nlohmann::json& j, const Transaction& t){
  j = nlohmann::json{
    // https://json.nlohmann.me/api/basic_json/array/
      {"id", t.getId()},
    {"items", nlohmann::json::array()},
{"totalCost", t.getTotalCost()},
{"customerId", t.getCustomer()->getCustomerId()},
  };

  for (const ItemIn item : t.getTransaction())
  {
    j["items"].push_back(item.item->getId());
    j["items"].push_back(item.purchaseQuantity);
    j["items"].push_back(item.purchasePrice);


  }
}