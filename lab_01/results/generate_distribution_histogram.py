import argparse

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from scipy.stats import norm, uniform


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("csv_file")
    parser.add_argument("--dist", choices=["normal", "uniform"], default="normal")
    parser.add_argument("--bins", type=int, default=30)
    parser.add_argument("--sep", default=",")
    args = parser.parse_args()

    df = pd.read_csv(args.csv_file, header=None, sep=args.sep)
    data = df.iloc[:, 0].dropna()

    plt.figure(figsize=(8, 5))
    plt.hist(data, bins=args.bins, density=True, alpha=0.6, color='skyblue', edgecolor='black')

    x = np.linspace(min(data), max(data), 1000)
    if args.dist == "normal":
        mu, sigma = norm.fit(data)
        plt.plot(x, norm.pdf(x, mu, sigma), 'r-')
    else:
        a, b = min(data), max(data)
        plt.plot(x, uniform.pdf(x, a, b - a), 'g-')

    plt.title(f"Histogram ({args.dist})")
    plt.xlabel("Wartości")
    plt.ylabel("Gęstość prawdopodobieństwa")
    plt.grid(True, alpha=0.3)
    plt.show()


if __name__ == "__main__":
    main()
