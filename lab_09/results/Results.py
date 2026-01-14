import pandas as pd
import matplotlib.pyplot as plt
import sys
import os

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
csv_file = os.path.join(BASE_DIR, "zdt1_results.csv")

def generate_plot():
    output_image = 'zdt1_pareto.png'

    if not os.path.exists(csv_file):
        print(f"Błąd: Nie znaleziono pliku '{csv_file}'.")
        print("Uruchom najpierw program w C++, aby wygenerować wyniki.")
        sys.exit(1)

    try:
        data = pd.read_csv(csv_file)
    except Exception as e:
        print(f"Błąd podczas wczytywania CSV: {e}")
        sys.exit(1)

    plt.figure(figsize=(10, 8), dpi=150) # Wysoka rozdzielczość

    plt.scatter(data['f1'], data['f2'],
                s=15,
                c='blue',
                marker='o',
                edgecolors='none',
                alpha=0.6,
                label='NSGA-II Solutions')

    plt.title('ZDT1 Pareto Front Approximation (NSGA-II)', fontsize=14)
    plt.xlabel('$f_1$', fontsize=12)
    plt.ylabel('$f_2$', fontsize=12)
    plt.xlim(0, 1.05)
    plt.ylim(0, 1.05)
    plt.grid(True, linestyle='--', alpha=0.7)
    plt.legend()
    plt.savefig(output_image, bbox_inches='tight')
    print(f"Sukces! Wykres został zapisany jako '{output_image}'.")


if __name__ == "__main__":
    generate_plot()