import random

# Graph edges and vertices
E = {
    "R1": ["v1", "v2"],
    "R2": ["v2", "v3"],
    "R3": ["v3", "v4"],
    "R4": ["v4", "v5"],
    "R5": ["v1", "v5"],
    "R6": ["v2", "v5"],
}

# Generates a random array of 0s and 1s
def elements():
    array = [random.choice([0, 1]) for _ in range(5)]
    return array

# Calculates the maximum number of edges that can be selected based on the vertices
def maxNumberdarete(arr):
    v1, v2, v3, v4, v5 = arr
    count = 0
    if v1 == 1 and v2 == 1:
        count += 1
    if v2 == 1 and v3 == 1:
        count += 1
    if v3 == 1 and v4 == 1:
        count += 1
    if v4 == 1 and v5 == 1:
        count += 1
    if v1 == 1 and v5 == 1:
        count += 1
    if v2 == 1 and v5 == 1:
        count += 1
    return count

# Counts the number of selected vertices (1s) in the array
def minNumberSommets(arr):
    return sum(arr)

# Objective function
def functionObjectf(arr):
    return maxNumberdarete(arr) - 0.5 * minNumberSommets(arr)

# Selects the top two arrays based on their fitness values
def selection(x1, x2, x3, x4):
    f1 = functionObjectf(x1)
    f2 = functionObjectf(x2)
    f3 = functionObjectf(x3)
    f4 = functionObjectf(x4)
    array_values = [(f1, x1), (f2, x2), (f3, x3), (f4, x4)]
    fitness = sorted(array_values, key=lambda x: x[0], reverse=True)
    return fitness[0][1], fitness[1][1], fitness[0][0], fitness[1][0]

# Performs crossover at a given point
def crossOverPoint(arr1, arr2, point):
    array1 = arr1[:point] + arr2[point:]
    array2 = arr2[:point] + arr1[point:]
    return array1, array2

# Mutates the array by randomly flipping 0s and 1s
def mutation(arr, flip_chance):
    for i in range(len(arr)):
        if random.random() < flip_chance:
            arr[i] = 1 - arr[i]  # Toggle between 0 and 1

# Main function to demonstrate the code
def main():
    arr1 = elements()
    arr2 = elements()
    arr3 = elements()
    arr4 = elements()

    print("Initial arrays, and fitness values:")
    print(f"arr1: {arr1} => Fitness of arr1: {functionObjectf(arr1)}")
    print(f"arr2: {arr2} => Fitness of arr2: {functionObjectf(arr2)}")
    print(f"arr3: {arr3} => Fitness of arr3: {functionObjectf(arr3)}")
    print(f"arr4: {arr4} => Fitness of arr4: {functionObjectf(arr4)}")

    # Select the top two arrays
    selected1, selected2, fitness1, fitness2 = selection(arr1, arr2, arr3, arr4)
    print(f"\nArray 1:{selected1} => fitness 1: {fitness1}")
    print(f"Array 2:{selected2} => fitness 2: {fitness2}")

    # Perform crossover at a random point
    crossover_point = random.randint(1, 4)
    child1, child2 = crossOverPoint(selected1, selected2, crossover_point)
    print("\nChildren after crossover:", child1, child2)

    # Mutate children
    flip_chance = 0.5
    mutation(child1, flip_chance)
    mutation(child2, flip_chance)
    print("Children after mutation:", child1, child2)

main()
