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


fitness_values = {name: fitness(chromosome) for name, chromosome in population.items()}
