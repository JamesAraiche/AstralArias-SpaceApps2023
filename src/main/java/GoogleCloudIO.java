import java.io.*;
import java.sql.*;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Properties;

public class GoogleCloudIO {
    static Connection connection;

    public static void establishConnection() {
        Properties props = new Properties();

        try {
            InputStream dbProps = new FileInputStream("./database.properties");
            props.load(dbProps);

            String jdbcUrl = props.getProperty("db.url");
            jdbcUrl += "?user=" + props.getProperty("db.username");
            jdbcUrl += "&password=" + props.getProperty("db.password");

            dbProps.close();

            System.out.println(jdbcUrl);
            connection = DriverManager.getConnection(jdbcUrl);
        } catch (IOException | SQLException e) {
            e.printStackTrace();
        }
    }


    public static void closeConnection() {
        try {
            connection.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }


    public static Collection<SinData>[][] fetchSinData(int rows, int cols) {
        Collection<SinData>[][] grid = new Collection[rows][cols];

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                grid[i][j] = new ArrayList<>();
            }
        }

        String query = "SELECT x, y, frequency, amplitude FROM coordsBandAmount cba, sigBands sb WHERE cba.coordKey = sb.coordKey";

        try {
            PreparedStatement statement = connection.prepareStatement(query);
            ResultSet rs = statement.executeQuery();

            while (rs.next()) {
                grid[rs.getInt("x") - 1][rs.getInt("y") - 1].add(new SinData(rs.getDouble("frequency"), rs.getDouble("amplitude")));
            }

            statement.close();
            rs.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return grid;
    }


    public static void sendAudioFilesToDatabase(int rows, int cols) {
        try {
            PreparedStatement clearStatement = connection.prepareStatement("DELETE FROM audio");
            clearStatement.executeUpdate();

            int id = 0;
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    File file = new File("./audioFiles/audio" + i + j + ".wav");
                    FileInputStream fis = new FileInputStream(file);

                    PreparedStatement statement = connection.prepareStatement("INSERT INTO audio (id, x, y, track) VALUES (?, ?, ?, ?)");
                    statement.setInt(1, id++);
                    statement.setInt(2, i);
                    statement.setInt(3, j);
                    statement.setBinaryStream(4, fis, (int) file.length());
                    statement.executeUpdate();

                    statement.close();
                    fis.close();
                }
            }
        } catch (SQLException e) {
            e.printStackTrace();
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }
}
