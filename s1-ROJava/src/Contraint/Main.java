package Contraint;

public class Main {
    public static void main(String[] args) {
        // Example usage
        int n = 5;
        double[][] distances = {
                {0, 10, 20, 2, 5, 30, 40, 50, 25},
                {0, 10, 23, 21, 15, 30, 40, 60, 25}
        };

        TSP tsp = new TSP(n, distances);
        tsp.setCapacity(100); // Example capacity
        tsp.initializeDecisionVariables();
        tsp.displayModelDetails();

        // Clean up resources
        tsp.cleanUp();
    }
}
