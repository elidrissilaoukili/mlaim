package timetabling;

import java.util.*;

public class TimetablingData {

    // Fields to store the data
    private int[] I; // Days
    private int[] J; // Periods
    private int[] K; // Groups of students
    private int[] K1; // Lower-grade years
    private int[] K2; // Higher-grade years
    private int[] L; // Teachers
    private int[] M; // Courses
    private int[] N; // Classrooms

    private Map<Integer, int[]> K_l; // Groups for which teacher l offers some course
    private Map<Integer, int[]> L_i; // Teachers available on day i
    private Map<Integer, int[]> L_k; // Teachers teaching at least one course for group k
    private Map<Integer, int[]> L_m; // Teachers teaching course m
    private Map<String, int[]> L_km; // Intersection of L_k and L_m
    private Map<String, int[]> L_ki; // Intersection of L_k and L_i

    private Map<Integer, int[]> M_k; // Courses designed for group k
    private Map<Integer, int[]> M_l; // Courses taught by teacher l
    private Map<Integer, int[]> M_n; // Courses fitting in classroom n
    private Map<String, int[]> M_kl; // Intersection of M_k and M_l
    private Map<String, int[]> M_kn; // Intersection of M_k and M_n
    private Map<String, int[]> M_kln; // Intersection of M_k, M_l, and M_n

    private int[][] M_k_com; // Elective courses
    private int[] M_lab; // Lab courses

    private Map<String, int[]> N_mk; // Classrooms fitting group k for course m
    private Map<Integer, int[]> I_n; // Days classroom n is available
    private Map<Integer, int[]> I_l; // Days teacher l is available
    private Map<String, int[]> I_ln; // Intersection of I_l and I_n

    private Map<String, int[]> J_iln; // Periods on day i for teacher l and classroom n
    private Map<String, int[][]> JL_iln; // Time intervals for teacher l and classroom n
    private Map<String, int[]> FJL_iln; // Starting periods of intervals

    private Map<Integer, int[]> P_m;
    private Map<Integer, int[]> H_m; // Length of multi-period sessions

    private Map<Integer, Integer> a_k;

    private Map<Integer, Integer> b_m; // Total teaching periods required for course m
    private Map<Integer, Integer> s_l; // Total teaching periods required for teacher l

    private int[][] PRA; // A priori assignments

    public void initializeData() {
        // Days (zero-based indexing)
        I = new int[]{0}; // Day 0

        // Periods (zero-based indexing)
        J = new int[]{0}; // Period 0

        // Groups of students (zero-based indexing)
        K = new int[]{0, 1};
        K1 = new int[]{0}; // Lower-grade years (Group 0)
        K2 = new int[]{1}; // Higher-grade years (Group 1)

        // Teachers (zero-based indexing)
        L = new int[]{0}; // Teacher 0

        // Courses (zero-based indexing)
        M = new int[]{0}; // Course 0

        // Classrooms (zero-based indexing)
        N = new int[]{0}; // Classroom 0

        // Subsets of K (Groups of students)
        K_l = new HashMap<>();
        K_l.put(0, new int[]{0, 1}); // Teacher 0 teaches groups 0 and 1

        // Subsets of L (Teachers)
        L_i = new HashMap<>();
        L_i.put(0, new int[]{0}); // Teacher 0 is available on day 0

        L_k = new HashMap<>();
        L_k.put(0, new int[]{0}); // Teacher 0 teaches group 0
        L_k.put(1, new int[]{0}); // Teacher 0 teaches group 1

        L_m = new HashMap<>();
        L_m.put(0, new int[]{0}); // Teacher 0 teaches course 0

        L_km = new HashMap<>();
        L_km.put("0_0", new int[]{0}); // Teacher 0 teaches course 0 for group 0
        L_km.put("1_0", new int[]{0}); // Teacher 0 teaches course 0 for group 1

        // Subsets of L (Teachers available for group k on day i)
        L_ki = new HashMap<>();
        L_ki.put("0_0", new int[]{0}); // Teacher 0 is available on day 0 for group 0
        L_ki.put("1_0", new int[]{0}); // Teacher 0 is available on day 0 for group 1

        // Subsets of M (Courses)
        M_k = new HashMap<>();
        M_k.put(0, new int[]{0}); // Course 0 is for group 0
        M_k.put(1, new int[]{0}); // Course 0 is for group 1

        M_l = new HashMap<>();
        M_l.put(0, new int[]{0}); // Teacher 0 teaches course 0

        M_n = new HashMap<>();
        M_n.put(0, new int[]{0}); // Course 0 fits in classroom 0

        // Subsets of M (Courses taught by teacher l for group k)
        M_kl = new HashMap<>();
        M_kl.put("0_0", new int[]{0}); // Teacher 0 teaches course 0 for group 0
        M_kl.put("1_0", new int[]{0}); // Teacher 0 teaches course 0 for group 1

        // Subsets of N (Classrooms)
        N_mk = new HashMap<>();
        N_mk.put("0_0", new int[]{0}); // Classroom 0 fits group 0 for course 0
        N_mk.put("0_1", new int[]{0}); // Classroom 0 fits group 1 for course 0

        // Days when teacher l and classroom n are available
        I_ln = new HashMap<>();
        I_ln.put("0_0", new int[]{0}); // Teacher 0 and classroom 0 are available on day 0

        // Periods on day i when teacher l and classroom n are available
        J_iln = new HashMap<>();
        J_iln.put("0_0_0", new int[]{0}); // Period 0 on day 0 for teacher 0 and classroom 0

        // Elective courses offered by other divisions
        M_k_com = new int[][]{
                {1, 0, 0} // Elective course 0 taught by teacher 0 for group 1
        };

        // Total teaching periods for each group
        a_k = new HashMap<>();
        a_k.put(0, 1); // Group 0 requires 1 teaching period
        a_k.put(1, 1); // Group 1 requires 1 teaching period

        // Initialize b_m (teaching periods required for each course)
        b_m = new HashMap<>();
        b_m.put(0, 2); // Course 0 requires 2 teaching periods
        // b_m.put(1, 3); // Course 1 requires 3 teaching periods

        // Initialize s_l (teaching periods required for each teacher)
        s_l = new HashMap<>();
        s_l.put(0, 4); // Teacher 0 requires 4 teaching periods
        // s_l.put(1, 5); // Teacher 1 requires 5 teaching periods

        // Initialize H_m (possible session lengths for each course)
        H_m = new HashMap<>();
        H_m.put(0, new int[]{1, 2}); // Course 0 can have sessions of length 1 or 2
        H_m.put(1, new int[]{2, 3}); // Course 1 can have sessions of length 2 or 3

        P_m = new HashMap<>();
        P_m.put(0, new int[]{1, 2}); // Example data for course 0
        P_m.put(1, new int[]{1});    // Example data for course 1

        // Print some data for verification
        System.out.println("Days (I): " + Arrays.toString(I));
        System.out.println("Periods (J): " + Arrays.toString(J));
        System.out.println("Groups (K): " + Arrays.toString(K));
        System.out.println("Teachers (L): " + Arrays.toString(L));
        System.out.println("Courses (M): " + Arrays.toString(M));
        System.out.println("Classrooms (N): " + Arrays.toString(N));
    }

/*    public void initializeData() {
        // Days (zero-based indexing)
        I = new int[]{0, 1}; // Day 0 and Day 1

        // Periods (zero-based indexing)
        J = new int[]{0, 1}; // Period 0 and Period 1

        // Groups of students (zero-based indexing)
        K = new int[]{0, 1};
        K1 = new int[]{0}; // Lower-grade years (Group 0)
        K2 = new int[]{1}; // Higher-grade years (Group 1)

        // Teachers (zero-based indexing)
        L = new int[]{0, 1}; // Teacher 0 and Teacher 1

        // Courses (zero-based indexing)
        M = new int[]{0, 1}; // Course 0 and Course 1

        // Classrooms (zero-based indexing)
        N = new int[]{0, 1}; // Classroom 0 and Classroom 1

        // Subsets of K (Groups of students)
        K_l = new HashMap<>();
        K_l.put(0, new int[]{0, 1}); // Teacher 0 teaches groups 0 and 1
        K_l.put(1, new int[]{0, 1}); // Teacher 1 teaches groups 0 and 1

        // Subsets of L (Teachers)
        L_i = new HashMap<>();
        L_i.put(0, new int[]{0, 1}); // Teacher 0 is available on day 0 and day 1
        L_i.put(1, new int[]{0, 1}); // Teacher 1 is available on day 0 and day 1

        L_k = new HashMap<>();
        L_k.put(0, new int[]{0, 1}); // Teacher 0 and Teacher 1 teach group 0
        L_k.put(1, new int[]{0, 1}); // Teacher 0 and Teacher 1 teach group 1

        L_m = new HashMap<>();
        L_m.put(0, new int[]{0}); // Teacher 0 teaches course 0
        L_m.put(1, new int[]{1}); // Teacher 1 teaches course 1

        L_km = new HashMap<>();
        L_km.put("0_0", new int[]{0}); // Teacher 0 teaches course 0 for group 0
        L_km.put("1_0", new int[]{0}); // Teacher 0 teaches course 0 for group 1
        L_km.put("0_1", new int[]{1}); // Teacher 1 teaches course 1 for group 0
        L_km.put("1_1", new int[]{1}); // Teacher 1 teaches course 1 for group 1

        // Subsets of L (Teachers available for group k on day i)
        L_ki = new HashMap<>();
        L_ki.put("0_0", new int[]{0, 1}); // Teacher 0 and Teacher 1 are available on day 0 for group 0
        L_ki.put("1_0", new int[]{0, 1}); // Teacher 0 and Teacher 1 are available on day 0 for group 1
        L_ki.put("0_1", new int[]{0, 1}); // Teacher 0 and Teacher 1 are available on day 1 for group 0
        L_ki.put("1_1", new int[]{0, 1}); // Teacher 0 and Teacher 1 are available on day 1 for group 1

        // Subsets of M (Courses)
        M_k = new HashMap<>();
        M_k.put(0, new int[]{0}); // Course 0 is for group 0
        M_k.put(1, new int[]{1}); // Course 1 is for group 1

        M_l = new HashMap<>();
        M_l.put(0, new int[]{0}); // Teacher 0 teaches course 0
        M_l.put(1, new int[]{1}); // Teacher 1 teaches course 1

        M_n = new HashMap<>();
        M_n.put(0, new int[]{0, 1}); // Course 0 fits in classroom 0 and classroom 1
        M_n.put(1, new int[]{0, 1}); // Course 1 fits in classroom 0 and classroom 1

        // Subsets of M (Courses taught by teacher l for group k)
        M_kl = new HashMap<>();
        M_kl.put("0_0", new int[]{0}); // Teacher 0 teaches course 0 for group 0
        M_kl.put("1_0", new int[]{0}); // Teacher 0 teaches course 0 for group 1
        M_kl.put("0_1", new int[]{1}); // Teacher 1 teaches course 1 for group 0
        M_kl.put("1_1", new int[]{1}); // Teacher 1 teaches course 1 for group 1

        // Subsets of N (Classrooms)
        N_mk = new HashMap<>();
        N_mk.put("0_0", new int[]{0, 1}); // Classroom 0 and 1 fit group 0 for course 0
        N_mk.put("0_1", new int[]{0, 1}); // Classroom 0 and 1 fit group 1 for course 0
        N_mk.put("1_0", new int[]{0, 1}); // Classroom 0 and 1 fit group 0 for course 1
        N_mk.put("1_1", new int[]{0, 1}); // Classroom 0 and 1 fit group 1 for course 1

        // Days when teacher l and classroom n are available
        I_ln = new HashMap<>();
        I_ln.put("0_0", new int[]{0, 1}); // Teacher 0 and classroom 0 are available on day 0 and day 1
        I_ln.put("0_1", new int[]{0, 1}); // Teacher 0 and classroom 1 are available on day 0 and day 1
        I_ln.put("1_0", new int[]{0, 1}); // Teacher 1 and classroom 0 are available on day 0 and day 1
        I_ln.put("1_1", new int[]{0, 1}); // Teacher 1 and classroom 1 are available on day 0 and day 1

        // Periods on day i when teacher l and classroom n are available
        J_iln = new HashMap<>();
        J_iln.put("0_0_0", new int[]{0, 1}); // Period 0 and 1 on day 0 for teacher 0 and classroom 0
        J_iln.put("0_0_1", new int[]{0, 1}); // Period 0 and 1 on day 0 for teacher 0 and classroom 1
        J_iln.put("0_1_0", new int[]{0, 1}); // Period 0 and 1 on day 0 for teacher 1 and classroom 0
        J_iln.put("0_1_1", new int[]{0, 1}); // Period 0 and 1 on day 0 for teacher 1 and classroom 1
        J_iln.put("1_0_0", new int[]{0, 1}); // Period 0 and 1 on day 1 for teacher 0 and classroom 0
        J_iln.put("1_0_1", new int[]{0, 1}); // Period 0 and 1 on day 1 for teacher 0 and classroom 1
        J_iln.put("1_1_0", new int[]{0, 1}); // Period 0 and 1 on day 1 for teacher 1 and classroom 0
        J_iln.put("1_1_1", new int[]{0, 1}); // Period 0 and 1 on day 1 for teacher 1 and classroom 1

        // Elective courses offered by other divisions
        M_k_com = new int[][]{
                {1, 0, 0} // Elective course 0 taught by teacher 0 for group 1
        };

        // Lab courses
        M_lab = new int[]{1}; // Course 1 is a lab course

        // Total teaching periods for each group
        a_k = new HashMap<>();
        a_k.put(0, 2); // Group 0 requires 2 teaching periods
        a_k.put(1, 2); // Group 1 requires 2 teaching periods

        // Initialize b_m (teaching periods required for each course)
        b_m = new HashMap<>();
        b_m.put(0, 2); // Course 0 requires 2 teaching periods
        b_m.put(1, 2); // Course 1 requires 2 teaching periods

        // Initialize s_l (teaching periods required for each teacher)
        s_l = new HashMap<>();
        s_l.put(0, 4); // Teacher 0 requires 4 teaching periods
        s_l.put(1, 4); // Teacher 1 requires 4 teaching periods

        // Initialize H_m (possible session lengths for each course)
        H_m = new HashMap<>();
        H_m.put(0, new int[]{1, 2}); // Course 0 can have sessions of length 1 or 2
        H_m.put(1, new int[]{2, 3}); // Course 1 can have sessions of length 2 or 3

        P_m = new HashMap<>();
        P_m.put(0, new int[]{1, 2}); // Example data for course 0
        P_m.put(1, new int[]{1});    // Example data for course 1

        // Print some data for verification
        System.out.println("Days (I): " + Arrays.toString(I));
        System.out.println("Periods (J): " + Arrays.toString(J));
        System.out.println("Groups (K): " + Arrays.toString(K));
        System.out.println("Teachers (L): " + Arrays.toString(L));
        System.out.println("Courses (M): " + Arrays.toString(M));
        System.out.println("Classrooms (N): " + Arrays.toString(N));
    }*/

    // Getter methods for accessing the data
    public int[] getI() {
        return I;
    }

    public int[] getJ() {
        return J;
    }

    public int[] getK() {
        return K;
    }

    public int[] getK1() {
        return K1;
    }

    public int[] getK2() {
        return K2;
    }

    public int[] getL() {
        return L;
    }

    public int[] getM() {
        return M;
    }

    public int[] getN() {
        return N;
    }

    public Map<Integer, int[]> getK_l() {
        return K_l;
    }

    public int[] getK_l(int l) {
        return K_l.get(l);
    }

    public Map<Integer, int[]> getL_i() {
        return L_i;
    }

    public int[] getL_k(int k) {
        return L_k.getOrDefault(k, new int[]{});
    }

    public Map<Integer, int[]> getL_m() {
        return L_m;
    }

    public Map<String, int[]> getL_km() {
        return L_km;
    }

    // public Map<String, int[]> getL_ki(int k, int i) {return L_ki;}
    public int[] getL_ki(int k, int i) {
        // Construct the key for the map
        String key = k + "_" + i;
        // Return the teachers for the given group and day, or an empty array if the key is not found
        return L_ki.getOrDefault(key, new int[]{});
    }

    public Map<Integer, int[]> getM_k() {
        return M_k;
    }

    public int[] getM_k(int k) {
        return M_k.get(k);
    }

    public Map<Integer, int[]> getM_l() {
        return M_l;
    }

    public Map<Integer, int[]> getM_n() {
        return M_n;
    }

    public Map<String, int[]> getM_kl() {
        return M_kl;
    }

    public int[] getM_kl(int k, int l) {
        return M_kl.get(k + "_" + l);
    }

    public Map<String, int[]> getM_kn() {
        return M_kn;
    }

    public Map<String, int[]> getM_kln(int k, int l, int n) {
        return M_kln;
    }

    public int[][] getM_k_com() {
        return M_k_com;
    }

    public int[] getM_lab() {
        return M_lab;
    }

    public Map<String, int[]> getN_mk() {
        return N_mk;
    }

    public int[] getN_mk(int m, int k) {
        return N_mk.get(m + "_" + k);
    }

    public Map<Integer, int[]> getI_n() {
        return I_n;
    }

    public Map<Integer, int[]> getI_l() {
        return I_l;
    }

    public int[] getI_ln(int l, int n) {
        String key = l + "_" + n;
        return I_ln.getOrDefault(key, new int[]{});
    }

    public int[] getJ_iln(int i, int l, int n) {
        String key = i + "_" + l + "_" + n;
        return J_iln.getOrDefault(key, new int[]{});
    }

    public int getA_k(int k) {
        return a_k.getOrDefault(k, 0);
    }

    public Map<String, int[][]> getJL_iln() {
        return JL_iln;
    }

    public Map<String, int[]> getFJL_iln() {
        return FJL_iln;
    }

    public Map<Integer, int[]> getP_m() {
        return P_m;
    }

    public Map<Integer, int[]> getH_m() {
        return H_m;
    }

    // Getter methods for b_m and s_l
    public int getB_m(int m) {
        return b_m.getOrDefault(m, 0);
    }

    public int getS_l(int l) {
        return s_l.getOrDefault(l, 0);
    }

    public int[] getH_m(int m) {
        return H_m.getOrDefault(m, new int[]{1}); // Default to single-period sessions
    }

    public int getMaxSessionLengths() {
        int maxLength = 0;
        for (int[] lengths : H_m.values()) {
            if (lengths.length > maxLength) {
                maxLength = lengths.length;
            }
        }
        return maxLength;
    }

    // public int[][] getPRA() {return PRA;}

    public int[][] getPRA() {
        return new int[][]{
                {0, 0, 0, 0, 0, 0}, // Pre-assign day 0, period 0, group 0, teacher 0, course 0, classroom 0
                {0, 1, 1, 0, 0, 0}  // Pre-assign day 0, period 1, group 1, teacher 0, course 0, classroom 0
        };
    }
}