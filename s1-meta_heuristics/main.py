import random

# Define fitness function
def fitness(chromosome):
    return (chromosome[0] + chromosome[1]) - (chromosome[2] + chromosome[3]) + (chromosome[4] + chromosome[5]) - (chromosome[6] + chromosome[7])

# Initial population (chromosomes)
population = {
    "x1": [6, 5, 4, 1, 3, 5, 3, 2],
    "x2": [8, 7, 1, 2, 6, 6, 0, 1],
    "x3": [2, 3, 9, 2, 1, 2, 8, 5],
    "x4": [4, 1, 8, 5, 2, 0, 9, 4]
}

# Step A: Calculate fitness and sort
fitness_values = {name: fitness(chromosome) for name, chromosome in population.items()}
sorted_population = sorted(fitness_values.items(), key=lambda x: x[1], reverse=True)

# Display sorted population by fitness
print("Population sorted by fitness (from highest to lowest):")
for name, fit in sorted_population:
    print(f"{name}: {population[name]} with fitness = {fit}")

# Step B: Perform crossover operations
# Single-point crossover at the middle
def single_point_crossover(parent1, parent2):
    point = len(parent1) // 2
    child1 = parent1[:point] + parent2[point:]
    child2 = parent2[:point] + parent1[point:]
    return child1, child2

# Two-point crossover at points 2 and 5
def two_point_crossover(parent1, parent2):
    point1, point2 = 2, 5
    child1 = parent1[:point1] + parent2[point1:point2] + parent1[point2:]
    child2 = parent2[:point1] + parent1[point1:point2] + parent2[point2:]
    return child1, child2

# Uniform crossover
def uniform_crossover(parent1, parent2):
    child1, child2 = [], []
    for gene1, gene2 in zip(parent1, parent2):
        if random.random() > 0.5:
            child1.append(gene1)
            child2.append(gene2)
        else:
            child1.append(gene2)
            child2.append(gene1)
    return child1, child2

# Step C: Crossover of the most fit individuals
# Selecting most fit (sorted_population[0] and sorted_population[1])
most_fit1 = population[sorted_population[0][0]]
most_fit2 = population[sorted_population[1][0]]

# Apply single-point crossover
child1_sp, child2_sp = single_point_crossover(most_fit1, most_fit2)
print("\nSingle-point crossover:")
print("Child 1:", child1_sp)
print("Child 2:", child2_sp)

# Apply two-point crossover
child1_tp, child2_tp = two_point_crossover(most_fit1, most_fit2)
print("\nTwo-point crossover:")
print("Child 1:", child1_tp)
print("Child 2:", child2_tp)

# Apply uniform crossover
child1_uc, child2_uc = uniform_crossover(most_fit1, most_fit2)
print("\nUniform crossover:")
print("Child 1:", child1_uc)
print("Child 2:", child2_uc)

# Step F: Evaluate fitness of new population
new_population = [child1_sp, child2_sp, child1_tp, child2_tp, child1_uc, child2_uc]
new_fitness_values = [fitness(chromosome) for chromosome in new_population]

# Display fitness values of new population
print("\nFitness values of new population:")
for i, fit in enumerate(new_fitness_values):
    print(f"Child {i+1} fitness: {fit}")

# Step G: Optimal solution and feasibility check without mutation
# Calculate maximum fitness with ideal chromosome
optimal_chromosome = [9, 9, 0, 0, 9, 9, 0, 0]
optimal_fitness = fitness(optimal_chromosome)
print("\nOptimal solution (ideal chromosome):", optimal_chromosome)
print("Optimal fitness:", optimal_fitness)

# Check if any child in new population has achieved optimal fitness
achieved_optimal = any(fit == optimal_fitness for fit in new_fitness_values)
print("\nHas the new population achieved the optimal solution?", "Yes" if achieved_optimal else "No")
