import random

def fitness(chromo):
    return ((chromo[0]+chromo[1]) - (chromo[2]+chromo[3]) + (chromo[4]+chromo[5]) - (chromo[6]+chromo[7]))

population = {
    "x1": [6, 5, 4, 1, 3, 5, 3, 2],
    "x2": [8, 7, 1, 2, 6, 6, 0, 1],
    "x3": [2, 3, 9, 2, 1, 2, 8, 5],
    "x4": [4, 1, 8, 5, 2, 0, 9, 4]
}

fitness_values = {
    name: fitness(chromosome) for name, chromosome in population.items()
}

sorted_population = sorted(fitness_values.items(), key=lambda x:x[1], reverse=True)

print("\nPopulation sorted by fitness (from highest to lowest): ")
for name, fit in sorted_population:
    print(f"{name}: {population[name]} => fitness = {fit}")
print(", ".join(f"{name[0]}" for name in sorted_population))

#perform crossover operations
#single-point (middle) crossover
def middle_crossover(parent1, parent2):
    middle = len(parent1) // 2
    child1 = parent1[:middle] + parent2[middle:]
    child2 = parent2[:middle] + parent1[middle:]
    return child1, child2

#two-points crossover
def two_points_crossover(parent1, parent2):
    point1, point2 = 2, 6
    child1 = parent1[:point1] + parent2[point1:point2] + parent1[point2:]
    child2 = parent2[:point1] + parent1[point1:point2] + parent2[point2:]
    return child1, child2

# uniform crossover
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

most_fit1 = population[sorted_population[0][0]]
most_fit2 = population[sorted_population[1][0]]
most_fit3 = population[sorted_population[2][0]]
most_fit4 = population[sorted_population[3][0]]

child1_mp, child2_mp = middle_crossover(most_fit1, most_fit2)
child1_tp, child2_tp = two_points_crossover(most_fit2, most_fit3)
child1_uc, child2_uc =  uniform_crossover(most_fit1, most_fit3)


# new population
new_population = {
    "y1": child1_mp, "y2": child2_mp, "y3": child1_tp,
    "y4": child2_tp, "y5": child1_uc, "y6": child2_uc
}
new_fitness_values = {
    name: fitness(chromosome) for name, chromosome in new_population.items()
}
new_sorted_population = sorted(new_fitness_values.items(), key=lambda x:x[1], reverse=True)

print("\n\nPopulation sorted by fitness (from highest to lowest): ")
for name, fit in new_sorted_population:
    print(f"{name}: {new_population[name]} => fitness = {fit}")
print(", ".join(f"{name[0]}" for name in new_sorted_population))
