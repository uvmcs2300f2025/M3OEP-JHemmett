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

unordered_map<int, Item> items;

//TODO: Read these from settings. I have them as global vars right now because directly reading from settings breaks stuff.
int numTransactions = 0;
int numCustomers = 0;

unordered_map<int, Transaction> transactions;
unordered_map<int, Transaction> transactionArchives;
queue<int> transactionArchivesAge;

unordered_map<int, Customer> customers;
queue<int> customersAge;


template <typename dataType>
dataType userInput(string messege);

void TransactionTester1();

bool createTransaction(int intex Transaction& transaction);
bool createTransaction(Transaction& trasnaction, Settings &settings);

/*
    Purpose: Checks if a transaction exists

    Input: index to be checked.

    Output: whether the transaction exists
*/
bool checkTransaction(int index);

/*
    Moves a transaction from pending to completed and saves it to a file.

    Input: index to be archived.

    Output: whether the archive could be completed.
*/
bool archiveTransaction(int index, Settings &settings);

/*
    Reads an archived transaction from the save file to memory.

    Input: index to be archived.

    Output: whether the retrieval could be completed.
*/
bool retrieveTransaction(int index, Settings &settings);
/*
    Checks if a customer exists

    Input: index to be checked.

    Output: whether the customer exists
*/
bool checkCustomer(int index);

/*
    Saves customer data to a file

    Input: index to be archived.

    Output: whether the archive could be completed.
*/
bool archiveCustomer(int index, Settings &settings);

/*
    Reads an archived customer from the save file to memory.

    Input: index to be archived.

    Output: whether the retrieval could be completed.
*/
bool retrieveCustomer(int index, Settings &settings);

/*
    Removes a customer's save data

    Input: index to be deleted.

    Output: whether the deletion could be completed.
*/
bool removeCustomerArchive(int index, Settings &settings);

/*
    Finds the item object to an associated ID

    Input: ID

    Output: Item Object
*/
optional<Item> findItem(int id);

/*
  Creates a customer from user input
*/
Customer createCustomerInput();

/*
  Creates a customer from data

  Output: whether the customer could create
*/
//TODO: Finish this
bool createCustomer();

//TODO: createTransaction()


//TODO: A lot of the code in here should be separate functions
int runProgram(){
    Settings settings("data/settings.json");

    cout << settings.getNumCustomers() << endl;


    // Wipes for testing.
    ofstream MyFile("data/transactions.json");

    items.emplace(1234, Item(1234, "Carrot", 1, 10));
    items.emplace(2345, Item(2345, "Pea", 2, 10));

    //TODO: Read from setting file
    customers.emplace(numCustomers, Customer(numCustomers, "Jonas", "H", "8023632222"));
    numCustomers++;

    customers.emplace(numCustomers, Customer(numCustomers, "JJonas", "J", "1023632222"));
    numCustomers++;

    transactions.emplace(numTransactions, Transaction(&p1, &customers.at(0), numTransactions));
    numTransactions++;

    cout << items.at(1234) << endl;

    transactions.at(0).addItem(items.at(1234));

    cout << items.at(1234) << endl;

    cout << "Transactions size: " << transactions.size() << endl;
    cout << "Transactions Archive size: " << transactionArchives.size() << endl;
    archiveTransaction(0, settings);

    cout << "Transactions size: " << transactions.size() << endl;
    cout << "Transactions Archive size: " << transactionArchives.size() << endl;

    transactions.emplace(numTransactions, Transaction(&p1, &customers.at(1), numTransactions));
    numTransactions++;

    transactions.at(1).addItem(items.at(2345));
    cout << "Transactions size: " << transactions.size() << endl;
    cout << "Transactions Archive size: " << transactionArchives.size() << endl;


    return 0;

    // End of main
}
void TransactionTester1()
{
    int numtransactions = 0;
    PaymentPortal p1;
    Item i1(1234, "Carrot", 1, 10);
    Customer jonas(0, "Jonas", "H", "8023632222");
    Transaction t1(&p1, &jonas, numtransactions);
    t1.addItem(i1);
    cout << i1 << endl;

    cout << i1 << endl;
}


bool createTransaction(int index, Transaction &transaction){
  if (checkTransaction(index)) return false;

  transactions.emplace(index, move(transaction));
  transactionArchivesAge.push(index);
  return true;
}

bool createTransaction(Transaction& trasnaction, Settings &settings){
  createTransaction(settings.getNumCustomers(), transaction)
  settings.addNumCustomers();
}


bool checkTransaction(int index){
    if (transactions.count(index) || transactionArchives.count(index)) return true;
    std::ifstream f("data/transactions.json");
    if (!f) {
        std::cerr << "Could not open file";
        return false;
    }


    // Opens JSON file and reads it in.
    nlohmann::json data;
    f >> data;
    f.close();


    if (!data.is_array()) {
        std::cerr << "JSON is not an array";
        return false;
    }

    // Loops through the transaction file looking for a matching ID.
    for (const auto& t : data)
    {
        if (t.contains("id") && t["id"] == index) return true;
    }

    return false;

}

// I copied some of this from https://json.nlohmann.me and https://www.youtube.com/watch?v=Sa8bdVogGIo, copied lines are marked with *
bool archiveTransaction(int index, Settings &settings) {
    //TODO: Input validation can be added when methods are not called manually.
    // if (index < 0 || index > numCustomers) return false;
    // if (!checkCustomer(index)) return false;
    // if (index < 0 || index > numTransactions) return false;
    // if (!checkTransaction(index)) return false;

    transactions.at(index).completeTransaction();
    transactions.at(index).getCustomer()->addTransaction(index);

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

    Transaction &trans = transactions.at(index); //*

    json t = trans; //*

    data.push_back(t); //*

    // Rewrite file
    std::ofstream out("data/transactions.json"); //*

    out << data.dump(4); //*
    out.close(); //*

    // If there are too many transactions in memory, the oldest one is archived
    if (transactionArchives.size() > settings.getMaxTransactions()) {
        transactionArchives.erase(transactionArchivesAge.front());
        transactionArchivesAge.pop();
    }
    auto transactionMove = transactions.extract(index);
    transactionArchives.insert(move(transactionMove));
    transactionArchivesAge.push(index);

    settings.addNumTransactions();
    return true;
}

// I copied some of this from https://json.nlohmann.me and https://www.youtube.com/watch?v=Sa8bdVogGIo, copied lines are marked with *
bool retrieveTransaction(int index, Settings &settings){
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
    f >> data;
    f.close();

    if (!data.is_array()) {
        std::cerr << "JSON is not an array";
        return false;
    }

    for (const auto& t : data){ //*
        if (t.contains("id") && t["id"] == index){
            int customerId = t.value("customerId", -1);  //*
            int totalCost   = t.value("totalCost", 0.0);  //*

            //TODO: Input validation
            retrieveCustomer(customerId, settings);
            Transaction transactionAdd(&p1, &customers.at(customerId), totalCost);
            transactionAdd.completeTransaction();

            int itemId = 0;
            int itemPurchaseQuantity = 0;
            int itemPurchasePrice = 0;

            vector<ItemIn> items;
            if (t.contains("items") && t["items"].is_array()) { //*
                auto itemsVec = t["items"].get<std::vector<nlohmann::json>>(); //*
                for (const auto& item : itemsVec){ //*
                    if (itemsVec.size() >= 4) {  //*
                        itemId = itemsVec[0].get<int>();
                        itemPurchaseQuantity = itemsVec[1].get<int>();
                        itemPurchasePrice = itemsVec[2].get<int>();

                        if (auto itemObjOpt = findItem(itemId)) { //*
                            Item itemObj = *itemObjOpt;
                            ItemIn item(&itemObj);
                            item.purchaseQuantity = itemPurchaseQuantity;
                            item.purchasePrice = itemPurchasePrice;
                            items.push_back(move(item));
                        }
                    }

                }
            transactionAdd.setItems(items);
            }

            transactionArchives.emplace(index, transactionAdd);
            transactionArchivesAge.push(index);

            // If there are too many transactions in memory, the oldest one is archived
            if (transactionArchives.size() > settings.getMaxTransactions()) {
                while (!transactionArchivesAge.empty()){
                    if (transactionArchives.empty()) {
                        cerr << "transactionArchivesAge > transactionArchives";
                        transactionArchivesAge.pop();
                        break;
                    }

                    int removeIndex = transactionArchivesAge.front();
                    transactionArchivesAge.pop();

                    if (!checkTransaction(removeIndex)) {
                        cerr << "transactionArchive could not be removed";
                        continue;
                    }

                    transactionArchives.erase(removeIndex);
                    return true;

                }
            }

            return true;
        }

    }

    return false;
}

// Very similar to checkTransaction, but with customers instead.
bool checkCustomer(int index){
    if (customers.count(index)) return true;
    std::ifstream f("data/customers.json");
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

    for (const auto& t : data)
    {
        if (t.contains("id") && t["id"] == index) return true;
    }

    return false;

}
// Very similar to retrieveTransaction, but with customers instead.
bool retrieveCustomer(int index, Settings &settings){
    //TODO: Input validation can be addedhen methods are not called manually
    // if (index < 0 || index > numCustomers) return false;
    // if (!checkCustomer(index)) return false;
    // if (index > settings.getNumCustomers() || index < 0) return false;
    // if (customers.count(index)) return true;
    // if (!checkCustomer(index)) return false;

    ifstream f("data/customers.json");
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
    vector<int> transactionsAdd;

    for (const auto& t : data){
        if (t.contains("id") && t["id"] == index){
            int id = t.value("id", -1);
            string firstName = t.value("firstName", "NULL");
            string lateName = t.value("lastName", "NULL");
            string phoneNumber = t.value("phoneNumber", "0000000000");
            int credit = t.value("credit", 0);
            if (t.contains("transactions") && t["transactions"].is_array()){
                transactionsAdd = t["transactions"].get<vector<int>>();
            }

            customers.emplace(index, Customer(id, firstName, lateName, phoneNumber));
            customersAge.push(index);
            if (credit > 0) customers.at(index).setCredit(credit);
            if (transactionsAdd.size()) customers.at(index).setTransactions(transactionsAdd);

            if (customers.size() > settings.getMaxCustomers()) {
                while (!customersAge.empty()){
                    if (customers.empty()) {
                        cerr << "customersAge > customers";
                        customersAge.pop();
                        break;
                    }

                    int removeIndex = customersAge.front();
                    customersAge.pop();

                    if (!checkCustomer(removeIndex)) {
                        cerr << "transactionArchive could not be removed";
                        continue;
                    }

                    customers.erase(removeIndex);
                    continue;

                }
            }

        }
    }
    return true;
}



// Very similar to archiveTransaction, but with customers instead.
bool archiveCustomer(int index, Settings &settings){

    //TODO: Input validation can be added when methods are not called manually.
    // if (index < 0 || index > numCustomers) return false;
    // if (!checkCustomer(index)) return false;
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
    Customer &customer = customers.at(index); //*

    // Convert to JSON
    json t = customer; //*

    // Append
    data.push_back(t); //*

    // Rewrite file
    std::ofstream out("data/customers.json"); //*

    out << data.dump(4); //*

    settings.addNumCustomers();
    return true;
}

bool removeCustomerArchive(int index, Settings &settings)
{
    if (index < 0 || index > numCustomers) return false;

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

//TODO: Make this smoother.
bool updateCustomer(int index, Settings &settings)
{
    if (removeCustomerArchive(index, settings) && archiveCustomer(index, settings)) return true;
    return false;
}
optional<Item> findItem(int id){
    if (!items.count(id)) return nullopt;
    return items.at(id);
}



Customer createCustomerInput(){
    string firstName, lastName, phoneNumber;

    while (true)
    {
        firstName = userInput<string>("Please enter your first name\n(E.G. Jonas):");

        if (!firstName.empty()) break;

        cout << "Invalid input!getter" << endl;
    }

    for (int i = 0; i < firstName.length(); i++)
    {
        firstName[i] = (!i) ? toupper(firstName[i]) : tolower(firstName[i]);
    }

    while (true)
    {
        lastName = userInput<string>("Please enter your last name\n(E.G. Hemmett):");

        lastName.erase(std::remove(lastName.begin(), lastName.end(), ' '), lastName.end());

        if (!lastName.empty()) break;

        cout << "Invalid input!getter" << endl;
    }

    // I added this to account for last names like McNee where 2 characters are capitalized.
    if (lastName.length() >= 2 && tolower(lastName[0]) == 'm' && tolower(lastName[1]) == 'c')
    {
        for (int i = 0; i < lastName.length(); i++)
        {
            lastName[i] = (!i || i == 2 ) ? toupper(lastName[i]) : tolower(lastName[i]);
        }

    } else if (lastName.length() >= 3 && tolower(lastName[0]) == 'v' && tolower(lastName[1]) == 'o'  && tolower(lastName[1]) == 'n'){
        for (int i = 0; i < lastName.length(); i++)
        {
            lastName[i] = (!i || i == 3) ? toupper(lastName[i]) : tolower(lastName[i]);
        }
    }


    else {
        for (int i = 0; i < lastName.length(); i++)
        {
            lastName[i] = (!i) ? toupper(lastName[i]) : tolower(lastName[i]);
        }

    }


    while (true)
    {
        int allChar = true;
        phoneNumber = userInput<string>("Please enter your phone number\n(Enter as 10 or 11 digits):");

        for (char c : phoneNumber)
        {
            if (!isdigit(c)) allChar = false;
        }

        if (allChar)
        {
            if (phoneNumber.length() == 10) phoneNumber.insert(phoneNumber.begin(), '1');
            if (phoneNumber.length() == 11)  break;
        }

        cout << "Phone: " << phoneNumber << endl;
        cout << "Invalid input!getter" << endl;
    }

    //TODO: Require ID
    return Customer(0, firstName, lastName, phoneNumber);
// End of createCustomer
}

void CreateCustomer(Customer& customer)
{

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
