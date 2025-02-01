package Contraint;

import ilog.concert.IloException;
import ilog.concert.IloNumVar;
import ilog.cplex.IloCplex;

public class TSP {
    private int n; // Number of nodes
        private double[][] distances; // Distance array
    private double capacity; // Vehicle capacity (if applicable)
    private IloCplex model; // CPLEX model instance
    private IloNumVar[][] decisionVariables; // Decision variables for the model

    // Constructor to initialize the TSP problem
    public TSP(int n, double[][] distances) {
        this.n = n;
        this.distances = distances;

        try {
            model = new IloCplex(); // Initialize the CPLEX model
            System.out.println("CPLEX model initialized successfully.");
        } catch (IloException e) {
            System.err.println("Error initializing CPLEX model: " + e.getMessage());
        }
    }

    /**
     * Method to create decision variables for the TSP model.
     *
     * @param cplex The IloCplex instance
     * @param n     The number of nodes
     * @return A 2D array of decision variables
     * @throws IloException If an error occurs while creating variables
     */
    public static IloNumVar[][] createDecisionVariables(IloCplex cplex, int n) throws IloException {
        IloNumVar[][] x = new IloNumVar[n][n];
        for (int i = 0; i < n; i++) {
            x[i] = cplex.boolVarArray(n); // Create binary decision variables
        }
        return x;
    }

    /**
     * Initialize decision variables for this instance.
     */
    public void initializeDecisionVariables() {
        try {
            this.decisionVariables = createDecisionVariables(model, n);
            System.out.println("Decision variables initialized successfully.");
        } catch (IloException e) {
            System.err.println("Error creating decision variables: " + e.getMessage());
        }
    }

    /**
     * Set the capacity for the TSP problem.
     *
     * @param capacity The vehicle capacity
     */
    public void setCapacity(double capacity) {
        this.capacity = capacity;
    }

    /**
     * Display the CPLEX model and problem details.
     */
    public void displayModelDetails() {
        System.out.println("TSP Problem Details:");
        System.out.println("Number of nodes: " + n);
        System.out.println("Distance matrix: " + (distances != null ? "Loaded" : "Not loaded"));
        System.out.println("Vehicle capacity: " + (capacity > 0 ? capacity : "Not set"));
        System.out.println("CPLEX model: " + (model != null ? "Initialized" : "Not initialized"));
    }

    // Getters
    public int getNodeCount() {
        return n;
    }

    public double[][] getDistances() {
        return distances;
    }

    public double getCapacity() {
        return capacity;
    }

    public IloCplex getModel() {
        return model;
    }

    public IloNumVar[][] getDecisionVariables() {
        return decisionVariables;
    }

    // Clean up CPLEX resources when done
    public void cleanUp() {
        if (model != null) {
            model.end();
            System.out.println("CPLEX resources released.");
        }
    }
}
