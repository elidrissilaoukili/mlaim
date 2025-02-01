package exam2122.binPacking;

public class Main {

    public static void main(String[] args) {
        // Example data
        short[] itemSizes = {100, 22, 25, 51, 95};
        int binCapacity = 150;
        int maxBins = 2;

        // Create and solve the problem
        BinPacking binPacking = new BinPacking(itemSizes, binCapacity, maxBins);
        if (binPacking.solve()) {
            binPacking.getSolutionY();
            binPacking.getSolutionZ();
        }
    }

}
