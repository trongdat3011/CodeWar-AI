import java.io.PrintWriter;

public class Task {
    private static int MAX_COLUMNS = 30;
    private static int MAX_ROWS = 20;
    private static boolean DEBUG_MODE = true;

    private class Cell {
        public int x;
        public int y;

        public String toString() {
            return Integer.toString(this.x) + ' ' + Integer.toString(this.y);
        }
    }

    public void solve(Main.InputReader in, PrintWriter out) {
        int nPlayers = in.nextInt();
        int myID = in.nextInt();

        String[] matrix = new String[MAX_ROWS];
        Cell[] positions = new Cell[nPlayers];

        for (int i = 0; i < MAX_ROWS; i++)
            matrix[i] = in.next();

        for (int i = 0; i < nPlayers; i++) {
            positions[i] = new Cell();
            positions[i].x = in.nextInt();
            positions[i].y = in.nextInt();
        }

        if (DEBUG_MODE) {
            System.out.println(nPlayers);
            System.out.println(myID);
            for (int i = 0; i < MAX_ROWS; i++)
                System.out.println(matrix[i]);
            for (int i = 0; i < nPlayers; i++)
                System.out.println(positions[i]);
        }
    }
}