import os
import numpy as np
import argparse

from scipy.stats import chisquare
from scipy.special import gammainc
import matplotlib.pyplot as plt

parser = argparse.ArgumentParser(description='Generate charts for the PRNG')
parser.add_argument(
    '--folder_name',
    type=str,
    help='The name of the folder with p values'
)

args = parser.parse_args()

root_folder = os.path.dirname(os.path.dirname(os.path.realpath(__file__)))
path = root_folder + "/results/nist_test" + args.folder_name

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
    "Serial_Test": "Тест на переодичность",
    "Approximate_Entropy_Test": "Тест на приближенную энтропию",
    "Cumulative_Sums_Test": "Тест на кумулятивные суммы",
    "Random_Excursions_Test": "Тест на произвольное отклонение",
    "Random_Excursions_Variant_Test": "Другой тест на произвольное отклонение"
}

def uniform_p_values(p_values, test_name):
    # 1. Разбиваем на 10 интервалов
    hist, _ = np.histogram(p_values, bins=10, range=(0,1))

    # 2. Ожидаемое количество в каждом интервале
    expected = len(p_values) / 10
    # 3. Хи-квадрат тест
    chi2 = np.sum((hist - expected)**2) / expected
    print(f"Chi-square = {chi2:.4f}")

    # 4. Степени свободы
    # NIST uses: P = igamc(df / 2, chi2 / 2)
    p_uniform = gammainc(9 / 2.0, chi2 / 2.0)

    print(f"P-value of P-values = {p_uniform:.6f}")

    title = f"Гистограмма равномерного распределения p-значений \"{russian_names[test_name]}\""

    if p_uniform > 0.0001:
        title = "✓" + title
        print(f"✅ P-values распределены равномерно (тест `{russian_names[test_name]}` пройден)")
    else:
        title = "✘" + title
        print(f"❌ P-values НЕ равномерны (тест `{russian_names[test_name]}` провален)")

    plt.hist(p_values, bins=20, density=True, edgecolor='black')
    plt.title(title)
    plt.xlabel('Значение')
    plt.ylabel('Плотность вероятности')
    plt.grid(True)
    plt.show()

for filename in os.listdir(path):
   with open(os.path.join(path, filename), 'r') as file:
      print(f"File: {filename}")
      data = [float(line.strip()) for line in file]
      uniform_p_values(data, filename[:-4])
      break
