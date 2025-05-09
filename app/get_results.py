import os

root_folder = os.path.dirname(os.path.dirname(os.path.realpath(__file__)))
path = root_folder + "/results/nist_test/"

results_files = []

for dir in os.listdir(path):
    p = os.path.join(path, dir)
    if os.path.isdir(p):
        results_files.append(os.path.join(p + "/result.txt"))

results_files.sort()

for results in results_files:
    with open(results, 'r') as file:
        print(f"{os.path.basename(os.path.dirname(results))}: {file.readlines()[-1]}")
