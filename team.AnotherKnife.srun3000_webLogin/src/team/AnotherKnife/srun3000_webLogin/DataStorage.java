package team.AnotherKnife.srun3000_webLogin;

import javax.swing.*;
import java.net.URLEncoder;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.net.URLConnection;

import org.json.*;

public class DataStorage {

	public static boolean isLinked = false;

    // storage data
	public static String user_name = "";
	public static char[] user_password = new char[0];
	public static boolean save_password = false;
	public static boolean auto_login = false;
    public static String acid = "1";
    public static String mac_address = "";
    public static String port = "69";

    // displayed information
    public static String account_name = "";
	public static String used_data = "";
	public static String ip_address = "";
	public static long connectTime = 0;

    public static final String encoding_key = "1234567890";

	public static String Login(String usr, char[] pwd) throws Exception {
        String urlencode_usr = URLEncoder.encode(usrEncode(usr), "utf-8");
        String urlencode_pwd = URLEncoder.encode(pwdEncode(String.valueOf(pwd)), "utf-8");
        String urlencode_acid = URLEncoder.encode(acid, "utf-8");
        String urlencode_mac = URLEncoder.encode(mac_address, "utf-8");
        String data =
                "action=login&username="
                        + urlencode_usr
                        + "&password="
                        + urlencode_pwd
                        + "&drop=0&pop=1&type=2&n=117&mbytes=0&minutes=0&ac_id="
                        + urlencode_acid
                        + "&mac="
                        + urlencode_mac;
        String response = HttpPost("http://172.16.154.130:" + port + "/cgi-bin/srun_portal", data);
        if (response.contains("login_ok")) {
            isLinked = true;
            return "success";
        } else {
            isLinked = false;
            return response;
        }
	}
	
	public static String disconnect(String usr) throws Exception {
        String urlencode_usr = URLEncoder.encode(usrEncode(usr), "utf-8");
        String urlencode_acid = URLEncoder.encode(acid, "utf-8");
        String urlencode_mac = URLEncoder.encode(mac_address, "utf-8");
        String response = HttpPost(
                "http://172.16.154.130:" + port + "/cgi-bin/srun_portal",
                "action=logout&ac_id=" + urlencode_acid + "&username=" + urlencode_usr + "&mac=" + urlencode_mac + "&type=2"
        );
		isLinked = false;
        return response;
	}

    public static String HttpGet(String url) throws Exception{
        URL urlObj = new URL(null, url);
        URLConnection urlConnection = urlObj.openConnection();
        urlConnection.setRequestProperty("Content-Type", "application/x-www-form-urlencoded");
        HttpURLConnection httpURLConnection = (HttpURLConnection) urlConnection;
        httpURLConnection.setRequestMethod("GET");
        InputStream inputStream = urlConnection.getInputStream();
        int contentLength = urlConnection.getContentLength();
        contentLength = contentLength == -1 ? 4096 : contentLength;
        byte[] buffer = new byte[contentLength];
        int offset = 0;
        while (true) {
            int remain = buffer.length - offset;
            if (remain <= 0) {
                int newSize = buffer.length * 2;
                byte[] newBuffer = new byte[newSize];
                System.arraycopy(buffer, 0, newBuffer, 0, offset);
                buffer = newBuffer;
                remain = buffer.length - offset;
            }
            int numRead = inputStream.read(buffer, offset, remain);
            if (numRead == -1) {
                break;
            }
            offset += numRead;
        }
        if (offset < buffer.length) {
            byte[] newBuffer = new byte[offset];
            System.arraycopy(buffer, 0, newBuffer, 0, offset);
            buffer = newBuffer;
        }
        return new String(buffer, "UTF-8");
    }


    public static String GetMessage() throws Exception {
	    String reback="";
        try {
            String response = HttpGet("http://172.16.154.130/v2/srun_portal_message");
            if (!response.isEmpty()) {
                JSONObject jsonObject = new JSONObject(response);
                if(jsonObject.getInt("Code")==0&&jsonObject.getString("Message").equals("ok")) {
                    JSONArray jsonArray=jsonObject.getJSONArray("Data");
                    String title=jsonArray.getJSONObject(0).getString("Title");
                    String content=jsonArray.getJSONObject(0).getString("Content");
                    reback="<html><body><h3>"+title+"</h3><p>"+content+"</p></body>";
                    //System.out.println(reback);
                }
            }
        } catch (Exception e) {
            JOptionPane.showMessageDialog(null, e.getMessage(), "错误", JOptionPane.ERROR_MESSAGE);
        }
        return reback;
    }


    public static String HttpPost(String url, String data) throws Exception{
        URL urlObj = new URL(null, url);
        URLConnection urlConnection = urlObj.openConnection();
        urlConnection.setRequestProperty("Content-Type", "application/x-www-form-urlencoded");
        HttpURLConnection httpURLConnection = (HttpURLConnection) urlConnection;
        httpURLConnection.setRequestMethod("POST");
        httpURLConnection.setDoOutput(true);
        byte[] contentBytes = data.getBytes("UTF-8");
        httpURLConnection.setFixedLengthStreamingMode(contentBytes.length);
        OutputStream out = httpURLConnection.getOutputStream();
        try {
            out.write(contentBytes);
        } finally {
            out.flush();
        }
        InputStream inputStream = urlConnection.getInputStream();
        int contentLength = urlConnection.getContentLength();
        contentLength = contentLength == -1 ? 4096 : contentLength;
        byte[] buffer = new byte[contentLength];
        int offset = 0;
        while (true) {
            int remain = buffer.length - offset;
            if (remain <= 0) {
                int newSize = buffer.length * 2;
                byte[] newBuffer = new byte[newSize];
                System.arraycopy(buffer, 0, newBuffer, 0, offset);
                buffer = newBuffer;
                remain = buffer.length - offset;
            }
            int numRead = inputStream.read(buffer, offset, remain);
            if (numRead == -1) {
                break;
            }
            offset += numRead;
        }
        if (offset < buffer.length) {
            byte[] newBuffer = new byte[offset];
            System.arraycopy(buffer, 0, newBuffer, 0, offset);
            buffer = newBuffer;
        }
        return new String(buffer, "UTF-8");
    }

	public static void getUserInfo(){
        try {
            String response = HttpGet("http://172.16.154.130/cgi-bin/rad_user_info");
            if (!response.equals("not_online")) {
                String[] infoArray = response.split(",");
                account_name = infoArray[0];
                long data = Long.valueOf(infoArray[6]);
                data /= (1024 * 1024);
                used_data = String.valueOf(data) + "M";
                ip_address = infoArray[8];
                connectTime = Long.valueOf(infoArray[7]);
                connectTime += Long.valueOf(infoArray[2]) - Long.valueOf(infoArray[1]);
                isLinked = true;
            } else isLinked = false;
        } catch (Exception e) {
            JOptionPane.showMessageDialog(null, e.getMessage(), "错误", JOptionPane.ERROR_MESSAGE);
        }
	}

    public static String usrEncode(String usr) {
        String rtn = "{SRUN3}\r\n";
        char[] usr_arr = usr.toCharArray();
        for (int i = 0; i < usr_arr.length; ++i) {
            rtn += (char)((int)usr_arr[i] + 4);
        }
        return rtn;
    }

    public static String pwdEncode(String pwd) {
        String pe = "";
        for (int i = 0; i < pwd.length(); i++) {
            int ki = encoding_key.charAt(encoding_key.length() - i % encoding_key.length() - 1) ^ pwd.charAt(i);
            char _l = (char)((ki & 0x0F) + 0x36);
            char _h = (char)((ki >> 4 & 0x0F) + 0x63);
            if (i % 2 == 0) pe += String.valueOf(_l) + String.valueOf(_h);
            else pe += String.valueOf(_h) + String.valueOf(_l);
        }
        return pe;
    }

}
