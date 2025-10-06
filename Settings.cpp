#include <string>
#include <fstream>
#include <iostream>
#include "include/json.hpp"
#include "Settings.h"
using namespace std;
Settings::Settings(const string path) : path(path) {
  ifstream in(path);
  if (in.is_open()) {
    try{
      nlohmann::json j;
      in >> j;
      numTransactions = j.value("numTransactions", numTransactions);
      numCustomers = j.value("numCustomers", numCustomers);
      maxTransactions = j.value("maxTransactions", maxTransactions);
      maxCustomers = j.value("maxCustomers", maxCustomers);

    } catch(...) {
      updateFile();
    }
  } else{
    updateFile();
  }
}

void Settings::updateFile() {
  nlohmann::json j;
  j["numTransactions"] = numTransactions;
  j["numCustomers"] = numCustomers;
  j["maxTransactions"] = maxTransactions;
  j["maxCustomers"] = maxCustomers;

  ofstream out(path);
  if (out.is_open()) {
    out << j.dump(5);
    out.close();
  }
}

int Settings::getNumTransactions() const {return numTransactions;}
void Settings::setNumTransactions(int numTransactions) {
  this->numTransactions = numTransactions;
  updateFile();
}

void Settings::addNumTransactions() {
  numTransactions++;
  cout << numTransactions << endl;
  updateFile();
}

int Settings::getNumCustomers() const {return numCustomers;}
void Settings::setNumCustomers(int numCustomers) {
  this->numCustomers = numCustomers;
  updateFile();
}

void Settings::addNumCustomers() {
  numCustomers++;
  updateFile();
}

int Settings::getMaxTransactions() const {return maxTransactions;}
int Settings::getMaxCustomers() const {return maxCustomers;}