import java.util.Arrays;
import java.util.Collection;
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
        // get sin data from database todo
        SinData sinData = new SinData(1.0, 0.1);
        SinData sinData2 = new SinData(2.0, 0.5);
        Collection<SinData> sinDataCollection = Arrays.asList(sinData, sinData2);

        SinFunctions2Audio sf2a = new SinFunctions2Audio();
        sf2a.createLinearCombo(sinDataCollection);
        // store sin waves in grid
        int[][] grid = createRandomGrid(10, 10);
        // smooth sounds
        PixelSoundSmoother pss = new PixelSoundSmoother();
        double[][] smoothGrid = pss.smoothGrid(grid, 3);

        // Use smoothened sin waves to create audio file
    }
}
