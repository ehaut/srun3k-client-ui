package team.AnotherKnife.srun3000_webLogin;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

/**
 * Created by qianchengyu on 16/3/8.
 */
public class AdvancedConfig extends JFrame {

    private JLayeredPane layeredPane;
    private JLabel label1;
    private JLabel label2;
    private JLabel label3;
    private JTextField acid;
    private JTextField mac_address;
    private JTextField port;
    private JButton ok;
    private JButton cancel;

    public AdvancedConfig() {
        super();
        initialization();
    }

    public void initialization() {
        this.setResizable(false);
        this.setTitle("高级设置");
        this.setBounds(130, 130, 257, 158);
        this.setDefaultCloseOperation(JFrame.HIDE_ON_CLOSE);
        this.getContentPane().setLayout(new CardLayout(0, 0));

        layeredPane = new JLayeredPane();
        this.getContentPane().add(layeredPane);

        label1 = new JLabel("ACID:");
        label1.setBounds(12, 11, 65, 16);
        label2 = new JLabel("MAC地址:");
        label2.setBounds(12, 41, 65, 16);
        label3 = new JLabel("认证端口:");
        label3.setBounds(12, 69, 65, 16);

        layeredPane.add(label1);
        layeredPane.add(label2);
        layeredPane.add(label3);

        acid = new JTextField("1");
        acid.setBounds(78, 6, 169, 30);
        mac_address = new JTextField("");
        mac_address.setBounds(78, 34, 169, 30);
        port = new JTextField("69");
        port.setBounds(78, 62, 169, 30);

        layeredPane.add(acid);
        layeredPane.add(mac_address);
        layeredPane.add(port);

        ok = new JButton("确定");
        ok.setBounds(14, 96, 103, 32);
        ok.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                DataStorage.port = port.getText();
                DataStorage.acid = acid.getText();
                DataStorage.mac_address = MacFormat(mac_address.getText());
                dispose();
            }
        });
        cancel = new JButton("取消");
        cancel.setBounds(138, 96, 103, 32);
        cancel.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                dispose();
            }
        });

        layeredPane.add(ok);
        layeredPane.add(cancel);

    }

    private String MacFormat(String address) {
        char[] char_arr = address.toCharArray();
        for (int i = 0; i < char_arr.length; ++i) {
            if (char_arr[i] == '-')
                char_arr[i] = ':';
        }
        return String.valueOf(char_arr);
    }
}
