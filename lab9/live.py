# Игра жизнь

import numpy as np 
import os
import time

class Glive:
    def __init__(
        self, width: int = 16, height: int = 10, speed: int = 1
    ) -> None:
        self.width = width  # Ширина поля
        self.hiqht = height  # Высота поля
        self.speed = speed  # Время задержки после каждого хода

    def cr_rand_grid(self) -> None:
        """Генерирует радомную матрицу с нулями и единицами (для начала игры)"""
        self.grid = np.random.randint(2, size=(self.hiqht, self.width))
        
    def is_alive(self, x, y)-> int:
        """Определяет, будет ли клетка жива в следущем поколении"""
        x_start = max(0, x - 1)  # Определяем значение верхней границы для предотвращения выхода за пределы массива
        x_end = min(self.hiqht, x + 2)  # Определяем значение нижней --||--
        y_start = max(0, y - 1)  # Определяем значение левой границы --||--
        y_end = min(self.width, y + 2)  # Определяем значение правой границы --||--

        s = self.grid[x_start:x_end, y_start:y_end].sum() - self.grid[x][y] 
        # Находим сумму соседей(то есть сколько соседей - живые клетки)

        if s == 3: return 1   # Клетка рождается, если соседей трое
        elif s == 2: return self.grid[x][y]  # Не изменяется, если соседей двое
        else: return 0  # В остальных случаях клетка умирает
    
    def get_new_gen(self) -> None:
        """Производит 'один ход игры'(пересчитывает матрицы для следующего поколения)"""
        vectorized_func = np.vectorize(self.is_alive)  # Функция делает возможным применять is_alive на многомерных массивах
        rows, cols = np.indices(self.grid.shape)  # Создаем массивы индексов
        self.grid = vectorized_func(rows, cols)   # Применяем функцию для каждого элемента матрицы

    def clear_console(self) -> None:
        """Очищает консоль(для вывода)"""
        os.system('cls' if os.name == 'nt' else 'clear')
    
    def run(self) -> None:
        self.cr_rand_grid()  # Генерируем стартовую матрицу
        while True:
            for row in self.grid:
                print(' '.join(map(str, row)))  # Выводим текущую матрицу в консоль
            self.get_new_gen()  # Получаем новое поколение
            time.sleep(self.speed)  # задержка отображения
            self.clear_console()  # Очищение консоли


live = Glive(height=3, width=3, speed=3)
live.run()