import pandas as pd
import matplotlib.pyplot as plt

data = pd.read_csv("data.csv")

plot = data.plot()

plot.set_ylabel("Process' Ticks")
plot.set_title("Lottery Schedling Graph with 1:2:3 tickets ratio")
plt.show()
