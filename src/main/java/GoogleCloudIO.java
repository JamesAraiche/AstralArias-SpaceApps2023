import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
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
}
