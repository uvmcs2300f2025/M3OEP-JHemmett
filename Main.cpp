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

int main(){

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

    Item carrot("Carrot", 1000, 10);
    addItem(carrot, sPass);
    Item pea("pea", 10, 1000);
    addItem(pea, sPass);

    Customer customer = createCustomerInput();
    createCustomer(customer, sPass);

    Transaction transaction(&paymentPortal);
    transactionInput(transaction, sPass);


    return 0;
}