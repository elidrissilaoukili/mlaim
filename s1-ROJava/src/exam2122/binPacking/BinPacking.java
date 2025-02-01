package exam2122.binPacking;

import ilog.concert.*;
import ilog.cplex.*;

public class BinPacking {

    // Problem data
    private short[] A; // Sizes of items
    private int W;     // Bin capacity
    private int nbObjet; // Number of items
    private int nbBoite; // Maximum number of bins
    private IloCplex modele;
    private IloNumVar[][] y; // y[i][j] = 1 if item i is in bin j
    private IloNumVar[] z;   // z[j] = 1 if bin j is used

    // Constructor
    public BinPacking(short[] A, int W, int nbBoite) {
        this.A = A;
        this.W = W;
        this.nbObjet = A.length;
        this.nbBoite = nbBoite;

        try {
            modele = new IloCplex();
            createModele();
        } catch (IloException e) {
            e.printStackTrace();
        }
    }

    // Create constraints
    private void createModele() throws IloException {
        createVariables();
        createConstraints1();
        createConstraints2();
        createFonctionObj();
    }

    // Create variables
    private void createVariables() throws IloException {
        y = new IloNumVar[nbObjet][nbBoite];
        z = new IloNumVar[nbBoite];

        // y[i][j] = 1 if item i is in bin j
        for (int i = 0; i < nbObjet; i++) {
            for (int j = 0; j < nbBoite; j++) {
                y[i][j] = modele.boolVar("y_" + i + "_" + j);
            }
        }

        // z[j] = 1 if bin j is used
        for (int j = 0; j < nbBoite; j++) {
            z[j] = modele.boolVar("z_" + j);
        }
    }

    // Constraint 1: Each item must be in exactly one bin
    private void createConstraints1() throws IloException {
        for (int i = 0; i < nbObjet; i++) {
            IloLinearNumExpr c = modele.linearNumExpr();
            for (int j = 0; j < nbBoite; j++) {
                c.addTerm(1, y[i][j]);
            }
            modele.addEq(c, 1, "Constraint1_Item_" + i);
        }
    }

    // Constraint 2: Bin capacity constraint
    private void createConstraints2() throws IloException {
        for (int j = 0; j < nbBoite; j++) {
            IloLinearNumExpr c = modele.linearNumExpr();
            for (int i = 0; i < nbObjet; i++) {
                c.addTerm(A[i], y[i][j]);
            }
            c.addTerm(-W, z[j]); // Ensure the bin is used if it has items
            modele.addLe(c, 0, "Constraint2_Bin_" + j);
        }
    }

    // Objective function: Minimize the number of bins used
    private void createFonctionObj() throws IloException {
        IloLinearNumExpr objective = modele.linearNumExpr();
        for (int j = 0; j < nbBoite; j++) {
            objective.addTerm(1, z[j]);
        }
        modele.addMinimize(objective, "MinimizeBins");
    }

    // Solve the problem
    public boolean solve() {
        try {
            if (modele.solve()) {
                System.out.println("Optimal solution found!");
                return true;
            } else {
                System.out.println("No solution found.");
                return false;
            }
        } catch (IloException e) {
            e.printStackTrace();
            return false;
        }
    }

    // Get solution for y[i][j]
    public void getSolutionY() {
        try {
            for (int i = 0; i < nbObjet; i++) {
                for (int j = 0; j < nbBoite; j++) {
                    if (modele.getValue(y[i][j]) > 0.5) {
                        System.out.println("Item " + i + " is in Bin " + j);
                    }
                }
            }
        } catch (IloException e) {
            e.printStackTrace();
        }
    }

    // Get solution for z[j]
    public void getSolutionZ() {
        try {
            for (int j = 0; j < nbBoite; j++) {
                if (modele.getValue(z[j]) > 0.5) {
                    System.out.println("Bin " + j + " is used.");
                }
            }
        } catch (IloException e) {
            e.printStackTrace();
        }
    }

}
