import matplotlib.pyplot as plt
import json

def viewCustomerPurchases(size):
    assert size >= 0

    # https://www.geeksforgeeks.org/python/read-json-file-using-python/
    with open('data/customers.json', 'r') as file:
        customers = json.load(file)

    if len(customers) > size:
        print("Error: len(customers) > size")
        size = len(customers)
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
    try:
        with open('data/customers.json', 'r') as file:
            customers = json.load(file)
            maxCustomers = len(customers)
    except:
        print("Could not open customers.json")
        maxCustomers = -1

    while True:
        try:
            choice = int(input("How many customers?\n(Enter number of customers): "))
        except:
            None
        
        if maxCustomers == -1:
            if not(choice < 0):
                break
        else:
            if not(choice < 0 or choice > maxCustomers):
                break
        
        print("Invalid Choice")
    viewCustomerPurchases(choice)

plt.show()
if __name__ == "__main__":

    while True:
        try:
            choice = int(input("What graphs would you like to view?\n(1: Customer Purchases): "))


        except:
            None
        
        if not(choice < 0 or choice > 1):
            break
        
        print("Invalid Choice")
    match choice:
        case 1:
            customerPurchasesMenu()
    
    