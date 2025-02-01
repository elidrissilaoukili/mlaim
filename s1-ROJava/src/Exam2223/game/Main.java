package Exam2223.game;

public class Main {

    public static void main(String[] args) {
        short[][] C = {
                {11, 13, 16, 41, 7, 17, 20},
                {9, 11, 18, 37, 13, 18, 46},
                {3, 23, 31, 53, 11, 17, 20}
        };

        Game game = new Game(C, 100);
        game.createModele();
    }
}
