import matplotlib.pyplot as plt

sales = [1, 2, 3, 6, 7]
names = ["J", "O", "N", "A", "S"]
ids = [101, 102, 103, 104, 105]
plt.title("Top Customers")
plt.xlabel('Customer')

plt.bar(names[:3], sales[:3])

# https://www.geeksforgeeks.org/python/matplotlib-pyplot-xticks-in-python/

nr = range(len(names[:3]))
plt.xticks(nr, [f"{names[i]}\n{ids[i]}" for i in nr])

plt.show()
