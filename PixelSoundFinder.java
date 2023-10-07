import java.util.*;

public class PixelSoundFinder {

    static class Coord {
        int x, y;

        public Coord(int x, int y) {
            this.x = x;
            this.y = y;
        }

        @Override
        public String toString() {
            return "(" + x + ", " + y + ")";
        }
    }

    private int[][] createRandomGrid(int rows, int cols) {
        Random random = new Random();
        int[][] grid = new int[rows][cols];

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                grid[i][j] = random.nextInt(50);
            }
        }

        return grid;
    }


    private Set<Coord> getNeighbours(int[][] g, Coord coord) {
        int rows = g.length;
        int cols = g[0].length;
        int x = coord.x;
        int y = coord.y;

        Set<Coord> neighbours = new HashSet<>();

        if (x > 0) {
            neighbours.add(new Coord(x - 1, y));
        }
        if (x < rows - 1) {
            neighbours.add(new Coord(x + 1, y));
        }
        if (y > 0) {
            neighbours.add(new Coord(x, y - 1));
        }
        if (y < cols - 1) {
            neighbours.add(new Coord(x, y + 1));
        }

        return neighbours;
    }


    private double findPixelSound(int[][] g, int x, int y, int radius) {
        Queue<Coord> q = new ArrayDeque<>();
        Map<Coord, Integer> distances = new HashMap<>();

        Coord start = new Coord(x, y);
        q.add(start);
        distances.put(start, 0);

        int distance = 1;

        while (distance < radius) {
            for (int i = q.size(); i > 0; i--) {
                Coord current = q.poll();
                assert current != null;

                Set<Coord> neighbours = getNeighbours(g, current);
                for (Coord neighbour : neighbours) {
                    if (!distances.containsKey(neighbour)) {
                        q.add(neighbour);
                        distances.put(neighbour, distance);
                    }
                }
            }

            distance++;
        }

        System.out.println(distances);

        return(calculatePixelSound(g, distances));
    }


    private double calculatePixelSound(int[][] g, Map<Coord, Integer> distances) {
        double pixelSound = 0;

        for (Coord coord : distances.keySet()) {
            System.out.println("coord: " + coord + ", distance: " + distances.get(coord) + ", value: " + g[coord.x][coord.y]);
            pixelSound += g[coord.x][coord.y] * Math.pow(0.5, distances.get(coord));
        }

        return pixelSound;
    }


    public static void main(String[] args) {
        PixelSoundFinder psf = new PixelSoundFinder();
        int[][] grid = psf.createRandomGrid(10, 10);
        System.out.println(psf.findPixelSound(grid, 1, 1, 3));
    }
}

