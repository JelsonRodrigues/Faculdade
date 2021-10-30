# Simples script para plotar dados
# Criado em 20/10/2021 por Jelson Stoelben Rodrigues

import matplotlib.pyplot as plt
import pandas as pd

dataframe = pd.read_csv("resultados.csv", index_col="passes")
dataframe['QuickSort'] = pd.to_datetime(dataframe['QuickSort'], format="%M:%S:%f")
dataframe['MergeSort'] = pd.to_datetime(dataframe['MergeSort'], format="%M:%S:%f")
dataframe['InsertionSort'] = pd.to_datetime(dataframe['InsertionSort'], format="%M:%S:%f")
dataframe['SelectionSort'] = pd.to_datetime(dataframe['SelectionSort'], format="%M:%S:%f")
dataframe['BubbleSort'] = pd.to_datetime(dataframe['BubbleSort'], format="%M:%S:%f")
dataframe.plot()
plt.grid()
plt.show()
print(dataframe)