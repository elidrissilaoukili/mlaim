package Exam2223.game;

import ilog.concert.*;
import ilog.cplex.*;

public class Jeux {
    private short[][] A; // The input matrix (series of numbers)
    // Number of series
    private int nbSerie;
    private int nbNombre; // Number of numbers in each series
    private int bornSup; // Upper bound for the sum
    private IloCplex cplex; // CPLEX optimizer
    private IloNumVar[] y; // Series selection variables
    private IloNumVar[][] x; // Number selection variables

    // Constructor
    public Jeux(short[][] A, int bornSup) {
        this.A = A;
        this.nbSerie = A.length;
        this.nbNombre = A[0].length;
        this.bornSup = bornSup;
        try {
            cplex = new IloCplex();
        } catch (IloException e) {
            e.printStackTrace();
        }
    }

    // Create Variables
    public void createVariables() {
        try {
            y = cplex.boolVarArray(nbSerie); // y[i] for selecting series i
            x = new IloNumVar[nbSerie][];
            for (int i = 0; i < nbSerie; i++) {
                x[i] = cplex.boolVarArray(nbNombre); // x[i][j] for selecting number j in series i
            }
        } catch (IloException e) {
            e.printStackTrace();
        }
    }

    // Create Objective Function
    public void createFonctionObj() {
        try {
            IloLinearNumExpr obj = cplex.linearNumExpr();
            for (int i = 0; i < nbSerie; i++) {
                for (int j = 0; j < nbNombre; j++) {
                    obj.addTerm(A[i][j], x[i][j]);
                }
            }
            cplex.addMaximize(obj); // Maximize the sum of selected numbers
        } catch (IloException e) {
            e.printStackTrace();
        }
    }

    // Create Constraint 1: Only one series can be selected
    public void createConstraints1() {
        try {
            IloLinearNumExpr seriesConstraint = cplex.linearNumExpr();
            for (int i = 0; i < nbSerie; i++) {
                seriesConstraint.addTerm(1, y[i]);
            }
            cplex.addEq(seriesConstraint, 1); // Ensure only one series is chosen
        } catch (IloException e) {
            e.printStackTrace();
        }
    }

    // Create Constraint 2: Numbers can only be selected from the chosen series
    public void createConstraints2() {
        try {
            for (int i = 0; i < nbSerie; i++) {
                for (int j = 0; j < nbNombre; j++) {
                    cplex.addLe(x[i][j], y[i]); // x[i][j] <= y[i]
                }
            }
        } catch (IloException e) {
            e.printStackTrace();
        }
    }

    // Create Constraint 3: Total sum must not exceed the upper bound
    public void createConstraints3() {
        try {
            IloLinearNumExpr sumConstraint = cplex.linearNumExpr();
            for (int i = 0; i < nbSerie; i++) {
                for (int j = 0; j < nbNombre; j++) {
                    sumConstraint.addTerm(A[i][j], x[i][j]);
                }
            }
            cplex.addLe(sumConstraint, bornSup); // Sum <= 100
        } catch (IloException e) {
            e.printStackTrace();
        }
    }

    // Solve the problem
    public void solve() {
        try {
            if (cplex.solve()) {
                System.out.println("Optimal solution found:");
                for (int i = 0; i < nbSerie; i++) {
                    if (cplex.getValue(y[i]) > 0.5) {
                        System.out.println("Selected Series: S" + (i + 1));
                        System.out.print("Selected Numbers: ");
                        for (int j = 0; j < nbNombre; j++) {
                            if (cplex.getValue(x[i][j]) > 0.5) {
                                System.out.print(A[i][j] + " ");
                            }
                        }
                        System.out.println();
                    }
                }
                System.out.println("Total Sum: " + cplex.getObjValue());
            } else {
                System.out.println("No feasible solution found.");
            }
        } catch (IloException e) {
            e.printStackTrace();
        }
    }
}