package SacdosProblem;

public class Main {

    public static void main(String[] args) {
        double[] b = {12, 2, 1, 4, 1};
        double[] c = {4, 2, 1, 10, 2};
        SacDos sac = new SacDos(c, b, 15);
        sac.getsolutionSac();
    }

}