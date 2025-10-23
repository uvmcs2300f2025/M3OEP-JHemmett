#include <string>
#include <fstream>
#include <iostream>
#include "include/json.hpp"

#ifndef SETTINGS_H
#define SETTINGS_H


/*
  Stores settings information that may be changed between runs.
*/
class Settings {
private:
  const std::string path;
  // The 0th customer is used for people not logged in, transactions start at 1 to match for simplicity’s sake
  int numTransactions = 1;
  int numCustomers = 1;
  int numItems = 1;
  int maxTransactions = 2;
  int maxCustomers = 2;

  void updateFile();

public:
  Settings(const std::string path);
  int getNumTransactions() const;
  void setNumTransactions(int numTransactions);
  void addNumTransactions();

  int getNumCustomers() const;
  void setNumCustomers(int numCustomers);
  void addNumCustomers();

  int getMaxTransactions() const;
  int getMaxCustomers() const;

  int getNumItems() const;
  void setNumItems(int numItems);
  void addNumItems();

  void wipe();

};



#endif //SETTINGS_H
