#include <iostream>
#include <string>
#include <limits>
#include "Customer.h"
#include "Employee.h"
#include "Manager.h"
#include "Transaction.h"
#include "Item.h"
#include <list>
#include <algorithm>
#include <string>
#include <iostream>
#include <cctype>
#include <sstream>
#include "ItemIn.h"
#include <memory>
#include <map>
#include <unordered_map>
#include <fstream>
#include "include/json.hpp"
#include <algorithm>
#include <queue>
#include <optional>
#include "settings.h"
#include "Tester.h"

#include "base.cpp"

bool settingsTest();
bool itemTest();
bool paymentPortalTest();
bool transactionTest();
bool mainTest();


int main(){
  cout << "settingsTest(): " << settingsTest() << endl;
  cout << "itemTest():" << itemTest() << endl;
  cout << "paymentPortalTest()" << paymentPortalTest() << endl;
  cout << "transactionTest()" << transactionTest() << endl;
  cout << "mainTest(): " << mainTest() << endl;

  // runProgram();
  return 0;
}

bool settingsTest(){
  bool passed = true;
  // Wipes for testing.
  ofstream tF("data/transactions.json");
  ofstream sF("data/settings.json");
  ofstream cF("data/customers.json");
  tF.close();
  sF.close();
  cF.close();


  Settings settings("data/settings.json");
  settings.setNumTransactions(0);
  settings.setNumCustomers(0);

  if (settings.getNumTransactions()){
    cout << "getNumTransactions() is not 0, but: " << settings.getNumTransactions() << endl;
    passed = false;
  }

  if (settings.getNumCustomers()){
    cout << "getNumCustomers() is not 0, but: " << settings.getNumCustomers() << endl;
    passed = false;
  }

  if (settings.getMaxTransactions() != 2){
    cout << "getMaxTransactions() is not 2, but: " << settings.getMaxTransactions() << endl;
    passed = false;
  }

  if (settings.getMaxCustomers() != 2){
    cout << "getMaxCustomers() is not 2, but: " << settings.getMaxCustomers() << endl;
    passed = false;
  }

  return passed;
}

bool itemTest(){
  bool passed = true;

  Item item(0, "name", 10, 10);


  // Default
  if (item.getId() != 0){
    passed = false;
    cout<<"Item getId() is not 0, but: "<<item.getId()<<endl;
  }

  if (item.getName() != "name"){
    passed = false;
    cout<<"Item getName() is not name, but: "<<item.getName()<<endl;
  }

  if (item.getQuantity() != 10){
    passed = false;
    cout<<"Item getQuantity() is not 10, but: "<<item.getQuantity()<<endl;
  }

  if (item.getHoldQuantity() != 0){
    passed = false;
    cout<<"Item getHoldQuantity() is not 0, but: "<<item.getHoldQuantity()<<endl;
  }

  if (item.getPrice() != 10){
    passed = false;
    cout<<"Item getPrice() is not 10, but: "<<item.getPrice()<<endl;
  }

  if (item.getAvailableQuantity() != 10){
    passed = false;
    cout<<"Item getAvailableQuantity() is not 10, but: "<<item.getAvailableQuantity()<<endl;
  }

  // Setters


  item.setName("name2");
  if (item.getName() != "name2"){
    passed = false;
    cout<<"Item getName(), after setName(), is not name2, but: "<<item.getName()<<endl;
  }

  item.setQuantity(11);
  if (item.getQuantity() != 11){
    passed = false;
    cout<<"Item getQuantity(), after setQuantity, is not 11, but: "<<item.getQuantity()<<endl;
  }

  item.setHoldQuantity(9);
  if (item.getHoldQuantity() != 9){
    passed = false;
    cout<<"Item getHoldQuantity(), after setId(), is not 9, but: "<<item.getHoldQuantity()<<endl;
  }

  item.setPrice(11);
  if (item.getPrice() != 11){
    passed = false;
    cout<<"Item getPrice(), after setPrice(), is not 11, but: "<<item.getPrice()<<endl;
  }

  // Changers
  item.changeQuantity(-1);
  if (item.getQuantity() != 10){
    passed = false;
    cout<<"Item getQuantity(), after changeQuantity(-1), is not 10, but: "<<item.getQuantity()<<endl;
  }

  if (item.getHoldQuantity() != 8){
    passed = false;
    cout<<"Item getHoldQuantity(), after changeQuantity(-1), is not 8, but: "<<item.getHoldQuantity()<<endl;
  }

  item.changeQuantity(1);
  if (item.getQuantity() != 11){
    passed = false;
    cout<<"Item getQuantity(), after changeQuantity(1), is not 11, but: "<<item.getQuantity()<<endl;
  }

  if (item.getHoldQuantity() != 8){
    passed = false;
    cout<<"Item getHoldQuantity(), after changeQuantity(1), is not 8, but: "<<item.getHoldQuantity()<<endl;
  }


  item.changeHoldQuantity(1);
  if (item.getHoldQuantity() != 9){
    passed = false;
    cout<<"Item getHoldQuantity(), after changeHoldQuantity(1), is not 9, but: "<<item.getHoldQuantity()<<endl;
  }

  ItemIn itemIn(&item);
  if (*itemIn.item == item){} else{
    passed = false;
    cout << "temIn.item does not match item" << endl;
  }

  if(itemIn.purchaseQuantity != 0){
    passed = false;
    cout << "temIn.item purchaseQuantity is not 0, but: " << itemIn.purchaseQuantity << endl;
  }

  if(itemIn.purchasePrice != 0){
    passed = false;
    cout << "temIn.item purchasePrice is not 0, but: " << itemIn.purchasePrice << endl;
  }
  return passed;
}

bool paymentPortalTest(){
  bool passed = true;
  PaymentPortal paymentPortal;

  if (paymentPortal.getBalance() != 0){
    passed = false;
    cout << "paymentPortal getBalance() is not 0, but: " << paymentPortal.getBalance() << endl;
  }

  if (paymentPortal.getOverDraft() != 0){
    passed = false;
    cout << "paymentPortal getOverDraft() is not 0, but: " << paymentPortal.getOverDraft() << endl;
  }

  paymentPortal.deposit(-1);

  if (paymentPortal.getBalance() != 0){
    passed = false;
    cout << "paymentPortal getBalance(), deposit(-1), after is not 0, but: " << paymentPortal.getBalance() << endl;
  }

  paymentPortal.deposit(20);

  if (paymentPortal.getBalance() != 20){
    passed = false;
    cout << "paymentPortal getBalance(), after deposit(20), is not 20, but: " << paymentPortal.getBalance() << endl;
  }

  if (paymentPortal.getOverDraft() != 0){
    passed = false;
    cout << "paymentPortal getOverDraft(), after deposit(20),  is not 0, but: " << paymentPortal.getOverDraft() << endl;
  }

  paymentPortal.withdraw(30);

  if (paymentPortal.getBalance() != 0){
    passed = false;
    cout << "paymentPortal getBalance(), after withdraw(30), is not 20, but: " << paymentPortal.getBalance() << endl;
  }

  if (paymentPortal.getOverDraft() != 10){
    passed = false;
    cout << "paymentPortal getOverDraft(), after withdraw(30),  is not 0, but: " << paymentPortal.getOverDraft() << endl;
  }


  return passed;
}

bool transactionTest(){
  bool passed = true;
  Settings settings("data/settings.json");
  PaymentPortal paymentPortal;
  Customer customer("Jonas", "H", "8023632222");
  Item item(0, "name", 10, 10);
  ItemIn itemIn(&item);
  Transaction transaction(&paymentPortal, 2,1);

  if(transaction.getId() != 2){
    passed = false;
    cout << "Transaction getId() is not 2, but: " << transaction.getId() << endl;
  }
  if(transaction.isCompleted()){
    passed = false;
    cout << "Transaction isCompleted() is true" << endl;
  }

  if(transaction.getCustomerId() != 1){
    passed = false;
    cout << "transaction.getCustomerId() != 1, but: " << transaction.getCustomerId() << endl;
  }

  if(transaction.getTotalCost() != 0){
    passed = false;
    cout << "transaction.getTotalCost() is not 0, but: " << transaction.getTotalCost() << endl;
  }

  transaction.addItem(item);
  if(transaction.getTotalCost() != 10){
    passed = false;
    cout << "transaction.getTotalCost(), after transaction.addItem(item), is not 10, but: " << transaction.getTotalCost() << endl;
  }

  transaction.removeItem(item);
  if(transaction.getTotalCost() != 0){
    passed = false;
    cout << "transaction.getTotalCost(), after transaction.removeItem(item), is not 0, but: " << transaction.getTotalCost() << endl;
  }

  transaction.completeTransaction();
  if(!transaction.isCompleted()){
    passed = false;
    cout << "Transaction isCompleted(), after completeTransaction(), is false" << endl;
  }
  return passed;
}

bool mainTest(){

  bool passed = true;

  unordered_map<int, Transaction> transactions;
  unordered_map<int, Transaction> transactionArchives;
  queue<int> transactionArchivesAge;

  unordered_map<int, Customer> customers;
  queue<int> customersAge;

  unordered_map<int, Item> items;

  Settings settings("data/settingsTest.json");

  // Wipe data
  settings.wipe();
  ofstream("data/transactions.json", ios::trunc).close();
  ofstream("data/items.json", ios::trunc).close();
  ofstream("data/customers.json", ios::trunc).close();


  PaymentPortal paymentPortal;
  SPass sPass(transactions, transactionArchives, transactionArchivesAge, customers, customersAge, settings, paymentPortal, items);

  Item item("Carrot", 100, 12);

  if(items.size() != 0){
    passed = false;
    cout << "1: items.size() is " << items.size() << " not 0" << endl;
  }

  if(item.getId() != -2){
    passed = false;
    cout << "2: item.getId() is " << item.getId() << " not -2" << endl;
  }


  addItem(item,sPass);
  if(items.size() != 1){
    passed = false;
    cout << "3: items.size() is " << items.size() << " not 1" << endl;
  }

  if(!items.count(2)){
    passed = false;
    cout << "4: items.count(2) is " << items.count(2) << " not 2" << endl;
  }

  // Manually erase to test archive and retrieve
  items.erase(2);

  if(items.size() != 0){
    passed = false;
    cout << "5: items.size() is " << items.size() << " not 0" << endl;
  }

  retrieveItems(sPass);
  if(items.size() != 1){
    passed = false;
    cout << "6: items.size() is " << items.size() << " not 1" << endl;
  }

  if(!items.count(2)){
    passed = false;
    cout << "7: items.count(2) is " << items.count(2) << " not 1" << endl;
  }

  if(items.at(2).getName() != "Carrot"){
    passed = false;
    cout << "8: items.count(2) is '" << items.count(2) << "' not 'Carrot'" << endl;
  }

  Customer cust("Jonas", "H", "8023632222");

  if(customers.size() != 0){
    passed = false;
    cout << "9: customers.size() is " << customers.size() << " not 0" << endl;
  }

  createCustomer(cust, sPass);
  if(customers.size() != 1){
    passed = false;
    cout << "10: customers.size() is " << customers.size() << " not 1" << endl;
  }

  if(!customers.count(2)){
    passed = false;
    cout << "11: customers.count(2) is " << customers.count(2) << " not 1" << endl;
  }

  if(customers.at(2).getFirstName() != "Jonas"){
    passed = false;
    cout << "12: customers.count(2) is " << customers.at(2).getFirstName() << " not 'Jonas'" << endl;
  }

  // Manually erase to test archive and retrieve
  customers.erase(2);
  customersAge.pop();

  if(customers.size() != 0){
    passed = false;
    cout << "12: customers.size() is " << customers.size() << " not 0" << endl;
  }

  if(customers.size() != 0){
    passed = false;
    cout << "13: customers.size() is " << customers.size() << " not 0" << endl;
  }

  retrieveCustomer(2, sPass);

  if(customers.size() != 1){
    passed = false;
    cout << "14: customers.size() is " << customers.size() << " not 1" << endl;
  }

  if(!customers.count(2)){
    passed = false;
    cout << "15: customers.count(2) is " << customers.count(2) << " not 1" << endl;
  }

  if(customers.at(2).getFirstName() != "Jonas"){
    passed = false;
    cout << "16: customers.count(2) is '" << customers.count(2) << "' not 'Jonas'" << endl;
  }

  Transaction tr(&paymentPortal);

  if(transactions.size() != 0){
    passed = false;
    cout << "17: transactions.size() is " << transactions.size() << " not 0" << endl;
  }

  if(tr.getId() != -2){
    passed = false;
    cout << "18: transactions.getId() is " << tr.getId() << " not -2" << endl;

  }

  if(tr.getCustomerId() != -2){
    passed = false;
    cout << "19: transactions.getCustomerId() is " << tr.getId() << " not -2" << endl;
  }

  createTransaction(tr,sPass);

  if(transactions.size() != 1){
    passed = false;
    cout << "20: transactions.size() is " << transactions.size() << " not 1" << endl;

  }

  if(tr.getId() != 2){
    passed = false;
    cout << "21: transactions.getId() is " << tr.getId() << " not 2" << endl;

  }
  if(tr.getCustomerId() != -2){
    passed = false;
    cout << "22: transactions.getCustomerId() is " << tr.getId() << " not -2" << endl;
  }

  if(item.getAvailableQuantity() != 12){
    passed = false;
    cout << "23: item.getAvailableQuantity() is " << item.getAvailableQuantity() << " not 12" << endl;
  }

  tr.addItem(item);

  if(item.getAvailableQuantity() != 11){
    passed = false;
    cout << "23: item.getAvailableQuantity() is " << item.getAvailableQuantity() << " not 11" << endl;
  }

  cancelTransaction(2, sPass);

  if(transactions.size() != 0){
    passed = false;
    cout << "23: transactions.size() is " << transactions.size() << " not 0" << endl;

  }

  return passed;



}