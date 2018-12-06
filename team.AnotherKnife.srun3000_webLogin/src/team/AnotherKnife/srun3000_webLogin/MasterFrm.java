package team.AnotherKnife.srun3000_webLogin;

import java.awt.*;
import javax.swing.*;
import java.io.*;
import java.util.Timer;
import java.util.TimerTask;
import java.awt.event.*;

public class MasterFrm extends JFrame {
	
	private Timer tickTimer;
	private JLabel IPAddress;
	private JLabel UsedData;
	private JLabel UsedTime;
	private JLabel AccountName;
	private JButton ConnectNet;
	private JButton DisconnectNet;
	private JButton MoreConfig;
	private JLayeredPane layeredPane;
	private JLabel label;
	private JLabel label_1;
	private JLabel label_2;
	private JLabel label_3;
	private JButton DisplayMessage;
    private AdvancedConfig configWindow;
    private Message message;
	/**
	 * Create the application.
	 */
	public MasterFrm() {
        super();
		initialize();
	}

	/**
	 * Initialize the contents of the frame.
	 */
	private void initialize() {
        this.addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent e) {
                super.windowClosing(e);
				if (DataStorage.isLinked)
					if (JOptionPane.showConfirmDialog(null, "是否断开连接？", "消息", JOptionPane.YES_NO_OPTION) == 0) {
                        try {
                            DataStorage.disconnect(DataStorage.user_name);
                        } catch (Exception exception) {
                            JOptionPane.showMessageDialog(null, exception.getMessage(), "错误", JOptionPane.ERROR_MESSAGE);
                        }
					}
            }
        });
		this.setResizable(false);
		this.setTitle("河工大学校园网客户端");
		this.setBounds(100, 100, 364, 178);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		this.getContentPane().setLayout(new CardLayout(0, 0));

		File configFilePath = new File("config");
		try {
			FileInputStream reader = new FileInputStream(configFilePath);
			int i;
			String configInfo = "";
			while(true) {
				try {
					i = reader.read();
					if (i == -1)
						break;
					configInfo += String.valueOf((char)(i ^ 0xAB));
				} catch (IOException ReadFileErr) {
					JOptionPane.showMessageDialog(null, "读取配置文件发生错误！", "错误", JOptionPane.ERROR_MESSAGE);
					if (configFilePath.exists()) configFilePath.delete();
				}
			}
			try {
				reader.close();
			} catch (IOException StreamCloseErr) {
				StreamCloseErr.printStackTrace();
			}
			String[] strArr = configInfo.split(",");
			if (strArr[0].equals("true") && strArr[1].equals("true")) {
                DataStorage.user_name = strArr[2];
				DataStorage.user_password = strArr[3].toCharArray();
				DataStorage.save_password = true;
				DataStorage.auto_login = true;
                DataStorage.acid = strArr[4];
                DataStorage.mac_address = strArr[5];
                DataStorage.port = strArr[6];
			} else if (strArr[0].equals("false") && strArr[1].equals("true")) {
                DataStorage.user_name = strArr[2];
				DataStorage.user_password = strArr[3].toCharArray();
				DataStorage.save_password = true;
				DataStorage.auto_login = false;
                DataStorage.acid = strArr[4];
                DataStorage.mac_address = strArr[5];
                DataStorage.port = strArr[6];
			} else if (strArr[0].equals("false") && strArr[1].equals("false")) {
				DataStorage.user_name = strArr[2];
				DataStorage.save_password = false;
				DataStorage.auto_login = false;
                DataStorage.acid = strArr[3];
                DataStorage.mac_address = strArr[4];
                DataStorage.port = strArr[5];
			} else {
				if (configFilePath.exists()) {
                    configFilePath.delete();
					try {
						configFilePath.createNewFile();
					} catch (IOException CreateFileErr) {
						JOptionPane.showMessageDialog(null, "无法创建配置文件！", "错误", JOptionPane.ERROR_MESSAGE);
					}
				}
			}
		} catch (FileNotFoundException FileNotFoundErr) {
			try {
				configFilePath.createNewFile();
			}
			catch (IOException CreateFileErr) {
				JOptionPane.showMessageDialog(null, "无法创建配置文件！", "错误", JOptionPane.ERROR_MESSAGE);
			}
		}

		tickTimer = new Timer();

		layeredPane = new JLayeredPane();
		this.getContentPane().add(layeredPane);
		
		label = new JLabel("连接账号：");
		label.setBounds(6, 6, 65, 16);
		layeredPane.add(label);

		AccountName = new JLabel("-");
		AccountName.setBounds(83, 6, 275, 16);
		layeredPane.add(AccountName);
		
		label_1 = new JLabel("已用时长：");
		label_1.setBounds(6, 62, 65, 16);
		layeredPane.add(label_1);

		label_2 = new JLabel("已用流量：");
		label_2.setBounds(6, 90, 65, 16);
		layeredPane.add(label_2);

		UsedTime = new JLabel("-");
		UsedTime.setBounds(83, 62, 275, 16);
		layeredPane.add(UsedTime);

		UsedData = new JLabel("-");
		UsedData.setBounds(83, 90, 275, 16);
		layeredPane.add(UsedData);
		
		label_3 = new JLabel("登陆IP：");
		label_3.setBounds(6, 34, 61, 16);
		layeredPane.add(label_3);

		IPAddress = new JLabel("-");
		IPAddress.setBounds(83, 34, 275, 16);
		layeredPane.add(IPAddress);

		ConnectNet = new JButton("登陆");
        ConnectNet.setBounds(149, 116, 107, 32);
		ConnectNet.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				LoginWindow dialog = new LoginWindow();
				dialog.addWindowListener(new WindowAdapter() {
					@Override
					public void windowDeactivated(WindowEvent e) {
						super.windowDeactivated(e);
						if (DataStorage.isLinked)
						{
                            try {
                                Thread.sleep(1000);
                            } catch (InterruptedException e1) {
                                e1.printStackTrace();
                            }
                            tickTimer.cancel();
							tickTimer = new Timer();
							DataStorage.getUserInfo();
							AccountName.setText(DataStorage.account_name);
							UsedData.setText(DataStorage.used_data);
							IPAddress.setText(DataStorage.ip_address);
							tickTimer.schedule(new TimerTask() {
								@Override
								public void run() {
									long hour = (long)Math.floor(DataStorage.connectTime / 3600);
									int minute = (int)(Math.floor(DataStorage.connectTime - hour * 3600) / 60);
									int second = (int)Math.floor(DataStorage.connectTime - hour * 3600 - minute * 60);
									String minuteStr = minute < 10 ? "0" + String.valueOf(minute) : String.valueOf(minute);
									String secondStr = second < 10 ? "0" + String.valueOf(second) : String.valueOf(second);
									UsedTime.setText(String.valueOf(hour) + ":" + minuteStr + ":" + secondStr);
									DataStorage.connectTime++;
								}
							}, 0, 1000);
							ConnectNet.setEnabled(false);
                            DisconnectNet.setEnabled(true);
                            MoreConfig.setEnabled(false);
                            MoreConfig.setVisible(false);
                            DisplayMessage.setEnabled(true);
                            DisplayMessage.setVisible(true);
						}
					}
				});
				dialog.setVisible(true);
			}
		});
		layeredPane.add(ConnectNet);

		DisconnectNet = new JButton("断开连接");
        DisconnectNet.setBounds(254, 116, 107, 32);
		DisconnectNet.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				if (JOptionPane.showConfirmDialog(null, "是否要断开连接？", "信息", JOptionPane.YES_NO_OPTION) == 0) {
                    try {
                        //String result = DataStorage.disconnect(DataStorage.user_name);
                        String result = DataStorage.disconnect(DataStorage.account_name);
                        if (result.equals("logout_ok")) {
                            resetComponent();
                            JOptionPane.showMessageDialog(null, "注销成功！", "信息", JOptionPane.INFORMATION_MESSAGE);
                        }
                        else JOptionPane.showMessageDialog(null, result, "警告", JOptionPane.WARNING_MESSAGE);
                    } catch (Exception exception) {
                        JOptionPane.showMessageDialog(null, exception.getMessage(), "错误", JOptionPane.ERROR_MESSAGE);
                    }
				}
			}
		});
        DisconnectNet.setEnabled(false);
        layeredPane.add(DisconnectNet);

        configWindow = new AdvancedConfig();

        MoreConfig = new JButton("高级");
        MoreConfig.setBounds(6, 116, 107, 32);
        MoreConfig.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                configWindow.setVisible(true);
            }
        });

        DisplayMessage = new JButton("显示公告");
        DisplayMessage.setBounds(6, 116, 107, 32);
        DisplayMessage.addActionListener(new ActionListener() {
             @Override
             public void actionPerformed(ActionEvent e) {
                 //configWindow.setVisible(true);
                 message = new Message();
                 message.addWindowListener(new WindowAdapter() {
                     @Override
                     public void windowDeactivated(WindowEvent e) {
                         super.windowDeactivated(e);
                         if (DataStorage.isLinked)
                         {
                             try {
                                 Thread.sleep(1000);
                             } catch (InterruptedException e1) {
                                 e1.printStackTrace();
                             }
                             tickTimer.cancel();
                             tickTimer = new Timer();
                             DataStorage.getUserInfo();
                             AccountName.setText(DataStorage.account_name);
                             UsedData.setText(DataStorage.used_data);
                             IPAddress.setText(DataStorage.ip_address);
                             tickTimer.schedule(new TimerTask() {
                                 @Override
                                 public void run() {
                                     long hour = (long)Math.floor(DataStorage.connectTime / 3600);
                                     int minute = (int)(Math.floor(DataStorage.connectTime - hour * 3600) / 60);
                                     int second = (int)Math.floor(DataStorage.connectTime - hour * 3600 - minute * 60);
                                     String minuteStr = minute < 10 ? "0" + String.valueOf(minute) : String.valueOf(minute);
                                     String secondStr = second < 10 ? "0" + String.valueOf(second) : String.valueOf(second);
                                     UsedTime.setText(String.valueOf(hour) + ":" + minuteStr + ":" + secondStr);
                                     DataStorage.connectTime++;
                                 }
                             }, 0, 1000);
                             ConnectNet.setEnabled(false);
                             DisconnectNet.setEnabled(true);
                             MoreConfig.setEnabled(false);
                             MoreConfig.setVisible(false);
                             DisplayMessage.setEnabled(true);
                             DisplayMessage.setVisible(true);
                         }
                     }
                 });
                 message.setVisible(true);
             }
        });
        layeredPane.add(DisplayMessage);
        layeredPane.add(MoreConfig);

		DataStorage.getUserInfo();
		if (DataStorage.isLinked) {
			AccountName.setText(DataStorage.account_name);
			UsedData.setText(DataStorage.used_data);
			IPAddress.setText(DataStorage.ip_address);
			tickTimer.schedule(new TimerTask() {
				@Override
				public void run() {
					long hour = (long)Math.floor(DataStorage.connectTime / 3600);
					int minute = (int)(Math.floor(DataStorage.connectTime - hour * 3600) / 60);
					int second = (int)Math.floor(DataStorage.connectTime - hour * 3600 - minute * 60);
					String minuteStr = minute < 10 ? "0" + String.valueOf(minute) : String.valueOf(minute);
					String secondStr = second < 10 ? "0" + String.valueOf(second) : String.valueOf(second);
					UsedTime.setText(String.valueOf(hour) + ":" + minuteStr + ":" + secondStr);
					DataStorage.connectTime++;
				}
			}, 0, 1000);
            ConnectNet.setEnabled(false);
            DisconnectNet.setEnabled(true);
            MoreConfig.setEnabled(false);
            MoreConfig.setVisible(false);
            DisplayMessage.setEnabled(true);
            DisplayMessage.setVisible(true);
		} else if (DataStorage.auto_login) {
            try {
                String result = DataStorage.Login(DataStorage.user_name, DataStorage.user_password);
                if (result.equals("success")) {
                    try {
                        Thread.sleep(1000);
                    } catch (InterruptedException e1) {
                        e1.printStackTrace();
                    }
                    DataStorage.getUserInfo();
                    AccountName.setText(DataStorage.account_name);
                    UsedData.setText(DataStorage.used_data);
                    IPAddress.setText(DataStorage.ip_address);
                    tickTimer.schedule(new TimerTask() {
                        @Override
                        public void run() {
                            long hour = (long)Math.floor(DataStorage.connectTime / 3600);
                            int minute = (int)(Math.floor(DataStorage.connectTime - hour * 3600) / 60);
                            int second = (int)Math.floor(DataStorage.connectTime - hour * 3600 - minute * 60);
                            String minuteStr = minute < 10 ? "0" + String.valueOf(minute) : String.valueOf(minute);
                            String secondStr = second < 10 ? "0" + String.valueOf(second) : String.valueOf(second);
                            UsedTime.setText(String.valueOf(hour) + ":" + minuteStr + ":" + secondStr);
                            DataStorage.connectTime++;
                        }
                    }, 0, 1000);
                    ConnectNet.setEnabled(false);
                    DisconnectNet.setEnabled(true);
                    MoreConfig.setEnabled(false);
                    MoreConfig.setVisible(false);
                    DisplayMessage.setEnabled(true);
                    DisplayMessage.setVisible(true);
                }
                else if (result.equals("login_error#E2531: User not found."))
                    JOptionPane.showMessageDialog(null, "用户名不存在！", "警告", JOptionPane.WARNING_MESSAGE);
                else if (result.equals("login_error#E2553: Password is error."))
                    JOptionPane.showMessageDialog(null, "密码错误！", "警告", JOptionPane.WARNING_MESSAGE);
                else
                    JOptionPane.showMessageDialog(null, "远程服务器信息：" + result, "警告", JOptionPane.WARNING_MESSAGE);
            } catch (Exception e) {
                JOptionPane.showMessageDialog(null, e.getMessage(), "错误", JOptionPane.ERROR_MESSAGE);
            }
		}
	}
	
	private void resetComponent() {
		ConnectNet.setEnabled(true);
        DisconnectNet.setEnabled(false);
		MoreConfig.setEnabled(true);
        MoreConfig.setVisible(true);
        DisplayMessage.setEnabled(false);
        DisplayMessage.setVisible(false);
		DataStorage.isLinked = false;
		IPAddress.setText("-");
		UsedData.setText("-");
		UsedTime.setText("-");
		AccountName.setText("-");
		tickTimer.cancel();
		tickTimer = new Timer();
		DataStorage.connectTime = 0;
	}
}
