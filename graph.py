import matplotlib.pyplot as plt
import json

def viewCustomerPurchases(size):
    assert size >= 0

    # https://www.geeksforgeeks.org/python/read-json-file-using-python/
    with open('data/customersTest.json', 'r') as file:
        customers = json.load(file)

    # Lambda function sort helper
    getNumTransactions = lambda c: len(c["transactions"])
    customers.sort(key = getNumTransactions, reverse = True)

    names = [c["firstName"] for c in customers[:size]]
    ids = [c["id"] for c in customers[:size]]
    sales = [len(c["transactions"]) for c in customers[:size]]

    plt.title("Top Customers")
    plt.xlabel('Customer')

    plt.bar(names[:size], sales[:size])

    # https://www.geeksforgeeks.org/python/matplotlib-pyplot-xticks-in-python/

    nr = range(len(names[:3]))
    plt.xticks(nr, [f"{names[i]}\n{ids[i]}" for i in nr])

    plt.show()

def customerPurchasesMenu():
    # Finds the number of customers, used 
    try:
        with open('data/customersTest.json', 'r') as file:
            customers = json.load(file)
            maxCustomers = len(customers)
    except:
        print("Could not open customersTest.json")
        maxCustomers = -1

    choice = None
    while True:
        try:
            if maxCustomers == -1:
                choice = int(input("How many customers?\n(Enter a number): "))
            elif maxCustomers == 0:
                print("There are 0 customers")
                return
            elif maxCustomers == 1:
                choice = 1
                break
            else:
                choice = int(input(f"How many customers?\n(Enter a number between 1 and {maxCustomers}): "))

        except:
            None
        
        if maxCustomers == -1:
            if not(choice < 0):
                break
        else:
            # Defaults to showing all customers
            if not(choice):
                choice = maxCustomers
                break
            if not(choice < 0 or choice > maxCustomers):
                break
        
        print("Invalid Choice")
    viewCustomerPurchases(choice)

def getCustomer(id):
    assert id

    # https://www.geeksforgeeks.org/python/read-json-file-using-python/
    with open('data/customersTest.json', 'r') as file:
        customers = json.load(file)

    customer = None
    for c in customers:
        if c["id"] == id:
            customer = c
            break
    
    if not customer:
        print(f"No customer found with id {id}")
        return

    return customer

def getTransaction(id):
    assert id

    # https://www.geeksforgeeks.org/python/read-json-file-using-python/
    with open('data/transactionsTest.json', 'r') as file:
        transactions = json.load(file)

    transaction = None
    for t in transactions:
        if t["id"] == id:
            transaction = t
            break
    
    if not transaction:
        print(f"No transaction found with id {id}")
        return

    return transaction

def viewCustomer(id):
    customer = getCustomer(id)
    
    if not customer:
        return

    transactions = customer["transactions"]
    plt.xlabel(f"Purchases")

    plt.bar(customer["transactions"], [getTransaction(t)["totalCost"] for t in customer["transactions"]])
    
    plt.subplots_adjust(top=0.75) 
    plt.figtext(0.1, 0.8, f"Name: {customer['firstName']} {customer['lastName']}\nID: {customer['id']}\nStore Credit: {customer['credit']}", ha = "left", va = "bottom", fontsize = 15)

    plt.show()

if __name__ == "__main__":

    choice = None
    while True:
        try:
            choice = int(input("What graphs would you like to view?\n(1: Customer Purchases, 2: Customer info): "))


        except:
            None
        
        if not(choice < 0 or choice > 2):
            break
        
        print("Invalid Choice")
    
    match choice:
        case 1:
            customerPurchasesMenu()
        case 2:
            viewCustomer(1)
    
    