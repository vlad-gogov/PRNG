import os
import numpy as np
import argparse

from scipy.stats import chisquare
from scipy.special import gammaincc
import matplotlib.pyplot as plt

parser = argparse.ArgumentParser(description='Generate charts for the PRNG')
parser.add_argument(
    '--folder_name',
    type=str,
    help='The name of the folder with p values'
)
parser.add_argument(
    '--test_count',
    type=int,
    help='The name of the folder with p values'
)

args = parser.parse_args()

root_folder = os.path.dirname(os.path.dirname(os.path.realpath(__file__)))
path = root_folder + "/results/nist_test/" + args.folder_name

russian_names = {
    "Frequency_Test": "Частотный побитовый тест",
    "Frequency_Block_Test": "Частотный блочный тест",
    "Runs_Test": "Тест на последовательность одинаковых бит",
    "Longest_Run_of_Ones_Test": "Тест на самую длинную последовательность единиц в блоке",
    "Binary_Matrix_Rank_Test": "Тест рангов бинарных матриц",
    "Discrete_Fourier_Transform_Test": "Спектральный тест",
    "Non-overlapping_Template_Matching_Test": "Тест на совпадение неперекрывающихся шаблонов",
    "Overlapping_Template_Matching_Test": "Тест на совпадение перекрывающихся шаблонов",
    "Maurer’s_“Universal_Statistical”_Test": "Универсальный статистический тест Маурера",
    "Linear_Complexity_Test": "Тест на линейную сложность",
    "Serial_Test_16": "Тест на переодичность 16",
    "Serial_Test_15": "Тест на переодичность 15",
    "Approximate_Entropy_Test": "Тест на приближенную энтропию",
    "Cumulative_Sums_Test": "Тест на кумулятивные суммы",
    "Random_Excursions_Test_-4": "Тест на произвольное отклонение -4",
    "Random_Excursions_Test_-3": "Тест на произвольное отклонение -3",
    "Random_Excursions_Test_-2": "Тест на произвольное отклонение -2",
    "Random_Excursions_Test_-1": "Тест на произвольное отклонение -1",
    "Random_Excursions_Test_1": "Тест на произвольное отклонение 4",
    "Random_Excursions_Test_2": "Тест на произвольное отклонение 3",
    "Random_Excursions_Test_3": "Тест на произвольное отклонение 2",
    "Random_Excursions_Test_4": "Тест на произвольное отклонение 1",
    "Random_Excursions_Variant_Test_-9": "Другой тест на произвольное отклонение -9",
    "Random_Excursions_Variant_Test_-8": "Другой тест на произвольное отклонение -8",
    "Random_Excursions_Variant_Test_-7": "Другой тест на произвольное отклонение -7",
    "Random_Excursions_Variant_Test_-6": "Другой тест на произвольное отклонение -6",
    "Random_Excursions_Variant_Test_-5": "Другой тест на произвольное отклонение -5",
    "Random_Excursions_Variant_Test_-4": "Другой тест на произвольное отклонение -4",
    "Random_Excursions_Variant_Test_-3": "Другой тест на произвольное отклонение -3",
    "Random_Excursions_Variant_Test_-2": "Другой тест на произвольное отклонение -2",
    "Random_Excursions_Variant_Test_-1": "Другой тест на произвольное отклонение -1",
    "Random_Excursions_Variant_Test_1": "Другой тест на произвольное отклонение 1",
    "Random_Excursions_Variant_Test_2": "Другой тест на произвольное отклонение 2",
    "Random_Excursions_Variant_Test_3": "Другой тест на произвольное отклонение 3",
    "Random_Excursions_Variant_Test_4": "Другой тест на произвольное отклонение 4",
    "Random_Excursions_Variant_Test_5": "Другой тест на произвольное отклонение 5",
    "Random_Excursions_Variant_Test_6": "Другой тест на произвольное отклонение 6",
    "Random_Excursions_Variant_Test_7": "Другой тест на произвольное отклонение 7",
    "Random_Excursions_Variant_Test_8": "Другой тест на произвольное отклонение 8",
    "Random_Excursions_Variant_Test_9": "Другой тест на произвольное отклонение 9",
} 

def uniform_p_values(p_values, test_name):
    # 1. Разбиваем на 10 интервалов
    hist, _ = np.histogram(p_values, bins=10, range=(0,1))
    print(*hist, sep=", ")

    # 2. Ожидаемое количество в каждом интервале
    expected = args.test_count / 10
    # 3. Хи-квадрат тест
    chi2 = np.sum((hist - expected)**2) / expected
    print(f"Chi-square = {chi2:.4f}")

    # 4. Степени свободы
    # NIST uses: P = igamc(df / 2, chi2 / 2)
    p_uniform = gammaincc(9 / 2.0, chi2 / 2.0)

    print(f"P-value of P-values = {p_uniform:.6f}")

    title = f"Гистограмма равномерного распределения p-значений \"{russian_names[test_name]}\""

    if p_uniform > 0.0001:
        title = "✓" + title
        print(f"✅ P-values распределены равномерно (тест `{russian_names[test_name]}` пройден)")
        plt.hist(p_values, bins=20, density=True, edgecolor='black')
        plt.title(title)
        plt.xlabel('Значение')
        plt.ylabel('Плотность вероятности')
        plt.grid(True)
        plt.show()
    else:
        title = "✘" + title
        print(f"❌ P-values НЕ равномерны (тест `{russian_names[test_name]}` провален)")

for filename in os.listdir(path):
   with open(os.path.join(path, filename), 'r') as file:
      if russian_names.get(filename[:-4]) is None:
          continue
      print(f"File: {filename}")
      data = [float(line.strip()) for line in file]
      uniform_p_values(data, filename[:-4])
