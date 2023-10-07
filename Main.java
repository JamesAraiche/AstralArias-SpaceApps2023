import java.util.Arrays;
import java.util.Random;

public class Main {
    private static int[][] createRandomGrid(int rows, int cols) {
        Random random = new Random();
        int[][] grid = new int[rows][cols];

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                grid[i][j] = random.nextInt(50);
            }
        }

        return grid;
    }


    public static void main(String[] args) {
        // extract sound from image
        // store sounds in grid
        int[][] grid = createRandomGrid(10, 10);
        // smooth sounds
        PixelSoundSmoother pss = new PixelSoundSmoother();
        double[][] smoothGrid = pss.smoothGrid(grid, 3);
        System.out.println(Arrays.deepToString(smoothGrid));

        while (true) {
            // get mouse position
            // get sound at mouse position
            // play sound
            break;
        }
    }
}
