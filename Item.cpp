
#include "Item.h"
#include <string>
#include <iostream>

using namespace std;

Item::Item(int id) : id(id), name(""), price(0), quantity(0), holdQuantity(0) {}
Item::Item(int id, string name) : id(id), name(name), price(0), quantity(0),holdQuantity(0) {}
Item::Item(int id, string name, int price) : id(id), name(name), price(price), quantity(0), holdQuantity(0) {}
Item::Item(int id, string name, int price, int quantity) : id(id), name(name), price(price), quantity(quantity), holdQuantity(0) {}

string Item::getName() const {return name;}
void Item::setName(string name) {this->name = name;}

int Item::getId() const {return id;}
void Item::setId(int id) {this->id = id;}

int Item::getPrice() const {return price;}
void Item::setPrice(int price) {this->price = price;}

int Item::getQuantity() const {return quantity;}
void Item::setQuantity(int quantity) {this->quantity = quantity;}
bool Item::changeQuantity(int quantity){
  if (quantity > 0)
  {
    this->quantity += quantity;
    return true;
  }

  if (this->quantity - holdQuantity >= 0)
  {
    holdQuantity += quantity;

    this->quantity += quantity;

    return true;
  }

  return false;


}

int Item::getHoldQuantity() const {return holdQuantity;}
void Item::setHoldQuantity(int holdQuantity) {this->holdQuantity = holdQuantity;}
void Item::changeHoldQuantity(int holdQuantity) {this->holdQuantity += holdQuantity;}

int Item::getAvailableQuantity() const{return quantity - holdQuantity;}

bool Item::operator==(const Item& other) const {
  return (id == other.id);
}

std::ostream& operator<<(std::ostream& os, const Item& obj){
  os << obj.name << "(" << obj.id << ") " << obj.quantity << "x " << obj.holdQuantity << "xHold $" << obj.price;
  return os;
}

void to_json(nlohmann::json& j, const Item& t){
  j = nlohmann::json{
    // https://json.nlohmann.me/api/basic_json/array/
{"id", t.getId()},
{"name", t.getName()},
{"price", t.getPrice()},
{"quantity", t.getQuantity()},
        };

}