# Nature Inspired Algorithms

This repository contains laboratory assignments for the *Nature-Inspired Algorithms* course.  
Each laboratory (`lab_01`, `lab_02`, etc.) is implemented as a separate CMake module in C++, with its own source code, tests, and result outputs.

---

## Laboratory 01 — Random Number Generators and Monte Carlo Experiment

**Lab 01** focuses on exploring pseudorandom number generators and empirical data visualization.  
It includes:

1. **Histogram of Uniform Distribution**
2. **Histogram of Normal (Gaussian) Distribution**
3. **Monte Carlo Experiment — Estimation of Circle Area**

---

## Laboratory 02 — Local Search (First Improvement)

**Lab 02** explores local search optimization techniques.
It includes an implementation of the **First Improvement Local Search** algorithm applied to various dimensions, running multiple times to analyze the average optimization behavior.

---

## Laboratory 03 — Local Search & Simulated Annealing

**Lab 03** extends the local search by introducing **Simulated Annealing**.
It compares the performance of a standard local search with simulated annealing, exploring neighborhoods and evaluating solution spaces.

---

## Laboratory 04 — Genetic Algorithm

**Lab 04** introduces evolutionary computation with the implementation of a **Genetic Algorithm (GA)**.
It covers population initialization, crossover, mutation, and selection mechanisms.

---

## Laboratory 05 — Benchmark Functions Evaluation

**Lab 05** visualizes and evaluates standard continuous optimization benchmark functions, including:
1. **Generalized Rosenbrock**
2. **Salomon**
3. **Whitley**

---

## Laboratory 08 — Non-Dominated Sorting (Kung's Algorithm)

**Lab 08** focuses on multi-objective optimization by implementing:
1. **Naive Non-Dominated Sorting**
2. **Kung's Algorithm** (a more efficient approach)
It includes experiments in 2D and 5D spaces and ranks points into Pareto fronts.

---

## Laboratory 09 — NSGA-II (Non-dominated Sorting Genetic Algorithm II)

**Lab 09** introduces the popular **NSGA-II** algorithm for multi-objective optimization.
Features include:
- Fast non-dominated sorting
- Crowding distance calculation
- Simulated Binary Crossover (SBX)
- Polynomial mutation
- Evaluation on the **ZDT1** test problem

---

## Laboratory 10 — NSGA-II on ZDT Benchmark Suite

**Lab 10** applies the NSGA-II implementation to a wider range of multi-objective benchmark problems.
It explores the algorithm's performance on **ZDT1, ZDT2, ZDT3, ZDT4, and ZDT6** across different dimensions (10, 30, 50).
