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
#include <map>
#include <unordered_map>
#include <fstream>
#include "include/json.hpp"
#include <algorithm>
#include <queue>
#include <optional>
#include "settings.h"
using namespace std;


//TODO: Move to runProgram.
PaymentPortal p1;

unordered_map<int, Item> itemsOld;

//TODO: Read these from settings. I have them as global vars right now because directly reading from settings breaks stuff.
// int numTransactions = 0;
// int numCustomers = 0;
//
// unordered_map<int, Transaction> transactions;
// unordered_map<int, Transaction> transactionArchives;
// queue<int> transactionArchivesAge;
//
// unordered_map<int, Customer> customers;
// queue<int> customersAge;

struct SPass {
    // Pending transactions
    unordered_map<int, Transaction>& transactions;
    // Completed transactions, in memory
    unordered_map<int, Transaction>& transactionArchives;
    // Ages of the completed transactions, newest to oldest by access
    queue<int>& transactionArchivesAge;
    // Customers, in memory
    unordered_map<int, Customer>& customers;
    // Ages of customer in memory
    queue<int>& customersAge;
    Settings& settings;
    PaymentPortal paymentPortal;
    unordered_map<int, Item>& items;

    // Used to make passing info to funcs easier
    SPass(unordered_map<int, Transaction>& transactions,
        unordered_map<int, Transaction>& transactionArchives,
        queue<int>& transactionArchivesAge,
        unordered_map<int, Customer>& customers,
        queue<int>& customersAge,
        Settings& settings,
        PaymentPortal& paymentPortal,
        unordered_map<int, Item>& items
        ):
        transactions(transactions),
        transactionArchives(transactionArchives),
        transactionArchivesAge(transactionArchivesAge),
        customers(customers), customersAge(customersAge),
        settings(settings),
        paymentPortal(paymentPortal),
        items(items){}
};
template <typename dataType>
dataType userInput(string messege);

void TransactionTester1();

bool createTransaction(int index, Transaction& transaction, SPass& sPass);
bool createTransaction(Transaction& transaction, SPass& sPass);


bool cancelTransaction(int index, SPass& sPass);

void numTransactionLimit(bool futureCheck, SPass& sPass);
/*
    Purpose: Checks if a transaction exists

    Input: index to be checked.

    Output: whether the transaction exists
*/
bool checkTransaction(int index, SPass& sPass);

/*
    Moves a transaction from pending to completed and saves it to a file.

    Input: index to be archived.

    Output: whether the archive could be completed.
*/

bool archiveTransaction(int index, SPass& sPass);

/*
    Reads an archived transaction from the save file to memory.

    Input: index to be archived.

    Output: whether the retrieval could be completed.
*/
bool retrieveTransaction(int index, SPass& sPass);
/*
    Checks if a customer exists

    Input: index to be checked.

    Output: whether the customer exists
*/
bool checkCustomer(int index, SPass& sPass);

/*
    Saves customer data to a file

    Input: index to be archived.

    Output: whether the archive could be completed.
*/
bool archiveCustomer(int index, SPass& sPass);

/*
    Reads an archived customer from the save file to memory.

    Input: index to be archived.

    Output: whether the retrieval could be completed.
*/
bool retrieveCustomer(int index, SPass& sPass);

/*
    Removes a customer's save data

    Input: index to be deleted.

    Output: whether the deletion could be completed.
*/
bool removeCustomer(int index, SPass& sPass);

/*
    Finds the item object to an associated ID

    Input: ID

    Output: Item Object
*/

void numCustomerLimit(bool futureCheck, SPass& sPass);

Item* findItem(int id, SPass& sPass);

/*
  Creates a customer from user input
*/
Customer createCustomerInput();

/*
  Creates a customer from data

  Output: whether the customer could create
*/
bool createCustomer(int index, Customer& customer, SPass& sPass);
bool createCustomer(Customer& customer, SPass& sPass);

bool retrieveItems(SPass& sPass);

bool archiveItem(int index, SPass& sPass);

bool addItem(int index, Item& item, SPass& sPass);
bool addItem(Item& item, SPass& sPass);
void runProgram(){
    unordered_map<int, Transaction> transactions;
    unordered_map<int, Transaction> transactionArchives;
    queue<int> transactionArchivesAge;

    unordered_map<int, Customer> customers;
    queue<int> customersAge;
    Settings settings("data/settings.json");
    PaymentPortal paymentPortal;
    unordered_map<int, Item> items;
    SPass sPass(transactions, transactionArchives, transactionArchivesAge, customers, customersAge, settings, paymentPortal,items);

    int cId = userInput<int>("Please enter your customer ID\n(type your customer ID or 0 for no account)");

    // Todo, make sure 0 customer exists, other input validation as well.
    Customer& customerUse = (cId > 0 && retrieveCustomer(cId, sPass)) ? customers.at(cId) : customers.at(0);

    if(cId == 0) {
        cout << "No Customer" << endl;
    } else if(checkCustomer(cId, sPass)) {
        retrieveCustomer(cId, sPass);
        cout << "Welcome back " << customerUse.getFirstName() << endl;
    } else {
        cout << "New Customer" << endl;
    }

}


// bool createTransaction(int index, SPass& sPass){
//   if (checkTransaction(index, sPass)) return false;
//
//   transactions.emplace(index, Transaction());
//   transactionArchivesAge.push(index);
//   return true;
// }

bool checkTransaction(int index, SPass& sPass){
    if (index < 0 ) return false;

    if (sPass.transactions.count(index) || sPass.transactionArchives.count(index)) return true;
    std::ifstream f("data/transactions.json");
    if (!f) {
        std::cerr << "Could not open file";
        return false;
    }


    // Opens JSON file and reads it in.
    nlohmann::json data;
    if (f.peek() == std::ifstream::traits_type::eof()) {
        data = nlohmann::json::array();
    } else {
        f >> data;
    }
    f.close();


   if (!data.is_array()) {
        std::cerr << "JSON is not an array";
        return false;
    }

    // Loops through the transaction file looking for a matching ID.
    for (const auto& t : data){
        if (t.contains("id") && t["id"] == index) return true;
    }

    return false;

}

// I copied some of this from https://json.nlohmann.me and https://www.youtube.com/watch?v=Sa8bdVogGIo, copied lines are marked with *
bool archiveTransaction(int index, SPass& sPass) {
    if (!sPass.transactions.count(index)) return false;

    // Completes the transaction and moves the customer's points and ids
    sPass.transactions.at(index).completeTransaction();

    int customerIndex = sPass.transactions.at(index).getCustomerId();
    if (customerIndex != -2 && checkCustomer(customerIndex, sPass)) {
        if (!sPass.customers.count(customerIndex)) {
            archiveCustomer(customerIndex, sPass);
        }
        sPass.customers.at(customerIndex).setpendingTransaction(nullptr);
        sPass.customers.at(customerIndex).addTransaction(index);
        archiveCustomer(customerIndex, sPass);

    }

    for (ItemIn i : sPass.transactions.at(index).getItems()) {

    }
    using nlohmann::json; //*


    json data;
    ifstream in("data/transactions.json"); //*
    if (in) { //*
        if (in.peek() == ifstream::traits_type::eof()) { //*
            data = json::array(); // empty file //*
        } else {
            in >> data; //*
        } //*
    } //*
    in.close(); //*

    if (!data.is_array()) { //*
        data = json::array(); //*
    } //*

    Transaction &trans = sPass.transactions.at(index); //*
    json t = trans; //*

    // Used to overwrite an existing transaction opposed to adding a new one.
    bool replaced = false;
    for (auto& entry : data) {
        if (t.contains("id") && t["id"] == index) {
            entry = t;
            replaced = true;
            break;
        }
    }

    if (!replaced) {
        data.push_back(t); //*
    }

    // Rewrite file
    std::ofstream out("data/transactions.json"); //*
    out << data.dump(4); //*
    out.close(); //*

    // Moves the transaction to the correct map.
    sPass.transactionArchives.emplace(index, move(sPass.transactions.at(index)));
    sPass.transactions.erase(index);
    sPass.transactionArchivesAge.push(index);
    numTransactionLimit(true, sPass);

    return true;
}

// I copied some of this from https://json.nlohmann.me and https://www.youtube.com/watch?v=Sa8bdVogGIo, copied lines are marked with *
// I used a lot of JSON array parsing stuff as well from https://codesignal.com/learn/courses/parsing-json-with-csharp/lessons/parsing-json-arrays-and-nested-structures and https://www.w3schools.com/js/js_json_arrays.asp
bool retrieveTransaction(int index, SPass& sPass){
    //TODO: Input validation can be added when methods are not called manually
    // if (index < 0 || index > numCustomers) return false;
    // if (!checkCustomer(index)) return false;
    // if (index > numTransactions || index < 0) return false;
    // if (transactionArchives.count(index)) return true;
    // if (!checkTransaction(index)) return false;

    ifstream f("data/transactions.json");
    if (!f) {
        std::cerr << "Could not open file";
        return false;
    }

    nlohmann::json data;

    if (f.peek() == std::ifstream::traits_type::eof()) {
        data = nlohmann::json::array();
    } else {
        f >> data;
    }
    f.close();

    if (!data.is_array()) {
        std::cerr << "JSON is not an array";
        return false;
    }

    for (const auto& t : data){ //*

        if (t.contains("id") && t["id"] == index){

            int customerId = t.value("customerId", -1);  //*
            customerId = checkCustomer(customerId, sPass) ? customerId : -2;
            int totalCost = t.value("totalCost", 0.0);  //*

            Transaction transactionAdd(&sPass.paymentPortal, customerId, totalCost);

            vector<ItemIn> items;
            if (t.contains("items") && t["items"].is_array()) {
                int itemPurchaseQuantity = 0;
                int itemPurchasePrice = 0;
                int itemId = 0;
                //*
                auto itemsVec = t["items"].get<std::vector<nlohmann::json>>(); //*

                for (const auto& itemArr : itemsVec){ //*
                    if (itemArr.size() == 3) {  //*
                        itemId = itemArr[0].get<int>();
                        itemPurchaseQuantity = itemArr[1].get<int>();
                        itemPurchasePrice = itemArr[2].get<int>();

                        ItemIn itemIn(nullptr);

                        if (auto itemObj = findItem(itemId, sPass)) {
                            itemIn.item = findItem(itemId, sPass);
                        }
                        itemIn.purchaseQuantity = itemPurchaseQuantity;
                        itemIn.purchasePrice = itemPurchasePrice;
                        items.push_back(itemIn);
                    }

                }
            }
            transactionAdd.setItems(items);

            sPass.transactionArchives.emplace(index, move(transactionAdd));
            sPass.transactionArchivesAge.push(index);
            numTransactionLimit(true, sPass);

            return true;

        }

    }

    return false;
}

// TODO Match customer
void numTransactionLimit(bool futureCheck, SPass& sPass){
    if (sPass.transactionArchives.size() != sPass.transactionArchivesAge.size())
    {
        cerr << "transactionArchivesAge.size() != transactionArchivesAge.size()";
    }
    if (sPass.transactionArchives.size() > sPass.settings.getMaxTransactions() - futureCheck && !sPass.transactionArchivesAge.empty()){
        int removeIndex = sPass.transactionArchivesAge.front();
        sPass.transactionArchivesAge.pop();
        sPass.transactionArchives.erase(removeIndex);
    }

}
// Very similar to checkTransaction, but with customers instead.
bool checkCustomer(int index, SPass& sPass){
    if (index < 0) return false;
    if (sPass.customers.count(index)) return true;
    std::ifstream f("data/customers.json");
    if (!f) {
        std::cerr << "Could not open file";
        return false;
    }

    nlohmann::json data;
    if (f.peek() == std::ifstream::traits_type::eof()) {
        data = nlohmann::json::array();
    } else {
        f >> data;
    }
    f.close();

    if (!data.is_array()) {
        std::cerr << "JSON is not an array";
        return false;
    }

    for (const auto& t : data)
    {
        if (t.contains("id") && t["id"] == index) return true;
    }

    return false;

}
// Very similar to retrieveTransaction, but with customers instead.
bool retrieveCustomer(int index, SPass& sPass){

    if (!checkCustomer(index, sPass)) return false;

    ifstream f("data/customers.json");
    if (!f) {
        std::cerr << "Could not open file";
        return false;
    }

    nlohmann::json data;
    if (f.peek() == std::ifstream::traits_type::eof()) {
        data = nlohmann::json::array();
    } else {
        f >> data;
    }
    f.close();

    if (!data.is_array()) {
        std::cerr << "JSON is not an array";
        return false;
    }
    vector<int> transactionsAdd;

    for (const auto& t : data){
        if (t.contains("customerId") && t["customerId"] == index){
            int customerId = t.value("id", -1);
            string firstName = t.value("firstName", "NULL");
            string lastName = t.value("lastName", "NULL");
            string phoneNumber = t.value("phoneNumber", "0000000000");
            int credit = t.value("credit", 0);
            if (t.contains("transactions") && t["transactions"].is_array()){
                transactionsAdd = t["transactions"].get<vector<int>>();
            }



            //TODO, add limit check
            sPass.customers.emplace(index, Customer(firstName, lastName, phoneNumber, customerId));
            sPass.customersAge.push(index);
            if (credit > 0) sPass.customers.at(index).setCredit(credit);
            if (transactionsAdd.size()) sPass.customers.at(index).setTransactions(transactionsAdd);
            numCustomerLimit(true, sPass);


            return true;
        }
    }
    return false;
}



// Very similar to archiveTransaction, but with customers instead.
bool archiveCustomer(int index, SPass& sPass){
    if (!checkCustomer(index, sPass)) return false;

    using nlohmann::json; //*

    json data;
    std::ifstream in("data/customers.json"); //*
    if (in) { //*
        if (in.peek() == std::ifstream::traits_type::eof()) { //*
            data = json::array(); // empty file //*
        } else {
            in >> data;           // parse existing JSON //*
        } //*
    } //*
    in.close(); //*

    if (!data.is_array()) { //*
        data = json::array(); //*
    } //*


    // Get transaction from map
    Customer &customer = sPass.customers.at(index); //*

    // Convert to JSON
    json c = customer; //*

    // Append

    // Rewrite file
    std::ofstream out("data/customers.json"); //*

    bool replaced = false;
    for (auto& entry : data) {
        if (c.contains("id") && c["id"] == index) {
            entry = c;
            replaced = true;
            break;
        }
    }

    if (!replaced) {
        data.push_back(c); //*
    }

    out << data.dump(4); //*

    sPass.settings.addNumCustomers();
    return true;
}

//check the transaction one
// TODO, This function needs a lot of work and I'm not sure if it is necessary. Removing a customer will require changing how the customers are stored in memory because the ages are stored in queues, which only really has pop. I think something with iterators might work, we will see.
bool removeCustomer(int index, SPass& sPass)
{
    if (index < 0 || index > sPass.settings.getNumCustomers()) return false;

    // Opens and converts JSON file.
    std::ifstream in("data/customers.json");
    if (!in.is_open()) {
        std::cerr << "Could not open customers.json";
        return false;
    }

    nlohmann::json data;
    in >> data;
    in.close();

    if (!data.is_array()) {
        std::cerr << "customers.json is not an array\n";
        return false;
    }

    bool found = false;
    for (auto it = data.begin(); it != data.end(); ++it) {
        if (it->contains("id") && (*it)["id"] == index) {
            data.erase(it);
            found = true;
            break;
        }
    }

    if (!found) return false;

    std::ofstream out("data/customers.json");
    if (!out.is_open()) {
        std::cerr << "Could not open file for writing";
        return false;
    }
    out << data.dump(5);
    out.close();


    return true;
}

void numCustomerLimit(bool futureCheck, SPass& sPass){
    if (sPass.customers.size() != sPass.customersAge.size())
    {
        cerr << "transactionArchivesAge.size() != transactionArchivesAge.size()";
    }

    // Removes the oldest one if too many are in memory.
    if (sPass.customers.size() > sPass.settings.getMaxCustomers() - futureCheck && !sPass.customersAge.empty()){
        int removeIndex = sPass.customersAge.front();
        sPass.customersAge.pop();
        sPass.customers.erase(removeIndex);
    }
}

Item* findItem(int id, SPass& sPass){
    if (!sPass.items.count(id)) return nullptr;
    return &sPass.items.at(id);
}



// Customer createCustomerInput(){
//     string firstName, lastName, phoneNumber;
//
//     while (true)
//     {
//         firstName = userInput<string>("Please enter your first name\n(E.G. Jonas):");
//
//         if (!firstName.empty()) break;
//
//         cout << "Invalid input!getter" << endl;
//     }
//
//     for (int i = 0; i < firstName.length(); i++)
//     {
//         firstName[i] = (!i) ? toupper(firstName[i]) : tolower(firstName[i]);
//     }
//
//     while (true)
//     {
//         lastName = userInput<string>("Please enter your last name\n(E.G. Hemmett):");
//
//         lastName.erase(std::remove(lastName.begin(), lastName.end(), ' '), lastName.end());
//
//         if (!lastName.empty()) break;
//
//         cout << "Invalid input!getter" << endl;
//     }
//
//     // I added this to account for last names like McNee where 2 characters are capitalized.
//     if (lastName.length() >= 2 && tolower(lastName[0]) == 'm' && tolower(lastName[1]) == 'c')
//     {
//         for (int i = 0; i < lastName.length(); i++)
//         {
//             lastName[i] = (!i || i == 2 ) ? toupper(lastName[i]) : tolower(lastName[i]);
//         }
//
//     } else if (lastName.length() >= 3 && tolower(lastName[0]) == 'v' && tolower(lastName[1]) == 'o'  && tolower(lastName[1]) == 'n'){
//         for (int i = 0; i < lastName.length(); i++)
//         {
//             lastName[i] = (!i || i == 3) ? toupper(lastName[i]) : tolower(lastName[i]);
//         }
//     }
//
//
//     else {
//         for (int i = 0; i < lastName.length(); i++)
//         {
//             lastName[i] = (!i) ? toupper(lastName[i]) : tolower(lastName[i]);
//         }
//
//     }
//
//
//     while (true)
//     {
//         int allChar = true;
//         phoneNumber = userInput<string>("Please enter your phone number\n(Enter as 10 or 11 digits):");
//
//         for (char c : phoneNumber)
//         {
//             if (!isdigit(c)) allChar = false;
//         }
//
//         if (allChar)
//         {
//             if (phoneNumber.length() == 10) phoneNumber.insert(phoneNumber.begin(), '1');
//             if (phoneNumber.length() == 11)  break;
//         }
//
//         cout << "Phone: " << phoneNumber << endl;
//         cout << "Invalid input!getter" << endl;
//     }
//
//     //TODO: Require ID
//     return Customer(0, firstName, lastName, phoneNumber);
// // End of createCustomer
// }

bool createCustomer(int index, Customer& customer, SPass& sPass) {
    if (index == -1) {
        sPass.settings.addNumCustomers();
        index = sPass.settings.getNumCustomers();
    } else if (index < -1) {
        return false;
    }
    if (checkCustomer(index, sPass)) {
        return false;
    }

    if (customer.getCustomerId()== -2) {
        customer.setCustomerId(index);
    }

    sPass.customers.emplace(index, customer);
    sPass.customersAge.emplace(index);
    archiveCustomer(index, sPass);
    numCustomerLimit(true, sPass);
    return true;
}
bool createCustomer(Customer& customer, SPass& sPass) {
    return createCustomer(-1, customer, sPass);
}


bool createTransaction(int index, Transaction& transaction, SPass& sPass) {
    if (index == -1) {
        sPass.settings.addNumCustomers();
        index = sPass.settings.getNumCustomers();
    } else if (index == -2) {
    } else if (index < -2) {
        return false;
    }
    if (checkCustomer(index, sPass)) {
        return false;
    }

    if (transaction.getId() == -2) {
        transaction.setId(index);
    }

    int customerIndex  = transaction.getCustomerId();

    if (customerIndex != -2) {
        if (checkCustomer(customerIndex, sPass)) {
            if (!sPass.customers.count(customerIndex)) {
                retrieveCustomer(customerIndex, sPass);
            }

            if (sPass.customers.at(customerIndex).getPendingTransaction() == nullptr) {
                sPass.customers.at(customerIndex).setpendingTransaction(&transaction);
            } else {
                cerr << "Transaction already exists\n";
                return false;
            }
        }
    }
    sPass.transactions.emplace(index,transaction);
    archiveTransaction(index, sPass);
        return true;

}

bool createTransaction(Transaction &transaction, SPass &sPass) {
    return createTransaction(-1, transaction, sPass);
}

bool cancelTransaction(int index, SPass& sPass) {
    if (!sPass.transactions.count(index)) return false;
    sPass.transactions.at(index).cancelTransaction();
    int customerIndex = sPass.transactions.at(index).getCustomerId();

    if (checkCustomer(customerIndex, sPass)) {
        if (!sPass.customers.count(customerIndex)) {
            retrieveCustomer(customerIndex, sPass);
        }
        sPass.customers.at(customerIndex).setpendingTransaction(nullptr);
        archiveCustomer(customerIndex, sPass);
    }
    sPass.transactions.erase(index);
    return true;
}

    // Allows for multiple input types using one function.
template <typename dataType>
dataType userInput(string messege){
    string inputString;
    dataType input;


    while(true)
    {

        bool noSpace = true;
        cout << messege;


        getline(cin, inputString);

        // Gets rid of invalid characters
        for (char c : inputString)
        {
            if (!(isalpha(c) || isdigit(c)) || c == ' ') {
                noSpace = false;
                break;
            }
        }

        if (noSpace)
        {
            stringstream iss(inputString);

            if (iss >> input) return input;

        }
        cout << "Invalid input!Inner" << endl;


    }

    // End of userInput
}

bool retrieveItems(SPass& sPass) {
    ifstream f("data/items.json");
    if (!f) {
        std::cerr << "Could not open file";
        return false;
    }

    nlohmann::json data;
    f >> data;
    f.close();

    if (!data.is_array()) {
        std::cerr << "JSON is not an array";
        return false;
    }
    unordered_map<int, Item> transactionsAdd;

    for (const auto& t : data) {
        int id = t.value("id", -2);
        string name = t.value("name", "NULL");
        int price = t.value("price", 0);
        int quantity = t.value("quantity", 0);
        transactionsAdd.emplace(id, Item(id, name, price, quantity));
    }
    sPass.items = transactionsAdd;
    return true;

}

bool archiveItem(int index, SPass& sPass){
    if (!findItem(index, sPass)) return false;

    using nlohmann::json; //*

    json data;
    std::ifstream in("data/items.json"); //*
    if (in) { //*
        if (in.peek() == std::ifstream::traits_type::eof()) { //*
            data = json::array(); // empty file //*
        } else {
            in >> data;           // parse existing JSON //*
        } //*
    } //*
    in.close(); //*

    if (!data.is_array()) { //*
        data = json::array(); //*
    } //*


    // Get transaction from map
    Item &item = sPass.items.at(index); //*

    item.setId(index);

    // Convert to JSON
    json c = item; //*

    // Append

    // Rewrite file
    std::ofstream out("data/items.json"); //*

    bool replaced = false;
    for (auto& entry : data) {
        if (c.contains("id") && c["id"] == index) {
            entry = c;
            replaced = true;
            break;
        }
    }

    if (!replaced) {
        data.push_back(c); //*
    }

    out << data.dump(4); //*

    sPass.settings.addNumItems();

    return true;
}


bool addItem(int index, Item& item, SPass& sPass) {
    if (index == -1) {
        sPass.settings.addNumItems();
        index = sPass.settings.getNumItems();
    } else if (index < -1) {
        return false;
    }
    if (findItem(index, sPass)){
        return false;
    }
    if (item.getId() == -2) {
        item.setId(index);

    }
    sPass.items.emplace(index, item);
    archiveItem(index, sPass);

    return true;

}
bool addItem(Item& item, SPass& sPass) {
    return addItem(-1, item, sPass);
}

bool removeItem(int index, SPass& sPass) {
    if (!findItem(index, sPass)) return false;
    sPass.items.erase(index);
    using nlohmann::json; //*

    json data;
    std::ifstream in("data/items.json"); //*
    if (in) { //*
        if (in.peek() == std::ifstream::traits_type::eof()) { //*
            data = json::array(); // empty file //*
        } else {
            in >> data;           // parse existing JSON //*
        } //*
    } //*
    in.close(); //*

    if (!data.is_array()) { //*
        data = json::array(); //*
    } //*


    // Get transaction from map
    Item &item = sPass.items.at(index); //*

    // Convert to JSON
    json c = item; //*

    // Append

    // Rewrite file
    std::ofstream out("data/items.json"); //*

    bool removed = false;
    for (auto it = data.begin(); it != data.end(); ++it) {
        if (it->contains("id") && (*it)["id"] == index) {
            data.erase(it);
            removed = true;
            break;
        }
    }


    if (!removed) {
       cerr << "Could not remove JSON item at: " << index << endl;
    }

    out << data.dump(4); //*

    return true;
}

