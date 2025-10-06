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
  int numTransactions = 0;
  int numCustomers = 0;
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

};



#endif //SETTINGS_H
