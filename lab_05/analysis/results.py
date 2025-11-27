import os

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

functions = ["GeneralizedRosenbrock", "Salomon", "Whitley"]
results_dir = os.path.join("..", "results")
output_dir = os.path.dirname(__file__)

for func_name in functions:
    csv_file = os.path.join(results_dir, f"{func_name}.csv")
    df = pd.read_csv(csv_file)

    x_unique = np.sort(df['x'].unique())
    y_unique = np.sort(df['y'].unique())
    X, Y = np.meshgrid(x_unique, y_unique)
    F = df.pivot(index='y', columns='x', values='f').values

    fig = plt.figure(figsize=(8, 6))
    ax = fig.add_subplot(111, projection='3d')
    ax.plot_surface(X, Y, F, cmap='viridis', edgecolor='none')
    ax.set_xlabel('x')
    ax.set_ylabel('y')
    ax.set_zlabel('f(x, y)')
    ax.set_title(func_name)

    plt.tight_layout()

    png_file = os.path.join(output_dir, f"{func_name}.png")
    plt.savefig(png_file, dpi=300)
    plt.close(fig)

    print(f"Zapisano wykres {func_name} do {png_file}")
