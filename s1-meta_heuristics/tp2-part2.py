# Define the parameters based on the document
schema = "1*0*1"
population_fitness_avg = 10  # Average fitness of the population
schema_fitness_avg = 12      # Average fitness of the schema H
schema_count = 5             # Number of instances of H in generation k
crossover_prob = 0.6         # Probability of crossover
mutation_prob = 0.01         # Probability of mutation for one bit
l = 5                        # Length of the binary strings

print()
# 1. Parameters of the schema
def schema_order(schema):
    # Count the number of fixed bits (not '*')
    return len([bit for bit in schema if bit != '*'])

def defining_length(schema):    
    # Calculate the distance between the first and the last fixed bits
    fixed_positions = [i for i, bit in enumerate(schema) if bit != '*']
    return fixed_positions[-1] - fixed_positions[0]

o_H = schema_order(schema)
delta_H = defining_length(schema)

# 2. Effect of selection
def selection_effect(schema_fitness_avg, population_fitness_avg, schema_count):
    # Calculate the expected number of copies of schema H after selection
    return (schema_fitness_avg / population_fitness_avg) * schema_count

M_H_k = selection_effect(schema_fitness_avg, population_fitness_avg, schema_count)

# 3. Effect of crossover
def crossover_effect(defining_length, crossover_prob, l):
    # Calculate the probability that the schema H is destroyed by crossover
    return (defining_length / (l - 1)) * crossover_prob

D_c_H = crossover_effect(delta_H, crossover_prob, l)

# Probability of survival after crossover
survival_crossover_prob = 1 - D_c_H

# 4. Effect of mutation
def mutation_effect(mutation_prob, schema_order):
    # Calculate the probability that the schema H survives mutation
    return (1 - mutation_prob) ** schema_order

S_m_H = mutation_effect(mutation_prob, o_H)

# 5. Total survival probability
def total_survival(M_H_k, survival_crossover_prob, S_m_H):
    # Calculate the total expected number of copies in the next generation
    return M_H_k * survival_crossover_prob * S_m_H

E_m_H = total_survival(M_H_k, survival_crossover_prob, S_m_H)

# Display the results
print("Order of the schema o(H):", o_H)
print("Defining length δ(H):", delta_H)
print("Expected number of copies after selection M(H, k):", M_H_k)
print("Destruction probability by crossover D_c(H):", D_c_H)
print("Survival probability after crossover:", survival_crossover_prob)
print("Survival probability after mutation S_m(H):", S_m_H)
print("Total expected number of copies in the next generation E[m(H, k+1)]:", E_m_H)
print()
