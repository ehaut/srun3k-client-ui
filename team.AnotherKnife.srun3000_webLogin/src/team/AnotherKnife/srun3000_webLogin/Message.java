package team.AnotherKnife.srun3000_webLogin;

import javax.swing.*;
import javax.swing.text.Document;
import javax.swing.text.html.HTMLEditorKit;
import javax.swing.text.html.StyleSheet;
import java.awt.*;

/**
 * Created by chn=student on 18/12/6.
 */

public class Message extends JDialog {
    private JEditorPane jEditorPane;
    private JScrollPane jScrollPane;
    private HTMLEditorKit htmlEditorKit;
    private StyleSheet styleSheet;
    private Document document;
    public Message(){
        super();
        setModal(true);
        setResizable(false);
        //setVisible(false);
        setSize(new Dimension(500,300));
        //setBounds(50, 50, 500, 300);
        setLocationRelativeTo(null);
        setDefaultCloseOperation(JDialog.DISPOSE_ON_CLOSE);
        jEditorPane = new JEditorPane();
        jEditorPane.setEditable(false);
        jScrollPane = new JScrollPane(jEditorPane);
        htmlEditorKit = new HTMLEditorKit();
        jEditorPane.setEditorKit(htmlEditorKit);
        styleSheet = htmlEditorKit.getStyleSheet();
        styleSheet.addRule("body {color:#000; font-family:simsun; margin: 4px; }");
        styleSheet.addRule("h3 {color: #000;font-size:20px;text-align:center;}");
        styleSheet.addRule("p {color: #000; font-size:18px;}");
        document = htmlEditorKit.createDefaultDocument();
        jEditorPane.setDocument(document);
        try {
            jEditorPane.setText(DataStorage.GetMessage());
        } catch (Exception e) {
            e.printStackTrace();
        }
        this.getContentPane().add(jScrollPane, BorderLayout.CENTER);
       // setVisible(true);
    }
}
