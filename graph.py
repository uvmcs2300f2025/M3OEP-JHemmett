import matplotlib.pyplot as plt
import json
with open('data/customers.json', 'r') as file:
    customers = json.load(file)


# Lambda function sort helper
getNumTransactions = lambda c: len(c["transactions"])
customers.sort(key = getNumTransactions, reverse = True)

# https://www.geeksforgeeks.org/python/read-json-file-using-python/

names = [c["firstName"] for c in customers[:3]]
ids = [c["id"] for c in customers[:3]]

sales = [len(c["transactions"]) for c in customers[:3]]
plt.title("Top Customers")
plt.xlabel('Customer')

plt.bar(names[:3], sales[:3])

# https://www.geeksforgeeks.org/python/matplotlib-pyplot-xticks-in-python/

nr = range(len(names[:3]))
plt.xticks(nr, [f"{names[i]}\n{ids[i]}" for i in nr])

plt.show()
