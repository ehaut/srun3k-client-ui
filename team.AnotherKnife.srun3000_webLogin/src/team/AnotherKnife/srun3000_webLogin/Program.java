package team.AnotherKnife.srun3000_webLogin;

import java.awt.*;

public class Program {
    /**
     * Launch the application.
     */
    public static void main(String[] args) {
        EventQueue.invokeLater(new Runnable() {
            public void run() {
                try {
                    MasterFrm window = new MasterFrm();
                    window.setVisible(true);
                } catch (Exception e) {
                    e.printStackTrace();
                    System.exit(1);
                }
            }
        });
    }
}
