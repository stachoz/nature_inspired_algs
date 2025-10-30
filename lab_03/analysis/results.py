import pandas as pd
import matplotlib.pyplot as plt
import os

BASE_DIR = os.path.dirname(os.path.abspath(__file__))

dimensions = [10]

for n in dimensions:
    file_path = os.path.join(BASE_DIR, "..", "results", f'first_10.csv')
    df = pd.read_csv(file_path)

    evaluations = df.iloc[:, 0]
    results = df.iloc[:, 1]

    plt.figure(figsize=(10, 6))
    plt.plot(evaluations, results, label=f'n={n}', marker='o')
    plt.title(f'Simualted Annealing - n={n}')
    plt.xlabel('Liczba ewaluacji')
    plt.ylabel('Wynik')
    plt.legend()
    # plt.xscale('log')
    plt.grid(True)

    output_file = os.path.join(BASE_DIR, f'local_search_n{n}.png')
    plt.savefig(output_file)
    plt.close()
    print(f'Wykres zapisany: {output_file}')

