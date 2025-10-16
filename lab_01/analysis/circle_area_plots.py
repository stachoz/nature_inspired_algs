import pandas as pd
import matplotlib.pyplot as plt
import os

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
RESULTS_PATH = os.path.join(BASE_DIR, "..", "results", "points.csv")

print(f"Loading data from: {RESULTS_PATH}")

df = pd.read_csv(RESULTS_PATH, header=None, names=['x', 'y', 'inside_flag'])

if not all(col in df.columns for col in ['x', 'y', 'inside_flag']):
    raise ValueError("CSV must contain 3 columns: x, y, inside_flag")

sample_sizes = [1000, 10000, 100000, 1000000]

for n in sample_sizes:
    subset = df.head(n)

    counts = subset['inside_flag'].value_counts().sort_index()
    inside = counts.get(1, 0)
    outside = counts.get(0, 0)

    plt.figure()
    plt.bar(['Inside', 'Outside'], [inside, outside], color=['green', 'red'])
    plt.title(f"Points Inside vs Outside (first {n} points)")
    plt.ylabel("Count")
    plt.grid(axis='y', linestyle='--', alpha=0.7)

    out_path = os.path.join(BASE_DIR, f"histogram_{n}.png")
    plt.savefig(out_path)
    plt.close()

    print(f"Saved: {out_path}")

print("All histograms generated.")
