import argparse
import subprocess
import os

import matplotlib.pyplot as plt
import seaborn as sns
import plotly.express as px
import numpy as np
from scipy.stats import norm

root_folder = os.path.dirname(os.path.dirname(os.path.realpath(__file__)))

class DrawChart:
    def __init__(self, file_name, generator_name, data_size, seed, chart_type):
        self.file_name = file_name
        self.generator_name = generator_name
        self.data_size = data_size
        self.seed = seed
        self.chart_type = chart_type

    def draw_chart(self):
        if self.chart_type == "histogram":
            self.draw_histogram()
        elif self.chart_type == "heat_map":
            self.draw_heat_map()
        elif self.chart_type == '3d_scattering':
            self.scattering_3d()
        elif self.chart_type == 'random_walk':
            self.random_walk()
        elif self.chart_type == 'rgb':
            self.rgb()
        else:
            raise ValueError("Wrong chart type")

    def generate_data(self, data_size, seed):
        call_args = [f"{root_folder}/build/bin/CHARTS", self.file_name, self.generator_name, str(data_size), str(seed)]
        popen = subprocess.call(call_args, stdout=subprocess.PIPE)
        if popen != 0:
            raise ValueError("Fail generate data")
        data = []
        with open(self.file_name, 'r') as file:
            data = [int(line.strip()) for line in file]
        return np.array(data)

    def draw_histogram(self):
        data = self.generate_data(self.data_size, self.seed)
        plt.hist(data, bins=1000, color='#2ecc71', edgecolor='black')
        plt.title("Распределение случайных чисел")
        plt.xlabel("Значение")
        plt.ylabel("Частота")
        plt.grid(alpha=0.3)
        plt.show()

    def draw_heat_map(self):
        data = self.generate_data(self.data_size, self.seed)
        mid = int(len(data) / 2)
        sns.jointplot(x=data[:mid], y=data[mid:], kind='hex', color='#3498db')
        plt.suptitle("Совместное распределение пар чисел")
        plt.show()

    def scattering_3d(self):
        data = self.generate_data(self.data_size, self.seed)
        fig = plt.figure()
        ax = fig.add_subplot(111, projection="3d")
        part = int(len(data) / 3)
        ax.scatter(data[:part], data[part:2*part], data[2*part:])
        plt.show()

    def rng_choise(self, data):
        indices = np.array(data) % 2
        steps = np.where(indices == 0, -1, 1)
        return steps

    def random_walk(self):
        data = self.generate_data(self.data_size, self.seed)
        steps = self.rng_choise(data)
        position = np.cumsum(steps)

        # 1. График траектории
        plt.figure(figsize=(12, 5))
        plt.plot(position)
        plt.title(f"Random Walk using {self.generator_name}")
        plt.xlabel("Step number")
        plt.ylabel("Position")
        plt.grid()
        plt.show()

        # 2. Проверка среднего и дисперсии
        mean_position = np.mean(position)
        var_position = np.var(position)

        print(f"Среднее положения: {mean_position:.4f}")
        print(f"Дисперсия положения: {var_position:.4f} (ожидается ~ {self.data_size})")

        # 3. Гистограмма конечных положений по многим экспериментам
        n_experiments = 5000
        final_positions = []

        data = self.generate_data((self.data_size + 1) * n_experiments, self.seed)
        for i in range(n_experiments):
            indices = np.array(data[(i + 1) * self.data_size:(i + 2) * self.data_size]) % 2
            steps = self.rng_choise(indices)
            position = np.cumsum(steps)
            final_positions.append(position[-1])

        final_positions = np.array(final_positions)

        # Построение гистограммы
        plt.figure(figsize=(12, 5))
        plt.hist(final_positions, bins=50, density=True, alpha=0.6, color='g')

        # Наложим нормальное распределение
        mu, std = np.mean(final_positions), np.std(final_positions)
        xmin, xmax = plt.xlim()
        x = np.linspace(xmin, xmax, 100)
        p = norm.pdf(x, mu, std)
        plt.plot(x, p, 'k', linewidth=2)
        plt.title(f"Гистограмма конечных положений\n(Random Walk, {self.generator_name})")
        plt.xlabel(f"Конечная позиция после {self.data_size} шагов")
        plt.ylabel("Плотность вероятности")
        plt.grid()
        plt.show()

        print(f"Среднее конечное положение: {mu:.4f}")
        print(f"Стандартное отклонение: {std:.4f} (ожидается ~ sqrt({self.data_size}) = {np.sqrt(self.data_size):.2f})")

        # 4. Расчет автокорреляции шагов
        lags = 50  # количество лагов для автокорреляции
        autocorr = [np.corrcoef(steps[:-lag], steps[lag:])[0, 1] for lag in range(1, lags + 1)]

        # График автокорреляции
        plt.figure(figsize=(12, 5))
        plt.plot(range(1, lags + 1), autocorr, marker='o', linestyle='-', color='b')
        plt.title("Автокорреляция шагов случайного блуждания")
        plt.xlabel("Лаг")
        plt.ylabel("Автокорреляция")
        plt.grid()
        plt.show()

        # Проверим, насколько автокорреляция близка к 0 для больших лагов
        print(f"Автокорреляция для лагов (1, 10, 50): {autocorr[0]:.4f}, {autocorr[9]:.4f}, {autocorr[49]:.4f}")


    def uint32_to_rgb_image(self, width=None, height=None, alpha=False):
        """
        Преобразует массив uint32 в RGB-изображение.

        Args:
            data: numpy-массив типа np.uint32
            width: ширина изображения (опционально)
            height: высота изображения (опционально)

        Returns:
            image: np.ndarray с формой (H, W, 3)
        """

        data = self.generate_data(width * height, self.seed)
        r = data & 0xFF
        g = (data >> 8) & 0xFF
        b = (data >> 16) & 0xFF
        if alpha:
            a = (data >> 24) & 0xFF

        colors = None
        if alpha:
            colors = np.stack((r, g, b, a), axis=1)
        else:
            colors = np.stack((r, g, b), axis=1)


        # Если размер не указан, делаем квадрат
        if width is None or height is None:
            size = int(np.ceil(np.sqrt(len(colors))))
            width = height = size

        image = colors.reshape((height, width, 3 + int(alpha)))

        return image
    
    def rgb(self):
        """
        Преобразует массив uint32 в RGB-изображение.
        """
        width = 256
        height = 256
        img = self.uint32_to_rgb_image(width, height)
        plt.figure(figsize=(6, 6))
        plt.imshow(img)
        plt.axis('off')
        plt.title(f'Визуализация uint32 последовательности, размер {width}*{height}: {self.generator_name}')
        plt.show()

def main():
    parser = argparse.ArgumentParser(description='Generate charts for the PRNG')
    parser.add_argument(
        '--file_name',
        type=str,
        help='The name of the file with PRNG data'
    )
    parser.add_argument(
        '--generator_name',
        type=str,
        help='The name of the PRNG'
    )
    parser.add_argument(
        '--data_size',
        type=int,
        help='Data size'
    )
    parser.add_argument(
        '--seed',
        type=int,
        help='Seed for PRNG'
    )
    parser.add_argument(
        '--chart_type',
        type=str,
        choices=['histogram', 'heat_map', '3d_scattering', 'random_walk', 'rgb'],
        help='The type of chart to generate'
    )
    args = parser.parse_args()
    print("Argument values:")
    print(args.file_name)
    print(args.generator_name)
    print(args.data_size)
    print(args.seed)
    print(args.chart_type)

    if args.file_name is None or args.generator_name is None or args.data_size is None or args.seed is None or args.chart_type is None:
        print("Please provide all required arguments")
        return 1
    if args.data_size <= 0:
        print("Data size must be greater than 0")
        return 1
    if args.seed < 0:   
        print("Seed must be greater than or equal to 0")
        return 1
    if args.file_name == "":
        print("File name must not be empty")
        return 1
    if args.generator_name == "":
        print("Generator name must not be empty")
        return 1
    if args.chart_type == "":
        print("Chart type must not be empty")
        return 1

    draw_chart = DrawChart(args.file_name, args.generator_name, args.data_size, args.seed, args.chart_type)
    draw_chart.draw_chart()
    return 0

if __name__ == '__main__':
    main()
