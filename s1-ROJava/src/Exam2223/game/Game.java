package Exam2223.game;

import ilog.concert.IloException;
import ilog.concert.IloLinearNumExpr;
import ilog.concert.IloNumVar;
import ilog.cplex.IloCplex;

public class Game {

    private short[][] A;
    private int nbSerie;
    private int nbNombre;
    private int bornSup;

    private IloCplex cplex;
    private IloNumVar[][] x;
    private IloNumVar[] y;

    public Game(short[][] A, int bornSup) {
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

    public void createModele() {
        this.createVariables();
        this.createConstraints1();
        this.createConstraints2();
        this.createObjectiveFunc();
        this.solve();
    }

    public void createVariables() {
        try {
            y = cplex.boolVarArray(nbSerie);
            x = new IloNumVar[nbSerie][nbNombre];
            for (int i = 0; i < nbSerie; i++) {
                x[i] = cplex.boolVarArray(nbNombre);
            }
        } catch (IloException e) {
            e.printStackTrace();
        }
    }

    public void createConstraints1() {
        try {
            for (int i = 0; i < nbSerie; i++) {
                IloLinearNumExpr sumConstraint = cplex.linearNumExpr();
                for (int j = 0; j < nbNombre; j++) {
                    sumConstraint.addTerm(A[i][j], x[i][j]);
                }
                // Link x[i][j] to y[i]: If y[i] is 0, x[i][j] must be 0.
                cplex.addLe(sumConstraint, cplex.prod(bornSup, y[i]));
            }
        } catch (IloException e) {
            e.printStackTrace();
        }
    }

    public void createConstraints2() {
        try {
            IloLinearNumExpr seriesConstraint = cplex.linearNumExpr();
            for (int i = 0; i < nbSerie; i++) {
                seriesConstraint.addTerm(1, y[i]);
            }
            cplex.addEq(seriesConstraint, 1);
        } catch (IloException e) {
            e.printStackTrace();
        }
    }

    public void createObjectiveFunc() {
        try {
            IloLinearNumExpr sumObjective = cplex.linearNumExpr();
            for (int i = 0; i < nbSerie; i++) {
                for (int j = 0; j < nbNombre; j++) {
                    sumObjective.addTerm(A[i][j], x[i][j]);
                }
            }
            cplex.addMaximize(sumObjective);
        } catch (IloException e) {
            e.printStackTrace();
        }
    }

    public void solve() {
        try {
            if (cplex.solve()) {
                System.out.println("Optimal solution found:");
                getSolutionY(); // Print selected series
                getSolutionX(); // Print selected numbers for each series
                System.out.println("Total sum (obj): " + cplex.getObjValue());
            } else {
                System.out.println("No feasible solution found.");
            }
        } catch (IloException e) {
            throw new RuntimeException("Error while solving the model: " + e.getMessage(), e);
        }
    }

    public void getSolutionX() throws IloException {
        for (int i = 0; i < nbSerie; i++) {
            boolean hasNumbers = false;
            System.out.print("Selected Numbers are: ");

            double computedSum = 0;
            for (int j = 0; j < nbNombre; j++) {
                if (cplex.getValue(x[i][j]) > 0.5) {
                    System.out.print(A[i][j] + " ");
                    computedSum += A[i][j];
                    hasNumbers = true;
                }
            }

            if (hasNumbers) {
                System.out.println("\nComputed Sum: " + computedSum);
            } else {
                System.out.println("None");
            }
        }
    }

    public void getSolutionY() throws IloException {
        for (int i = 0; i < nbSerie; i++) {
            if (cplex.getValue(y[i]) > 0.5) {
                System.out.println("Selected Series are: S" + (i + 1));
            }
        }
    }

}
