import os

import matplotlib.pyplot as plt
import pandas as pd

BASE_DIR = os.path.dirname(os.path.abspath(__file__))

dimensions = [10]

results_dir = os.path.join(BASE_DIR, "..", "results")
files = [
    os.path.join(results_dir, "F1_bin"),
    os.path.join(results_dir, "F1_real"),
    os.path.join(results_dir, "F2_bin"),
    os.path.join(results_dir, "F2_real")
]

for file in files:
    for n in dimensions:
        # file_path = os.path.join(BASE_DIR, "..", "results", f'test1-binary-{n}.csv')
        filename = os.path.splitext(os.path.basename(file))[0]
        df = pd.read_csv(file + ".csv")

        evaluations = df.iloc[:, 0]
        results = df.iloc[:, 1]

        plt.figure(figsize=(10, 6))
        plt.plot(evaluations, results, label=f'n={n}', marker='o')
        plt.title(f'Genetic Algorithm - {filename}')
        plt.xlabel('Liczba ewaluacji')
        plt.ylabel('Wynik')
        plt.legend()
        # plt.xscale('log')
        plt.grid(True)

        output_file = os.path.join(BASE_DIR, f'{file}.png')
        plt.savefig(output_file)
        plt.close()
        print(f'Wykres zapisany: {output_file}')
