package timetabling;

public class Main {
    public static void main(String[] args) {
        // Step 1: Initialize the data
        TimetablingData data = new TimetablingData();
        data.initializeData();

        // Step 2: Create the timetable model
        Timetable timetable = new Timetable(data);

        // Step 3: Create and solve the model
        timetable.createModel();
        timetable.solveModel();
    }
}
