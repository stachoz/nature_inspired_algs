import pandas as pd
import matplotlib.pyplot as plt
import os

# katalog z plikami CSV
BASE_DIR = os.path.dirname(os.path.abspath(__file__))

# lista wymiarów
dimensions = [2, 5, 10]
for n in dimensions:
    file_path = os.path.join(BASE_DIR,"..", "results", f'results_n{n}.csv')
    df = pd.read_csv(file_path)

    mean_series = df.cummax(axis=0).mean(axis=1)

    plt.figure(figsize=(10, 6))
    plt.plot(mean_series, label=f'n={n}')
    plt.title(f'First Improvement Local Search - n={n}')
    plt.xlabel('Wywołania funkcji oceny')
    plt.ylabel('Najlepsza znaleziona wartość')
    plt.legend()
    plt.grid(True)

    output_file = os.path.join(BASE_DIR, f'local_search_n{n}.png')
    plt.savefig(output_file)
    plt.close()
    print(f'Wykres zapisany: {output_file}')
