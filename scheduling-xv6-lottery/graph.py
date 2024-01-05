import pandas as pd
import matplotlib.pyplot as plt

data = pd.read_csv("data.csv", index_col=[0])

# Display all columns
print(data.columns)

# Plot all columns, including those with all zeros
plot = data[['Process10', 'Process20', 'Process30']].plot()


plot.set_ylabel("Process' Ticks")
plot.set_title("Lottery Scheduling Graph with 1:2:3 tickets ratio")
plt.show()
