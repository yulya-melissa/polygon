
import numpy as np
import pandas as pd
import os

# Создание двумерного массива
data = np.array([[1, 2, 3],
                 [4, 5, 6],
                 [7, 8, 9]])

for row in data:
    print(' '.join(map(str, row))) 
os.system('cls' if os.name == 'nt' else 'clear')

print(np.array_str(data, suppress_small=True))
df = pd.DataFrame(data)

# Вывод DataFrame
print(df.to_string(index=False, header=False)) 