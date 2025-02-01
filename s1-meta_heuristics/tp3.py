import numpy as np
import random

def calculate_cost(flow_matrix, distance_matrix, solution):
    n = len(solution)
    cost = 0
    for i in range(n):
        for j in range(n):
            cost += flow_matrix[i][j] * distance_matrix[solution[i]][solution[j]]
    return cost

def tabu_search(flow_matrix, distance_matrix, max_iterations=10, tabu_list_size=7):
    n = len(flow_matrix)

    # Step 1: Initialization
    current_solution = list(range(n))
    random.shuffle(current_solution)
    best_solution = current_solution[:]
    best_cost = calculate_cost(flow_matrix, distance_matrix, best_solution)
    tabu_list = []

    for iteration in range(max_iterations):
        # Step 2: Generate neighbors
        neighbors = []
        for i in range(n):
            for j in range(i + 1, n):
                neighbor = current_solution[:]
                neighbor[i], neighbor[j] = neighbor[j], neighbor[i]
                neighbors.append((neighbor, (i, j)))

        # Step 3: Evaluate neighbors
        best_neighbor = None
        best_neighbor_cost = float('inf')
        best_swap = None
        
        for neighbor, swap in neighbors:
            if swap not in tabu_list:
                neighbor_cost = calculate_cost(flow_matrix, distance_matrix, neighbor)
                if neighbor_cost < best_neighbor_cost:
                    best_neighbor = neighbor
                    best_neighbor_cost = neighbor_cost
                    best_swap = swap

        # Update current solution and Tabu list
        if best_neighbor:
            current_solution = best_neighbor
            if best_swap not in tabu_list:
                tabu_list.append(best_swap)
                if len(tabu_list) > tabu_list_size:
                    tabu_list.pop(0)

            # Update the best solution found
            if best_neighbor_cost < best_cost:
                best_solution = best_neighbor
                best_cost = best_neighbor_cost

        # Output the status at each iteration
        print(f"Iteration {iteration + 1}: Current Cost = {best_neighbor_cost}, Best Cost = {best_cost}")

    return best_solution, best_cost

# Matrices from the document
F = np.array([
    [0, 5, 2, 4],
    [5, 0, 3, 6],
    [2, 3, 0, 1],
    [4, 6, 1, 0]
])

D = np.array([
    [0, 10, 2, 8],
    [10, 0, 4, 5],
    [2, 4, 0, 7],
    [8, 5, 7, 0]
])

# Run the Tabu Search
best_solution, best_cost = tabu_search(F, D)
print(f"\nBest Solution: {best_solution}")
print(f"Best Cost: {best_cost}")
