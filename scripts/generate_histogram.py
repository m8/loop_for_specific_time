from cProfile import label
from turtle import color
import matplotlib.pyplot as plt
from numpy.polynomial.polynomial import polyfit
import pandas as pd

data = pd.read_csv('res/function_comparision.csv')

f1 = data.loc[data['function'] == 1]
f2 = data.loc[data['function'] == 2]

plt.scatter(f1["input_size"], f1["clock_cycle"], color="b", label="function 1 - epoch")
plt.scatter(f2["input_size"], f2["clock_cycle"], color="g", label="function 2 - mod")


b, m = polyfit(f1["input_size"], f1["clock_cycle"], 1)
b2, m2 = polyfit(f2["input_size"], f2["clock_cycle"], 1)

plt.plot(f1["input_size"], f1["clock_cycle"],'.')
plt.plot(f1["input_size"], b + m * f1["input_size"], '-')


plt.plot(f2["input_size"], f2["clock_cycle"],'.')
plt.plot(f2["input_size"], b2 + m2 * f2["input_size"], '-')


plt.legend()
plt.tight_layout()
plt.show()
