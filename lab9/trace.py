import numpy as np


data = np.array([[1, 2, 3],
                 [4, 5, 6],
                 [7, 8, 9]])  # Создание матрицы

trace_value = np.trace(data)  # Вычисление суммы элементов на главной диагонали
print(trace_value)  # Вывод результата