import os
import matplotlib.pyplot as plt
import pandas as pd

BASE_DIR = os.path.dirname(os.path.abspath(__file__))

RUN_NUM = 100 # amount of independent algorithm runs - size of row
dimensions = [15]

results_dir = os.path.join(BASE_DIR, "..", "results")
files = [
    os.path.join(results_dir, "F1_real"),
    os.path.join(results_dir, "F2_real"),
    os.path.join(results_dir, "F3_real")
]

for file in files:
    for n in dimensions:
        csv_path = file + ".csv"

        if not os.path.exists(csv_path):
            print(f"Skipping (not found): {csv_path}")
            continue

        filename = os.path.splitext(os.path.basename(file))[0]
        df = pd.read_csv(csv_path, header=None)
        results_mean = df.mean(axis=1)
        evaluations = [i * RUN_NUM for i in range(len(results_mean))]

        plt.figure(figsize=(10, 6))
        plt.plot(evaluations, results_mean, label=f'Mean (n={n})', color='blue')

        plt.title(f'Genetic Algorithm Average - {filename}')
        plt.xlabel('Liczba ewaluacji')
        plt.ylabel('Średni Wynik (Mean Fitness)')
        plt.legend()
        plt.grid(True, linestyle='--', alpha=0.7)

        output_file = os.path.join(BASE_DIR, f'{filename}.png')
        plt.savefig(output_file)
        plt.close()
        print(f'Wykres zapisany: {output_file}')