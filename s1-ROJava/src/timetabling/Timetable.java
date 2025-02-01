package timetabling;

import ilog.concert.*;
import ilog.cplex.*;

import java.util.*;

public class Timetable {

    private IloCplex model; // CPLEX model
    private IloNumVar[][][][][][] x; // Decision variables
    private IloNumVar[][][][][][] y;

    private TimetablingData data; // Data for the problem

    public Timetable(TimetablingData data) {
        this.data = data;
        try {
            model = new IloCplex();
        } catch (IloException e) {
            throw new RuntimeException(e);
        }
    }

    public void createModel() {
        try {
            defineDecisionVariables();
            defineAuxiliaryVariables();
            createObjectiveFunction();

            // uniqueness constraints
            createConstraint1();
            createConstraint2();
            createConstraint3();
            createConstraint4();

            // Completeness constraints
            createConstraint5();
            createConstraint6();
            createConstraint7();
            createConstraint8();

            // Consecutiveness constraints
            createConstraint9();
            createConstraint10();
            createConstraint11();

            // Repetitiveness constraints
            createConstraint12();
            createConstraint13();
            createConstraint14();

            // Pre-assignment constraints
            createConstraint15();
        } catch (IloException e) {
            e.printStackTrace();
        }
    }

    private void defineDecisionVariables() throws IloException {
        int[] I = data.getI();
        int[] J = data.getJ();
        int[] K = data.getK();
        int[] L = data.getL();
        int[] M = data.getM();
        int[] N = data.getN();

        x = new IloNumVar[I.length][J.length][K.length][L.length][M.length][N.length];

        for (int i = 0; i < I.length; i++) {
            for (int j = 0; j < J.length; j++) {
                for (int k = 0; k < K.length; k++) {
                    for (int l = 0; l < L.length; l++) {
                        for (int m = 0; m < M.length; m++) {
                            for (int n = 0; n < N.length; n++) {
                                x[i][j][k][l][m][n] = model.boolVar();
                            }
                        }
                    }
                }
            }
        }
    }

    private void defineAuxiliaryVariables() throws IloException {
        int[] I = data.getI(); // Days
        int[] K = data.getK(); // Groups
        int[] M = data.getM(); // Courses
        int[] N = data.getN(); // Classrooms

        // Initialize y with dimensions [I.length][P_max][K.length][H_max][M.length][N.length]
        int P_max = 3; // Maximum number of repetitions for a session (adjust as needed)
        int H_max = data.getMaxSessionLengths(); // Maximum number of session lengths for any course

        y = new IloNumVar[I.length][P_max][K.length][H_max][M.length][N.length];

        for (int i = 0; i < I.length; i++) {
            for (int pv = 0; pv < P_max; pv++) {
                for (int k = 0; k < K.length; k++) {
                    for (int hv = 0; hv < H_max; hv++) {
                        for (int m = 0; m < M.length; m++) {
                            for (int n = 0; n < N.length; n++) {
                                y[i][pv][k][hv][m][n] = model.boolVar();
                            }
                        }
                    }
                }
            }
        }
    }

    private void createObjectiveFunction() throws IloException {
        IloLinearNumExpr objective = model.linearNumExpr();

        int[] I = data.getI();
        int[] J = data.getJ();
        int[] K = data.getK();
        int[] L = data.getL();
        int[] M = data.getM();
        int[] N = data.getN();

        // First term of the objective function: Cost of assigning course m to the j-th period of day i
        for (int i = 0; i < I.length; i++) {
            for (int j = 0; j < J.length; j++) {
                for (int k = 0; k < K.length; k++) {
                    for (int l = 0; l < L.length; l++) {
                        for (int m = 0; m < M.length; m++) {
                            for (int n = 0; n < N.length; n++) {
                                double cost = 1.0; // Assign a non-zero cost to incentivize assignments
                                objective.addTerm(cost, x[i][j][k][l][m][n]);
                            }
                        }
                    }
                }
            }
        }

        // Second term of the objective function: Cost incurred from the assignment of courses requiring multi-period sessions
        int P_max = 3; // Maximum number of repetitions for a session (adjust as needed)
        int H_max = data.getMaxSessionLengths(); // Maximum number of session lengths for any course

        for (int i = 0; i < I.length; i++) {
            for (int pv = 0; pv < P_max; pv++) {
                for (int k = 0; k < K.length; k++) {
                    for (int hv = 0; hv < H_max; hv++) {
                        for (int m = 0; m < M.length; m++) {
                            for (int n = 0; n < N.length; n++) {
                                double cost = 1.0; // Assign a non-zero cost to incentivize assignments
                                objective.addTerm(cost, y[i][pv][k][hv][m][n]);
                            }
                        }
                    }
                }
            }
        }

        // Add the objective function to the model
        model.addMinimize(objective);
    }

    private void createConstraint1() throws IloException {
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

    private void createConstraint2() throws IloException {
        int[] I = data.getI();
        int[] J = data.getJ();
        int[] K1 = data.getK1(); // Lower-grade years

        // Constraint 2: For every group in K1, at most one course, one teacher, and one classroom per period
        for (int k : K1) { // Iterate over lower-grade groups
            for (int i = 0; i < I.length; i++) {
                for (int j = 0; j < J.length; j++) {
                    IloLinearNumExpr expr = model.linearNumExpr();

                    // Get teachers available for group k on day i
                    int[] teachersForGroupAndDay = data.getL_ki(k, i);
                    System.out.println("Teachers for group " + k + " and day " + i + ": " + Arrays.toString(teachersForGroupAndDay)); // Debugging
                    if (teachersForGroupAndDay != null) {
                        for (int l : teachersForGroupAndDay) {
                            // Get courses taught by teacher l for group k
                            int[] coursesForGroupAndTeacher = data.getM_kl(k, l);
                            System.out.println("Courses for group " + k + " and teacher " + l + ": " + Arrays.toString(coursesForGroupAndTeacher)); // Debugging
                            if (coursesForGroupAndTeacher != null) {
                                for (int m : coursesForGroupAndTeacher) {
                                    // Get classrooms that fit group k for course m
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

                    // Add the constraint: Sum <= 1
                    model.addLe(expr, 1.0);
                }
            }
        }
    }

    private void createConstraint3() throws IloException {
        int[] I = data.getI();
        int[] J = data.getJ();
        int[] K2 = data.getK2(); // Higher-grade years

        // Constraint 3: For every group in K2, at most one course, one teacher, and one classroom per period
        for (int k : K2) { // Iterate over higher-grade groups
            for (int i = 0; i < I.length; i++) {
                for (int j = 0; j < J.length; j++) {
                    IloLinearNumExpr expr = model.linearNumExpr();

                    // First term: Courses designed for the group
                    int[] teachersForGroupAndDay = data.getL_ki(k, i);
                    System.out.println("Teachers for group " + k + " and day " + i + ": " + Arrays.toString(teachersForGroupAndDay)); // Debugging
                    if (teachersForGroupAndDay != null) {
                        for (int l : teachersForGroupAndDay) {
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

                    // Second term: Elective courses offered by other divisions
                    int[][] electiveCourses = data.getM_k_com();
                    for (int[] elective : electiveCourses) {
                        int ka = elective[0]; // Group offering the elective
                        int l = elective[1];  // Teacher
                        int m = elective[2];  // Course

                        int[] classroomsForElective = data.getN_mk(m, ka);
                        if (classroomsForElective != null) {
                            for (int n : classroomsForElective) {
                                expr.addTerm(1.0, x[i][j][ka][l][m][n]);
                            }
                        }
                    }

                    // Add the constraint: Sum <= 1
                    model.addLe(expr, 1.0);
                }
            }
        }
    }

    private void createConstraint4() throws IloException {
        int[] I = data.getI(); // Days
        int[] J = data.getJ(); // Periods
        int[] K = data.getK(); // Groups of students
        int[] N = data.getN(); // Classrooms

        // Constraint 4: For every group, day, and period, the sum of x_{i,j,k,l,m,n} <= 1
        for (int n = 0; n < N.length; n++) { // Iterate over classrooms
            for (int i = 0; i < I.length; i++) { // Iterate over days
                for (int j = 0; j < J.length; j++) { // Iterate over periods
                    IloLinearNumExpr expr = model.linearNumExpr();

                    // Iterate over all groups (K) instead of using I_n
                    for (int k = 0; k < K.length; k++) {
                        // Get teachers available for group k on day i
                        int[] teachersForGroupAndDay = data.getL_ki(k, i);
                        if (teachersForGroupAndDay != null) {
                            for (int l = 0; l < teachersForGroupAndDay.length; l++) {
                                // Get courses taught by teacher l for group k
                                int[] coursesForGroupAndTeacher = data.getM_kl(k, teachersForGroupAndDay[l]);
                                if (coursesForGroupAndTeacher != null) {
                                    for (int m = 0; m < coursesForGroupAndTeacher.length; m++) {
                                        // Add the variable x_{i,j,k,l,m,n} to the expression
                                        expr.addTerm(1.0, x[i][j][k][teachersForGroupAndDay[l]][coursesForGroupAndTeacher[m]][n]);
                                    }
                                }
                            }
                        }
                    }

                    // Add the constraint: Sum <= 1
                    model.addLe(expr, 1.0);
                }
            }
        }
    }

    private void createConstraint5() throws IloException {
        int[] K1 = data.getK1(); // Lower-grade years

        // Constraint 5: All courses for lower-grade groups must be scheduled with the correct number of teaching periods
        for (int k : K1) { // Iterate over lower-grade groups
            IloLinearNumExpr expr = model.linearNumExpr();

            // Get teachers for group k
            int[] teachersForGroup = data.getL_k(k);
            if (teachersForGroup == null || teachersForGroup.length == 0) {
                System.out.println("No teachers found for group " + k);
                continue; // Skip this group if no teachers are available
            }
            System.out.println("Teachers for group " + k + ": " + Arrays.toString(teachersForGroup));

            for (int l : teachersForGroup) {
                // Get courses taught by teacher l for group k
                int[] coursesForGroupAndTeacher = data.getM_kl(k, l);
                if (coursesForGroupAndTeacher == null || coursesForGroupAndTeacher.length == 0) {
                    System.out.println("No courses found for group " + k + " and teacher " + l);
                    continue; // Skip this teacher if no courses are available
                }
                System.out.println("Courses for group " + k + " and teacher " + l + ": " + Arrays.toString(coursesForGroupAndTeacher));

                for (int m : coursesForGroupAndTeacher) {
                    // Get classrooms that fit group k for course m
                    int[] classroomsForCourseAndGroup = data.getN_mk(m, k);
                    if (classroomsForCourseAndGroup == null || classroomsForCourseAndGroup.length == 0) {
                        System.out.println("No classrooms found for course " + m + " and group " + k);
                        continue; // Skip this course if no classrooms are available
                    }
                    System.out.println("Classrooms for course " + m + " and group " + k + ": " + Arrays.toString(classroomsForCourseAndGroup));

                    for (int n : classroomsForCourseAndGroup) {
                        // Get days and periods for teacher l and classroom n
                        int[] daysForTeacherAndClassroom = data.getI_ln(l, n);
                        if (daysForTeacherAndClassroom == null || daysForTeacherAndClassroom.length == 0) {
                            System.out.println("No days found for teacher " + l + " and classroom " + n);
                            continue; // Skip this classroom if no days are available
                        }
                        System.out.println("Days for teacher " + l + " and classroom " + n + ": " + Arrays.toString(daysForTeacherAndClassroom));

                        for (int i : daysForTeacherAndClassroom) {
                            int[] periodsForDayAndTeacherAndClassroom = data.getJ_iln(i, l, n);
                            if (periodsForDayAndTeacherAndClassroom == null || periodsForDayAndTeacherAndClassroom.length == 0) {
                                System.out.println("No periods found for day " + i + ", teacher " + l + ", and classroom " + n);
                                continue; // Skip this day if no periods are available
                            }
                            System.out.println("Periods for day " + i + ", teacher " + l + ", and classroom " + n + ": " + Arrays.toString(periodsForDayAndTeacherAndClassroom));

                            for (int j : periodsForDayAndTeacherAndClassroom) {
                                // Debugging: Print indices and array sizes
                                System.out.println("Accessing x[" + i + "][" + j + "][" + k + "][" + l + "][" + m + "][" + n + "]");
                                expr.addTerm(1.0, x[i][j][k][l][m][n]);
                            }
                        }
                    }
                }
            }

            // Add the constraint: Sum of assignments = a_k
            int a_k = data.getA_k(k); // Total teaching periods for group k
            model.addEq(expr, a_k);
        }
    }

    private void createConstraint6() throws IloException {
        int[] K2 = data.getK2(); // Higher-grade years

        // Constraint 6: All courses for higher-grade groups must be scheduled with the correct number of teaching periods
        for (int k : K2) { // Iterate over higher-grade groups
            IloLinearNumExpr expr = model.linearNumExpr();

            // First term: Courses designed for the group
            int[] teachersForGroup = data.getL_k(k);
            if (teachersForGroup == null || teachersForGroup.length == 0) {
                System.out.println("No teachers found for group " + k);
                continue; // Skip this group if no teachers are available
            }

            for (int l : teachersForGroup) {
                int[] coursesForGroupAndTeacher = data.getM_kl(k, l);
                if (coursesForGroupAndTeacher == null || coursesForGroupAndTeacher.length == 0) {
                    System.out.println("No courses found for group " + k + " and teacher " + l);
                    continue; // Skip this teacher if no courses are available
                }

                for (int m : coursesForGroupAndTeacher) {
                    int[] classroomsForCourseAndGroup = data.getN_mk(m, k);
                    if (classroomsForCourseAndGroup == null || classroomsForCourseAndGroup.length == 0) {
                        System.out.println("No classrooms found for course " + m + " and group " + k);
                        continue; // Skip this course if no classrooms are available
                    }

                    for (int n : classroomsForCourseAndGroup) {
                        int[] daysForTeacherAndClassroom = data.getI_ln(l, n);
                        if (daysForTeacherAndClassroom == null || daysForTeacherAndClassroom.length == 0) {
                            System.out.println("No days found for teacher " + l + " and classroom " + n);
                            continue; // Skip this classroom if no days are available
                        }

                        for (int i : daysForTeacherAndClassroom) {
                            // Check if i is within bounds for x
                            if (i < 0 || i >= x.length) {
                                System.out.println("Invalid day index: " + i);
                                continue;
                            }

                            int[] periodsForDayAndTeacherAndClassroom = data.getJ_iln(i, l, n);
                            if (periodsForDayAndTeacherAndClassroom == null || periodsForDayAndTeacherAndClassroom.length == 0) {
                                System.out.println("No periods found for day " + i + ", teacher " + l + ", and classroom " + n);
                                continue; // Skip this day if no periods are available
                            }

                            for (int j : periodsForDayAndTeacherAndClassroom) {
                                // Check if j is within bounds for x[i]
                                if (j < 0 || j >= x[i].length) {
                                    System.out.println("Invalid period index: " + j);
                                    continue;
                                }

                                // Ensure all indices are within bounds
                                if (k >= 0 && k < x[i][j].length &&
                                        l >= 0 && l < x[i][j][k].length &&
                                        m >= 0 && m < x[i][j][k][l].length &&
                                        n >= 0 && n < x[i][j][k][l][m].length) {
                                    expr.addTerm(1.0, x[i][j][k][l][m][n]);
                                } else {
                                    System.out.println("Invalid indices: x[" + i + "][" + j + "][" + k + "][" + l + "][" + m + "][" + n + "]");
                                }
                            }
                        }
                    }
                }
            }

            // Second term: Elective courses offered by other divisions
            int[][] electiveCourses = data.getM_k_com();
            if (electiveCourses != null) {
                for (int[] elective : electiveCourses) {
                    int ka = elective[0]; // Group offering the elective
                    int l = elective[1];  // Teacher
                    int m = elective[2];  // Course

                    int[] classroomsForElective = data.getN_mk(m, ka);
                    if (classroomsForElective == null || classroomsForElective.length == 0) {
                        System.out.println("No classrooms found for elective course " + m + " and group " + ka);
                        continue; // Skip this elective if no classrooms are available
                    }

                    for (int n : classroomsForElective) {
                        int[] daysForTeacherAndClassroom = data.getI_ln(l, n);
                        if (daysForTeacherAndClassroom == null || daysForTeacherAndClassroom.length == 0) {
                            System.out.println("No days found for teacher " + l + " and classroom " + n);
                            continue; // Skip this classroom if no days are available
                        }

                        for (int i : daysForTeacherAndClassroom) {
                            int[] periodsForDayAndTeacherAndClassroom = data.getJ_iln(i, l, n);
                            if (periodsForDayAndTeacherAndClassroom == null || periodsForDayAndTeacherAndClassroom.length == 0) {
                                System.out.println("No periods found for day " + i + ", teacher " + l + ", and classroom " + n);
                                continue; // Skip this day if no periods are available
                            }

                            for (int j : periodsForDayAndTeacherAndClassroom) {
                                // Ensure all indices are within bounds
                                if (ka >= 0 && ka < x[i][j].length &&
                                        l >= 0 && l < x[i][j][ka].length &&
                                        m >= 0 && m < x[i][j][ka][l].length &&
                                        n >= 0 && n < x[i][j][ka][l][m].length) {
                                    expr.addTerm(1.0, x[i][j][ka][l][m][n]);
                                } else {
                                    System.out.println("Invalid indices: x[" + i + "][" + j + "][" + ka + "][" + l + "][" + m + "][" + n + "]");
                                }
                            }
                        }
                    }
                }
            }

            // Add the constraint: Sum of assignments = a_k
            int a_k = data.getA_k(k); // Total teaching periods for group k
            model.addEq(expr, a_k);
        }
    }

    private void createConstraint7() throws IloException {
        int[] K = data.getK(); // Groups of students
        int[] L = data.getL(); // Teachers
        int[] M = data.getM(); // Courses

        for (int k : K) { // Iterate over groups
            int[] teachersForGroup = data.getL_k(k); // Teachers for group k
            if (teachersForGroup == null || teachersForGroup.length == 0) {
                System.out.println("No teachers found for group " + k);
                continue;
            }

            for (int l : teachersForGroup) { // Iterate over teachers for group k
                int[] coursesForGroupAndTeacher = data.getM_kl(k, l); // Courses for group k and teacher l
                if (coursesForGroupAndTeacher == null || coursesForGroupAndTeacher.length == 0) {
                    System.out.println("No courses found for group " + k + " and teacher " + l);
                    continue;
                }

                for (int m : coursesForGroupAndTeacher) { // Iterate over courses for group k and teacher l
                    IloLinearNumExpr expr = model.linearNumExpr();

                    int[] classroomsForCourseAndGroup = data.getN_mk(m, k); // Classrooms for course m and group k
                    if (classroomsForCourseAndGroup == null || classroomsForCourseAndGroup.length == 0) {
                        System.out.println("No classrooms found for course " + m + " and group " + k);
                        continue;
                    }

                    for (int n : classroomsForCourseAndGroup) { // Iterate over classrooms for course m and group k
                        int[] daysForTeacherAndClassroom = data.getI_ln(l, n); // Days for teacher l and classroom n
                        if (daysForTeacherAndClassroom == null || daysForTeacherAndClassroom.length == 0) {
                            System.out.println("No days found for teacher " + l + " and classroom " + n);
                            continue;
                        }

                        for (int i : daysForTeacherAndClassroom) { // Iterate over days for teacher l and classroom n
                            int[] periodsForDayAndTeacherAndClassroom = data.getJ_iln(i, l, n); // Periods for day i, teacher l, and classroom n
                            if (periodsForDayAndTeacherAndClassroom == null || periodsForDayAndTeacherAndClassroom.length == 0) {
                                System.out.println("No periods found for day " + i + ", teacher " + l + ", and classroom " + n);
                                continue;
                            }

                            for (int j : periodsForDayAndTeacherAndClassroom) { // Iterate over periods for day i, teacher l, and classroom n
                                expr.addTerm(1.0, x[i][j][k][l][m][n]);
                            }
                        }
                    }

                    // Add the constraint: Sum of assignments = b_m
                    int b_m = data.getB_m(m); // Total teaching periods required for course m
                    model.addEq(expr, b_m);
                }
            }
        }
    }

    private void createConstraint8() throws IloException {
        int[] L = data.getL(); // Teachers

        for (int l : L) { // Iterate over teachers
            IloLinearNumExpr expr = model.linearNumExpr();

            int[] groupsForTeacher = data.getK_l(l); // Groups for teacher l
            if (groupsForTeacher == null || groupsForTeacher.length == 0) {
                System.out.println("No groups found for teacher " + l);
                continue;
            }

            for (int k : groupsForTeacher) { // Iterate over groups for teacher l
                int[] coursesForGroupAndTeacher = data.getM_kl(k, l); // Courses for group k and teacher l
                if (coursesForGroupAndTeacher == null || coursesForGroupAndTeacher.length == 0) {
                    System.out.println("No courses found for group " + k + " and teacher " + l);
                    continue;
                }

                for (int m : coursesForGroupAndTeacher) { // Iterate over courses for group k and teacher l
                    int[] classroomsForCourseAndGroup = data.getN_mk(m, k); // Classrooms for course m and group k
                    if (classroomsForCourseAndGroup == null || classroomsForCourseAndGroup.length == 0) {
                        System.out.println("No classrooms found for course " + m + " and group " + k);
                        continue;
                    }

                    for (int n : classroomsForCourseAndGroup) { // Iterate over classrooms for course m and group k
                        int[] daysForTeacherAndClassroom = data.getI_ln(l, n); // Days for teacher l and classroom n
                        if (daysForTeacherAndClassroom == null || daysForTeacherAndClassroom.length == 0) {
                            System.out.println("No days found for teacher " + l + " and classroom " + n);
                            continue;
                        }

                        for (int i : daysForTeacherAndClassroom) { // Iterate over days for teacher l and classroom n
                            int[] periodsForDayAndTeacherAndClassroom = data.getJ_iln(i, l, n); // Periods for day i, teacher l, and classroom n
                            if (periodsForDayAndTeacherAndClassroom == null || periodsForDayAndTeacherAndClassroom.length == 0) {
                                System.out.println("No periods found for day " + i + ", teacher " + l + ", and classroom " + n);
                                continue;
                            }

                            for (int j : periodsForDayAndTeacherAndClassroom) { // Iterate over periods for day i, teacher l, and classroom n
                                expr.addTerm(1.0, x[i][j][k][l][m][n]);
                            }
                        }
                    }
                }
            }

            // Add the constraint: Sum of assignments = s_l
            int s_l = data.getS_l(l); // Total teaching periods required for teacher l
            model.addEq(expr, s_l);
        }
    }

    private void createConstraint9() throws IloException {
        int[] K = data.getK(); // Groups of students
        int[] M = data.getM(); // Courses
        int[] N = data.getN(); // Classrooms
        int[] I = data.getI(); // Days

        // Debug: Print dimensions of y
        System.out.println("y dimensions: " + y.length + ", " + y[0].length + ", " + y[0][0].length + ", " + y[0][0][0].length + ", " + y[0][0][0][0].length + ", " + y[0][0][0][0][0].length);

        for (int k : K) { // Iterate over groups
            int[] coursesForGroup = data.getM_k(k); // Courses for group k
            if (coursesForGroup == null || coursesForGroup.length == 0) {
                System.out.println("No courses found for group " + k);
                continue;
            }

            for (int m : coursesForGroup) { // Iterate over courses for group k
                int[] sessionLengths = data.getH_m(m); // Possible session lengths for course m
                if (sessionLengths == null || sessionLengths.length == 0) {
                    System.out.println("No session lengths found for course " + m);
                    continue;
                }

                for (int hv : sessionLengths) { // Iterate over session lengths
                    int[] classroomsForCourseAndGroup = data.getN_mk(m, k); // Classrooms for course m and group k
                    if (classroomsForCourseAndGroup == null || classroomsForCourseAndGroup.length == 0) {
                        System.out.println("No classrooms found for course " + m + " and group " + k);
                        continue;
                    }

                    for (int n : classroomsForCourseAndGroup) { // Iterate over classrooms
                        for (int i : I) { // Iterate over days
                            for (int pv = 0; pv < y[i].length; pv++) { // Iterate over repetitions
                                // Check array bounds before accessing
                                if (i >= y.length || pv >= y[i].length || k >= y[i][pv].length || hv >= y[i][pv][k].length || m >= y[i][pv][k][hv].length || n >= y[i][pv][k][hv][m].length) {
                                    System.out.println("Invalid indices: " + i + ", " + pv + ", " + k + ", " + hv + ", " + m + ", " + n);
                                    continue;
                                }

                                // Access y[i][pv][k][hv][m][n]
                                System.out.println("Accessing y[" + i + "][" + pv + "][" + k + "][" + hv + "][" + m + "][" + n + "]");
                                IloLinearNumExpr expr = model.linearNumExpr();
                                expr.addTerm(1.0, y[i][pv][k][hv][m][n]);

                                // Add the constraint: Sum of y_{i,pv,k,hv,m,n} <= 1
                                model.addLe(expr, 1.0);
                            }
                        }
                    }
                }
            }
        }
    }

    private void createConstraint10() throws IloException {
        int[] I = data.getI(); // Days
        int[] K = data.getK(); // Groups
        int[] L = data.getL(); // Teachers
        int[] M = data.getM(); // Courses
        int[] N = data.getN(); // Classrooms

        for (int i : I) { // Iterate over days
            for (int k : K) { // Iterate over groups
                int[] teachersForGroupAndDay = data.getL_ki(k, i); // Teachers for group k on day i
                if (teachersForGroupAndDay == null || teachersForGroupAndDay.length == 0) {
                    System.out.println("No teachers found for group " + k + " on day " + i);
                    continue;
                }

                for (int l : teachersForGroupAndDay) { // Iterate over teachers for group k on day i
                    int[] coursesForGroupAndTeacher = data.getM_kl(k, l); // Courses for group k and teacher l
                    if (coursesForGroupAndTeacher == null || coursesForGroupAndTeacher.length == 0) {
                        System.out.println("No courses found for group " + k + " and teacher " + l);
                        continue;
                    }

                    for (int m : coursesForGroupAndTeacher) { // Iterate over courses for group k and teacher l
                        int[] classroomsForCourseAndGroup = data.getN_mk(m, k); // Classrooms for course m and group k
                        if (classroomsForCourseAndGroup == null || classroomsForCourseAndGroup.length == 0) {
                            System.out.println("No classrooms found for course " + m + " and group " + k);
                            continue;
                        }

                        for (int n : classroomsForCourseAndGroup) { // Iterate over classrooms for course m and group k
                            int[] periodsForDayAndTeacherAndClassroom = data.getJ_iln(i, l, n); // Periods for day i, teacher l, and classroom n
                            if (periodsForDayAndTeacherAndClassroom == null || periodsForDayAndTeacherAndClassroom.length == 0) {
                                System.out.println("No periods found for day " + i + ", teacher " + l + ", and classroom " + n);
                                continue;
                            }

                            int[] H_m = data.getH_m(m); // Possible session lengths for course m
                            for (int hv : H_m) { // Iterate over possible session lengths
                                if (hv > 1) { // Only apply for multi-period sessions
                                    for (int ja : periodsForDayAndTeacherAndClassroom) { // Iterate over starting periods
                                        if (ja + hv - 1 < periodsForDayAndTeacherAndClassroom.length) { // Ensure enough periods remain
                                            for (int t = 1; t < hv; t++) { // Iterate over consecutive periods
                                                // Constraint: x_{i,ja,k,l,m,n} - x_{i,ja+t,k,l,m,n} <= 0
                                                IloLinearNumExpr expr = model.linearNumExpr();
                                                expr.addTerm(1.0, x[i][ja][k][l][m][n]);
                                                expr.addTerm(-1.0, x[i][ja + t][k][l][m][n]);
                                                model.addLe(expr, 0.0);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    private void createConstraint11() throws IloException {
        int[] I = data.getI(); // Days
        int[] K = data.getK(); // Groups
        int[] L = data.getL(); // Teachers
        int[] M = data.getM(); // Courses
        int[] N = data.getN(); // Classrooms

        for (int i : I) { // Iterate over days
            for (int k : K) { // Iterate over groups
                int[] teachersForGroupAndDay = data.getL_ki(k, i); // Teachers for group k on day i
                if (teachersForGroupAndDay == null || teachersForGroupAndDay.length == 0) {
                    System.out.println("No teachers found for group " + k + " on day " + i);
                    continue;
                }

                for (int l : teachersForGroupAndDay) { // Iterate over teachers for group k on day i
                    int[] coursesForGroupAndTeacher = data.getM_kl(k, l); // Courses for group k and teacher l
                    if (coursesForGroupAndTeacher == null || coursesForGroupAndTeacher.length == 0) {
                        System.out.println("No courses found for group " + k + " and teacher " + l);
                        continue;
                    }

                    for (int m : coursesForGroupAndTeacher) { // Iterate over courses for group k and teacher l
                        int[] classroomsForCourseAndGroup = data.getN_mk(m, k); // Classrooms for course m and group k
                        if (classroomsForCourseAndGroup == null || classroomsForCourseAndGroup.length == 0) {
                            System.out.println("No classrooms found for course " + m + " and group " + k);
                            continue;
                        }

                        for (int n : classroomsForCourseAndGroup) { // Iterate over classrooms for course m and group k
                            int[] periodsForDayAndTeacherAndClassroom = data.getJ_iln(i, l, n); // Periods for day i, teacher l, and classroom n
                            if (periodsForDayAndTeacherAndClassroom == null || periodsForDayAndTeacherAndClassroom.length == 0) {
                                System.out.println("No periods found for day " + i + ", teacher " + l + ", and classroom " + n);
                                continue;
                            }

                            int[] H_m = data.getH_m(m); // Possible session lengths for course m
                            for (int hv : H_m) { // Iterate over possible session lengths
                                if (hv > 1) { // Only apply for multi-period sessions
                                    for (int j : periodsForDayAndTeacherAndClassroom) { // Iterate over periods
                                        if (j + hv - 1 < periodsForDayAndTeacherAndClassroom.length) { // Ensure enough periods remain
                                            for (int t = 2; t < hv; t++) { // Iterate over consecutive periods
                                                // Constraint: -x_{i,j,k,l,m,n} + x_{i,j+1,k,l,m,n} - x_{i,j+t,k,l,m,n} <= 0
                                                IloLinearNumExpr expr = model.linearNumExpr();
                                                expr.addTerm(-1.0, x[i][j][k][l][m][n]);
                                                expr.addTerm(1.0, x[i][j + 1][k][l][m][n]);
                                                expr.addTerm(-1.0, x[i][j + t][k][l][m][n]);
                                                model.addLe(expr, 0.0);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    private void createConstraint12() throws IloException {
        int[] K = data.getK(); // Groups of students
        int[] M = data.getM(); // Courses
        int[] N = data.getN(); // Classrooms
        int[] I = data.getI(); // Days

        for (int k : K) { // Iterate over groups
            int[] coursesForGroup = data.getM_k(k); // Courses for group k
            if (coursesForGroup == null || coursesForGroup.length == 0) {
                System.out.println("No courses found for group " + k);
                continue;
            }

            for (int m : coursesForGroup) { // Iterate over courses for group k
                if (data.getM_lab() != null && Arrays.stream(data.getM_lab()).anyMatch(lab -> lab == m)) {
                    continue; // Skip lab courses (handled by Constraint 14)
                }

                int[] sessionLengths = data.getH_m(m); // Possible session lengths for course m
                if (sessionLengths == null || sessionLengths.length == 0) {
                    System.out.println("No session lengths found for course " + m);
                    continue;
                }

                for (int hv : sessionLengths) { // Iterate over session lengths
                    int[] classroomsForCourseAndGroup = data.getN_mk(m, k); // Classrooms for course m and group k
                    if (classroomsForCourseAndGroup == null || classroomsForCourseAndGroup.length == 0) {
                        System.out.println("No classrooms found for course " + m + " and group " + k);
                        continue;
                    }

                    for (int n : classroomsForCourseAndGroup) { // Iterate over classrooms
                        IloLinearNumExpr expr = model.linearNumExpr();

                        // Fix for getL_m: Retrieve the array of teachers for course m
                        int[] teachersForCourse = data.getL_m().get(m);
                        if (teachersForCourse == null || teachersForCourse.length == 0) {
                            System.out.println("No teachers found for course " + m);
                            continue;
                        }

                        int[] daysForClassroom = data.getI_ln(teachersForCourse[0], n); // Days for teacher and classroom
                        if (daysForClassroom == null || daysForClassroom.length == 0) {
                            System.out.println("No days found for classroom " + n);
                            continue;
                        }

                        for (int i : daysForClassroom) { // Iterate over days
                            for (int pv = 0; pv < y[i].length; pv++) { // Iterate over repetitions
                                expr.addTerm(1.0, y[i][pv][k][hv - 1][m][n]); // hv - 1 because array indices start at 0
                            }
                        }

                        // Add the constraint: Sum of y_{i,pv,k,hv,m,n} = b_{m,hv}
                        int b_m_hv = data.getB_m(m); // Total number of h_v-period sessions required for course m
                        model.addEq(expr, b_m_hv);
                    }
                }
            }
        }
    }

    private void createConstraint13() throws IloException {
        int[] K = data.getK(); // Groups of students
        int[] M = data.getM(); // Courses
        int[] N = data.getN(); // Classrooms
        int[] I = data.getI(); // Days

        for (int k : K) { // Iterate over groups
            int[] coursesForGroup = data.getM_k(k); // Courses for group k
            if (coursesForGroup == null || coursesForGroup.length == 0) {
                System.out.println("No courses found for group " + k);
                continue;
            }

            for (int m : coursesForGroup) { // Iterate over courses for group k
                if (data.getM_lab() != null && Arrays.stream(data.getM_lab()).anyMatch(lab -> lab == m)) {
                    continue; // Skip lab courses (handled by Constraint 14)
                }

                int[] sessionLengths = data.getH_m(m); // Possible session lengths for course m
                if (sessionLengths == null || sessionLengths.length == 0) {
                    System.out.println("No session lengths found for course " + m);
                    continue;
                }

                int[] classroomsForCourseAndGroup = data.getN_mk(m, k); // Classrooms for course m and group k
                if (classroomsForCourseAndGroup == null || classroomsForCourseAndGroup.length == 0) {
                    System.out.println("No classrooms found for course " + m + " and group " + k);
                    continue;
                }

                for (int n : classroomsForCourseAndGroup) { // Iterate over classrooms
                    for (int i : I) { // Iterate over days
                        IloLinearNumExpr expr = model.linearNumExpr();

                        for (int hv : sessionLengths) { // Iterate over session lengths
                            for (int pv = 0; pv < y[i].length; pv++) { // Iterate over repetitions
                                expr.addTerm(1.0, y[i][pv][k][hv - 1][m][n]); // hv - 1 because array indices start at 0
                            }
                        }

                        // Add the constraint: Sum of y_{i,pv,k,hv,m,n} <= 1
                        model.addLe(expr, 1.0);
                    }
                }
            }
        }
    }

    private void createConstraint14() throws IloException {
        int[] K = data.getK(); // Groups of students
        int[] M_lab = data.getM_lab(); // Lab courses
        int[] N = data.getN(); // Classrooms
        int[] I = data.getI(); // Days

        if (M_lab == null || M_lab.length == 0) {
            System.out.println("No lab courses found.");
            return;
        }

        for (int k : K) { // Iterate over groups
            for (int m : M_lab) { // Iterate over lab courses
                int[] sessionLengths = data.getH_m(m); // Possible session lengths for course m
                if (sessionLengths == null || sessionLengths.length == 0) {
                    System.out.println("No session lengths found for lab course " + m);
                    continue;
                }

                for (int hv : sessionLengths) { // Iterate over session lengths
                    IloLinearNumExpr expr = model.linearNumExpr();

                    int[] classroomsForCourseAndGroup = data.getN_mk(m, k); // Classrooms for course m and group k
                    if (classroomsForCourseAndGroup == null || classroomsForCourseAndGroup.length == 0) {
                        System.out.println("No classrooms found for lab course " + m + " and group " + k);
                        continue;
                    }

                    for (int n : classroomsForCourseAndGroup) { // Iterate over classrooms
                        int[] teachersForCourse = data.getL_m().get(m); // Get the array of teachers for course m
                        if (teachersForCourse == null || teachersForCourse.length == 0) {
                            System.out.println("No teachers found for lab course " + m);
                            continue;
                        }

                        int[] daysForClassroom = data.getI_ln(teachersForCourse[0], n); // Days for teacher and classroom
                        if (daysForClassroom == null || daysForClassroom.length == 0) {
                            System.out.println("No days found for classroom " + n);
                            continue;
                        }

                /*        for (int i : daysForClassroom) { // Iterate over days
                            for (int pv = 0; pv < y[i].length; pv++) { // Iterate over repetitions
                                expr.addTerm(1.0, y[i][pv][k][hv - 1][m][n]); // hv - 1 because array indices start at 0
                            }
                        }*/
                        for (int i : daysForClassroom) { // Iterate over days
                            if (i < 0 || i >= y.length) { // Validate `i`
                                System.out.println("Invalid day index: " + i);
                                continue;
                            }

                            for (int pv = 0; pv < y[i].length; pv++) { // Iterate over repetitions
                                if (k < 0 || k >= y[i][pv].length) { // Validate `k`
                                    System.out.println("Invalid group index: " + k);
                                    continue;
                                }

                                if (hv - 1 < 0 || hv - 1 >= y[i][pv][k].length) { // Validate `hv - 1`
                                    System.out.println("Invalid session length index: " + (hv - 1));
                                    continue;
                                }

                                if (m < 0 || m >= y[i][pv][k][hv - 1].length) { // Validate `m`
                                    System.out.println("Invalid course index: " + m);
                                    continue;
                                }

                                if (n < 0 || n >= y[i][pv][k][hv - 1][m].length) { // Validate `n`
                                    System.out.println("Invalid classroom index: " + n);
                                    continue;
                                }

                                // If all indices are valid, proceed with adding the term
                                expr.addTerm(1.0, y[i][pv][k][hv - 1][m][n]);
                            }
                        }


                    }

                    // Fix for getP_m: Retrieve the array of repetitions for course m
                    int[] p_max_m_array = data.getP_m().get(m);
                    if (p_max_m_array == null || p_max_m_array.length == 0) {
                        System.out.println("No repetitions found for lab course " + m);
                        continue;
                    }
                    int p_max_m = p_max_m_array[0]; // Use the first value in the array

                    // Add the constraint: Sum of y_{i,pv,k,hv,m,n} = p_max_m
                    model.addEq(expr, p_max_m);
                }
            }
        }
    }

    private void createConstraint15() throws IloException {
        int[][] PRA = data.getPRA(); // Get the pre-assignment set

        if (PRA == null || PRA.length == 0) {
            System.out.println("No pre-assignments found.");
            return;
        }

        for (int[] assignment : PRA) {
            // Extract pre-assignment details
            int i = assignment[0]; // Day
            int j = assignment[1]; // Period
            int k = assignment[2]; // Student group
            int l = assignment[3]; // Teacher
            int m = assignment[4]; // Course
            int n = assignment[5]; // Classroom

            // Ensure the indices are within bounds
            if (i < 0 || i >= y.length || j < 0 || j >= y[i].length || k < 0 || k >= y[i][j].length ||
                    l < 0 || l >= y[i][j][k].length || m < 0 || m >= y[i][j][k][l].length || n < 0 || n >= y[i][j][k][l][m].length) {
                System.out.println("Invalid pre-assignment indices: " + Arrays.toString(assignment));
                continue;
            }

            // Add the constraint: x_{i,j,k,l,m,n} = 1
            model.addEq(y[i][j][k][l][m][n], 1.0);
        }
    }

    private void getSolution() throws IloException {
        if (model.solve()) {
            System.out.println("Timetable generated successfully.");
            System.out.println("Objective Value: " + model.getObjValue());

            int[] I = data.getI(); // Days
            int[] J = data.getJ(); // Periods
            int[] K = data.getK(); // Teachers
            int[] L = data.getL(); // Groups
            int[] M = data.getM(); // Courses
            int[] N = data.getN(); // Classrooms

            System.out.println("Generated Timetable:");
            for (int i = 0; i < I.length; i++) {
                for (int j = 0; j < J.length; j++) {
                    for (int k = 0; k < K.length; k++) {
                        for (int l = 0; l < L.length; l++) {
                            for (int m = 0; m < M.length; m++) {
                                for (int n = 0; n < N.length; n++) {
                                    // Check if the decision variable x[i][j][k][l][m][n] is selected
                                    if (model.getValue(x[i][j][k][l][m][n]) > 0.5) {
                                        System.out.println(
                                                "Day " + i +
                                                        ", Period " + j +
                                                        ", Teacher " + k +
                                                        ", Group " + l +
                                                        ", Course " + m +
                                                        ", Classroom " + n
                                        );
                                    }
                                }
                            }
                        }
                    }
                }
            }


            int P_max = 3; // Maximum number of repetitions for a session
            int H_max = data.getMaxSessionLengths(); // Maximum session lengths
            for (int i = 0; i < I.length; i++) {
                for (int pv = 0; pv < P_max; pv++) {
                    for (int k = 0; k < K.length; k++) {
                        for (int hv = 0; hv < H_max; hv++) {
                            for (int m = 0; m < M.length; m++) {
                                for (int n = 0; n < N.length; n++) {
                                    // Check if the decision variable y[i][pv][k][hv][m][n] is selected
                                    if (model.getValue(y[i][pv][k][hv][m][n]) > 0.5) {
                                        System.out.println(
                                                "Multi-Period Session: " +
                                                        "Day " + i +
                                                        ", Period Variation " + pv +
                                                        ", Teacher " + k +
                                                        ", Session Length " + hv +
                                                        ", Course " + m +
                                                        ", Classroom " + n
                                        );
                                    }
                                }
                            }
                        }
                    }
                }
            }
        } else {
            System.out.println("No feasible timetable could be generated. Check the constraints and input data.");
        }
    }

    public void solveModel() {
        try {
            if (model.solve()) {
                System.out.println("Solution status: " + model.getStatus());
                System.out.println("Objective value: " + model.getObjValue());

                // Print the solution (if needed)
                for (int i = 0; i < data.getI().length; i++) {
                    for (int j = 0; j < data.getJ().length; j++) {
                        for (int k = 0; k < data.getK().length; k++) {
                            for (int l = 0; l < data.getL().length; l++) {
                                for (int m = 0; m < data.getM().length; m++) {
                                    for (int n = 0; n < data.getN().length; n++) {
                                        if (model.getValue(x[i][j][k][l][m][n]) > 0.5) {
                                            System.out.printf("x[%d][%d][%d][%d][%d][%d] = 1\n", i, j, k, l, m, n);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                getSolution();
            } else {
                System.out.println("No solution found.");
                getSolution();
            }
        } catch (IloException e) {
            e.printStackTrace();
        }
    }
}