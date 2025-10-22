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
  item.setId(1);
  if (item.getId() != 1){
    passed = false;
    cout<<"Item getId(), after setId(), is not 1, but: "<<item.getId()<<endl;
  }

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
  Customer customer(1, "Jonas", "H", "8023632222");
  Item item(0, "name", 10, 10);
  ItemIn itemIn(&item);
  Transaction transaction(&paymentPortal, &customer, 2);

  if(transaction.getId() != 2){
    passed = false;
    cout << "Transaction getId() is not 2, but: " << transaction.getId() << endl;
  }
  if(transaction.isCompleted()){
    passed = false;
    cout << "Transaction isCompleted() is true" << endl;
  }

  if(transaction.getCustomer()->getFirstName() != customer.getFirstName()){
    passed = false;
    cout << "transaction.getCustomer()->getFirstName() is not Jonas, but: " << transaction.getCustomer()->getFirstName() << endl;
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
  Settings settings("data/settings.json");
  PaymentPortal paymentPortal;
  SPass sPass(transactions, transactionArchives, transactionArchivesAge, customers, customersAge, settings, paymentPortal);


  // Declared in Base
  // PaymentPortal p1;
//  Customer customer(1, "Jonas", "H", "8023632222");

  // Declared in Base
  //  unordered_map<int, Item> items;
  items.emplace(0, Item(0, "name", 10, 10));

  // Declared in Base
  // unordered_map<int, Customer> customers;
  customers.emplace(1, Customer(1, "Jonas", "H", "8023632222"));

  sPass.settings.addNumCustomers();

  // Declared in Base
  // queue<int> customersAge;
  customersAge.emplace(1);

  // Declared in Base
  // unordered_map<int, Transaction> transactions;
  transactions.emplace(2, Transaction(&p1, &customers.at(1), 2));

  // Declared in Base
  // unordered_map<int, Transaction> transactionArchives;

  // Declared in Base
  //  queue<int> transactionArchivesAge;

  if (customers.size() != 1){
    passed = false;
    cout << "customers.size() is not 1, but: " << customers.size() << endl;
  }

  if (customersAge.size() != 1){
    passed = false;
    cout << "customersAge.size() is not 1, but: " << customersAge.size() << endl;
  }

  if (transactions.size() != 1){
    passed = false;
    cout << "transactions.size() is not 1, but: " << transactions.size() << endl;
  }

  if (transactionArchives.size() != 0){
    passed = false;
    cout << "transactionArchives.size() is not 0, but: " << transactionArchives.size() << endl;
  }

  if (transactionArchivesAge.size() != 0){
    passed = false;
    cout << "transactionArchivesAge.size() is not 0, but: " << transactionArchivesAge.size() << endl;
  }

  customers.emplace(2, Customer("CoolerJonas", "GH", "8023632223"));
  settings.addNumCustomers();
  customers.emplace(3, Customer("CoolestJonas", "JGH", "8023632224"));
  settings.addNumCustomers();

  archiveCustomer(1, sPass);

  settings.setNumCustomers(0);
  // Emptys Customers
  customers.clear();

  while (customersAge.size()) customersAge.pop();


  retrieveCustomer(1, sPass);


  if (customers.size() != 1){
    passed = false;
    cout << "customers.size(), after  retrieveCustomer(1, settings), is not 1, but: " << customers.size() << endl;
  }

  if (customersAge.size() != 1){
    passed = false;
    cout << "customersAge.size, after  retrieveCustomer(1, settings), is not 1, but: " << customersAge.size() << endl;
  }

  if (transactions.size() != 1){
    passed = false;
    cout << "transactions.size(), after  retrieveCustomer(1, settings), is not 1, but: " << transactions.size() << endl;
  }
  customers.at(1).addCredit(10);
  archiveCustomer(1, sPass);




  archiveTransaction(2, sPass);

  sPass.settings.setNumTransactions(0);
  // Emptys Customers
  transactions.clear();
  while (transactionArchivesAge.size()) transactionArchivesAge.pop();

  retrieveTransaction(2, sPass);

  if (transactions.size() != 1){
    passed = false;
    cout << "transaction.size(), after  retrieveTransaction(2, settings), is not 1, but: " << transactions.size() << endl;
  }

  if (transactionArchivesAge.size() != 1){
    passed = false;
    cout << "transactionArchivesAge.size, after  retrieveTransaction(2, settings), is not 1, but: " << customersAge.size() << endl;
  }

  return passed;



}