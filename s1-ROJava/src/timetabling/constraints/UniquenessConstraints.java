package timetabling.constraints;

import ilog.concert.*;
import ilog.cplex.*;
import timetabling.TimetablingData;

import java.util.Arrays;

public class UniquenessConstraints {

    private IloCplex model; // CPLEX model
    private IloNumVar[][][][][][] x; // Decision variables

    TimetablingData data;

    public UniquenessConstraints(IloCplex model, IloNumVar[][][][][][] x) {
        this.model = model;
        this.data = new TimetablingData();
        data.initializeData();
    }

    public void createConstraints1() throws IloException {
        int[] I = data.getI();
        int[] J = data.getJ();
        int[] L = data.getL();

        // Uniqueness constraints
        for (int i = 0; i < I.length; i++) {
            for (int j = 0; j < J.length; j++) {
                for (int l : L) { // Iterate over the actual teacher IDs
                    IloLinearNumExpr expr = model.linearNumExpr();

                    int[] groupsForTeacher = data.getK_l(l);
                    System.out.println("Groups for teacher " + l + ": " + Arrays.toString(groupsForTeacher)); // Debugging
                    if (groupsForTeacher != null) {
                        for (int k : groupsForTeacher) {
                            int[] coursesForGroupAndTeacher = data.getM_kl(k, l);
                            System.out.println("Courses for group " + k + " and teacher " + l + ": " + Arrays.toString(coursesForGroupAndTeacher)); // Debugging
                            if (coursesForGroupAndTeacher != null) {
                                for (int m : coursesForGroupAndTeacher) {
                                    int[] classroomsForCourseAndGroup = data.getN_mk(m, k);
                                    System.out.println("Classrooms for course " + m + " and group " + k + ": " + Arrays.toString(classroomsForCourseAndGroup)); // Debugging
                                    if (classroomsForCourseAndGroup != null) {
                                        for (int n : classroomsForCourseAndGroup) {
                                            expr.addTerm(1.0, x[i][j][k][l][m][n]);
                                        }
                                    }
                                }
                            }
                        }
                    }

                    model.addLe(expr, 1.0); // Sum <= 1
                }
            }
        }
    }
}

