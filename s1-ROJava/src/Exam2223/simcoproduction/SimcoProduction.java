package Exam2223.simcoproduction;

import ilog.concert.*;
import ilog.cplex.*;

public class SimcoProduction {

    public void optimizeProduction() {
        try {
            // Step 1: Create the CPLEX environment and model
            IloCplex cplex = new IloCplex();

            // Step 2: Define decision variables
            IloNumVar x1 = cplex.numVar(0, Double.MAX_VALUE, "QL-100"); // x1 >= 0
            IloNumVar x2 = cplex.numVar(0, Double.MAX_VALUE, "QL-200"); // x2 >= 0

            // Step 3: Define the objective function
            // Maximize Z = 100*x1 + 120*x2
            IloLinearNumExpr objective = cplex.linearNumExpr();
            objective.addTerm(100, x1);
            objective.addTerm(120, x2);
            cplex.addMaximize(objective);

            // Step 4: Define the constraints
            // Constraint 1: 3*x1 + 4*x2 <= 4200 (Assembly time)
            IloLinearNumExpr assemblyConstraint = cplex.linearNumExpr();
            assemblyConstraint.addTerm(3, x1);
            assemblyConstraint.addTerm(4, x2);
            cplex.addLe(assemblyConstraint, 4200, "Assembly");

            // Constraint 2: x1 + 3*x2 <= 2400 (Verification time)
            IloLinearNumExpr verificationConstraint = cplex.linearNumExpr();
            verificationConstraint.addTerm(1, x1);
            verificationConstraint.addTerm(3, x2);
            cplex.addLe(verificationConstraint, 2400, "Verification");

            // Constraint 3: 2*x1 + 2*x2 <= 2600 (Packaging time)
            IloLinearNumExpr packagingConstraint = cplex.linearNumExpr();
            packagingConstraint.addTerm(2, x1);
            packagingConstraint.addTerm(2, x2);
            cplex.addLe(packagingConstraint, 2600, "Packaging");

            // Step 5: Solve the model
            if (cplex.solve()) {
                System.out.println("\n\nhere is the output: ");
                System.out.println("Optimal Solution Found:");
                System.out.println("Maximum Profit: " + cplex.getObjValue());
                System.out.println("QL-100 to produce (x1): " + cplex.getValue(x1));
                System.out.println("QL-200 to produce (x2): " + cplex.getValue(x2));
            } else {
                System.out.println("No solution found.");
            }

            // Clean up
            cplex.end();

        } catch (IloException e) {
            e.printStackTrace();
        }
    }
}

