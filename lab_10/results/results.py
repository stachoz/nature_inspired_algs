import pandas as pd
import matplotlib.pyplot as plt
import os

BASE_DIR = os.path.dirname(os.path.abspath(__file__))

PROBLEMS = ["ZDT1", "ZDT2", "ZDT3", "ZDT4", "ZDT6"]
DIMS = [10, 30, 50]
ITERATIONS = [20, 50, 100, 500]
COLORS = {20: 'red', 50: 'orange', 100: 'blue', 500: 'green'}

def generate_plots():
    if any(f.endswith('.csv') for f in os.listdir(BASE_DIR)):
        data_path = BASE_DIR
    else:
        print(f"error: Nie znaleziono plikow .csv w {BASE_DIR}")
        return

    for prob in PROBLEMS:
        for dim in DIMS:
            csv_name = f"{prob}_dim{dim}.csv"
            csv_path = os.path.join(data_path, csv_name)

            output_image = os.path.join(BASE_DIR, f"plot_{prob}_dim{dim}.png")

            if not os.path.exists(csv_path):
                continue

            try:
                data = pd.read_csv(csv_path)

                plt.figure(figsize=(10, 7), dpi=100)

                found_any_iter = False
                for it in ITERATIONS:
                    subset = data[data['iteration'] == it]
                    if not subset.empty:
                        plt.scatter(subset['f1'], subset['f2'],
                                    s=25,
                                    c=COLORS[it],
                                    label=f'Iteracja {it}',
                                    alpha=0.7,
                                    edgecolors='white',
                                    linewidths=0.5)
                        found_any_iter = True

                if not found_any_iter:
                    plt.close()
                    continue

                plt.title(f'Front Pareto NSGA-II: {prob} (N={dim})', fontsize=14)
                plt.xlabel('$f_1$', fontsize=12)
                plt.ylabel('$f_2$', fontsize=12)
                plt.grid(True, linestyle=':', alpha=0.6)
                plt.legend(loc='upper right')

                plt.savefig(output_image, bbox_inches='tight')
                plt.close()
                print(f"Utworzono: plot_{prob}_dim{dim}.png")

            except Exception as e:
                print(f"error {csv_name}: {e}")

if __name__ == "__main__":
    generate_plots()
    print("\nProces zakonczony.")