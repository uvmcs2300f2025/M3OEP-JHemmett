import matplotlib.pyplot as plt

sales = [1, 2, 3, 6, 7]
names = ["J", "O", "N", "A", "S"]
plt.title("Top Customers")
plt.xlabel('Customer')

plt.bar(names[:3], sales[:3])
plt.show()
