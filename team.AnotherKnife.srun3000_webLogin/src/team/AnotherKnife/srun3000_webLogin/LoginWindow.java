package team.AnotherKnife.srun3000_webLogin;

import javax.swing.*;
import java.awt.event.*;
import java.io.*;

public class LoginWindow extends JDialog {

	private JLayeredPane layeredPane;
	private JLabel label;
	private JLabel label_1;
	private JPasswordField PasswordField;
	private JTextField UserName;
	private JCheckBox SavePassword;
	private JCheckBox AutoLogin;
	private JButton Login;

	/**
	 * Create the dialog.
	 */
	public LoginWindow() {
		super();
		setModal(true);
		setResizable(false);
		setBounds(120, 120, 350, 120);
		setDefaultCloseOperation(JDialog.DISPOSE_ON_CLOSE);
		
		layeredPane = new JLayeredPane();
		this.getContentPane().add(layeredPane);
		
		label = new JLabel("账号：");
		label.setBounds(6, 6, 39, 16);
		layeredPane.add(label);
		
		label_1 = new JLabel("密码：");
		label_1.setBounds(6, 34, 39, 16);
		layeredPane.add(label_1);
		
		PasswordField = new JPasswordField();
		PasswordField.setBounds(57, 28, 289, 28);
		PasswordField.setText(String.valueOf(DataStorage.user_password));
		layeredPane.add(PasswordField);
		
		UserName = new JTextField();
		UserName.setBounds(57, 0, 289, 28);
		UserName.setText(String.valueOf(DataStorage.user_name));
		layeredPane.add(UserName);

		SavePassword = new JCheckBox("记住密码");
		SavePassword.setBounds(6, 62, 92, 23);
		SavePassword.setSelected(DataStorage.save_password);
		if (DataStorage.auto_login)
			SavePassword.setEnabled(false);
		layeredPane.add(SavePassword);
		
		AutoLogin = new JCheckBox("自动登录");
		AutoLogin.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				if (AutoLogin.isSelected())
				{
					SavePassword.setSelected(true);
					SavePassword.setEnabled(false);
				}
				else
					SavePassword.setEnabled(true);
			}
		});
		AutoLogin.setBounds(98, 62, 92, 23);
		AutoLogin.setSelected(DataStorage.auto_login);
		layeredPane.add(AutoLogin);
		
		Login = new JButton("登陆");
		Login.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				if (UserName.getText().equals(""))
					JOptionPane.showMessageDialog(null, "请输入账号！", "警告", JOptionPane.WARNING_MESSAGE);
				else if (PasswordField.getPassword().length == 0)
					JOptionPane.showMessageDialog(null, "请输入密码！", "警告", JOptionPane.WARNING_MESSAGE);
				else {
					String result = null;
					try {
                        result = DataStorage.Login(UserName.getText(), PasswordField.getPassword());
					} catch (Exception exception) {
                        JOptionPane.showMessageDialog(null, exception.getMessage(), "错误", JOptionPane.ERROR_MESSAGE);
					}
                    if (result != null) {
                        if (result.equals("success")) {
                            DataStorage.user_name = UserName.getText();
                            DataStorage.user_password = PasswordField.getPassword();
                            DataStorage.save_password = SavePassword.isSelected();
                            DataStorage.auto_login = AutoLogin.isSelected();
                            File path = new File("config");
                            try {
                                FileOutputStream writer = new FileOutputStream(path);
                                char[] saveInfo;
                                if (DataStorage.auto_login && DataStorage.save_password)
                                    saveInfo = ("true,true," + DataStorage.user_name + "," + String.valueOf(DataStorage.user_password) + "," + DataStorage.acid + "," + DataStorage.mac_address + "," + DataStorage.port).toCharArray();
                                else if (!DataStorage.auto_login && DataStorage.save_password)
                                    saveInfo = ("false,true," + DataStorage.user_name + "," + String.valueOf(DataStorage.user_password) + "," + DataStorage.acid + "," + DataStorage.mac_address + "," + DataStorage.port).toCharArray();
                                else
                                    saveInfo = ("false,false," + DataStorage.user_name + "," + DataStorage.acid + "," + DataStorage.mac_address + "," + DataStorage.port).toCharArray();
                                int[] code = new int[saveInfo.length];
                                for (int i = 0; i < saveInfo.length; i++) {
                                    code[i] = (int)saveInfo[i];
                                    code[i] ^= 0xAB;
                                }
                                try {
                                    for (int i = 0; i < code.length; i++)
                                        writer.write(code[i]);
                                    try {
                                        writer.close();
                                    } catch (IOException StreamCloseErr) {
                                        StreamCloseErr.printStackTrace();
                                    }
                                } catch (IOException WriteFileErr) {
                                    JOptionPane.showMessageDialog(null, "写入配置文件时发生错误！", "错误", JOptionPane.ERROR_MESSAGE);
                                    if (path.exists()) path.delete();
                                }
                            }
                            catch (FileNotFoundException FileNotFoundErr) {
                                try {
                                    path.createNewFile();
                                } catch (IOException CreateFileErr) {
                                    JOptionPane.showMessageDialog(null, "无法创建配置文件！", "错误", JOptionPane.ERROR_MESSAGE);
                                }
                            }
                            dispose();
                        }
                        else if (result.equals("login_error#E2531: User not found."))
                            JOptionPane.showMessageDialog(null, "用户名不存在！", "警告", JOptionPane.WARNING_MESSAGE);
                        else if (result.equals("login_error#E2553: Password is error."))
                            JOptionPane.showMessageDialog(null, "密码错误！", "警告", JOptionPane.WARNING_MESSAGE);
                        else
                            JOptionPane.showMessageDialog(null, "远程服务器信息：" + result, "警告", JOptionPane.WARNING_MESSAGE);
                    }
				}
			}
		});
		Login.setBounds(229, 61, 117, 29);
		layeredPane.add(Login);
	}
}
