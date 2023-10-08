import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Random;

public class Main {
    private static Collection<SinData>[][] createRandomGrid(int rows, int cols) {
        Random random = new Random();
        Collection<SinData>[][] grid = new Collection[rows][cols];

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                ArrayList<SinData> sinData = new ArrayList<>();
                sinData.add(new SinData(random.nextDouble(0, 1), random.nextDouble(0, 8)));
                grid[i][j] = sinData;
            }
        }

        return grid;
    }


    public static void main(String[] args) {
        // get sin data from database
        GoogleCloudIO.establishConnection();
        // store sin waves in grid
        Collection<SinData>[][] grid = createRandomGrid(10, 10);
        // smooth sounds
        PixelSoundSmoother pss = new PixelSoundSmoother();
        Collection<SinData>[][] smoothGrid = pss.smoothGrid(grid, 3);

        System.out.println(Arrays.deepToString(grid));
        System.out.println(Arrays.deepToString(smoothGrid));
        // Use smoothened sin waves to create audio file
        System.out.println(smoothGrid[5][5]);
        Collection<SinData> sinDataCollection = smoothGrid[5][5];
        SinFunctions2Audio sf2a = new SinFunctions2Audio();
        sf2a.createLinearCombo(sinDataCollection);
    }
}
