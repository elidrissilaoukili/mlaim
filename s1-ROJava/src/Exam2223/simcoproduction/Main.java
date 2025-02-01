package Exam2223.simcoproduction;

public class Main {

    public static void main(String[] args) {
        try {
            SimcoProduction simco = new SimcoProduction();
            simco.optimizeProduction(); // Call the method to solve the optimization problem
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

}
