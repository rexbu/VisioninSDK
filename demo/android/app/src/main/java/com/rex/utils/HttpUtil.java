package com.rex.utils;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Pair;
import javax.net.ssl.*;
import java.io.*;
import java.net.HttpURLConnection;
import java.net.Socket;
import java.net.URL;
import java.security.SecureRandom;
import java.security.cert.CertificateException;
import java.security.cert.X509Certificate;

/**
 * Created by Rex on 14-5-5.
 */
public class HttpUtil {

    public static HttpURLConnection getConnection(String urlStr) throws IOException {
        URL url = new URL(urlStr);
        HttpURLConnection conn = (HttpURLConnection)url.openConnection();
        conn.setDoInput(true);
        conn.connect();
        if (conn.getResponseCode() != 200) {
            return null;
        }
        return conn;
    }

    public static InputStream urlInputStream(HttpURLConnection conn) throws IOException {
        return conn.getInputStream();
    }

    public static InputStream urlInputStream(String urlStr) throws IOException {
        HttpURLConnection conn = getConnection(urlStr);
        return urlInputStream(conn);
    }

    public static int getContentLength(String urlStr) throws IOException {
        HttpURLConnection conn = getConnection(urlStr);
        return conn.getContentLength();
    }

    public static String get(InputStream is)throws IOException{
        BufferedReader br = new BufferedReader(new InputStreamReader(is));
        StringBuilder sb = new StringBuilder();
        String line = null;

        while ((line = br.readLine()) != null) {
            sb.append(line);
        }
        br.close();
        is.close();

        return sb.toString();
    }

    public static String get(String urlStr) throws IOException{
        return get(urlInputStream(urlStr));
    }

    public static Bitmap getBitMap(InputStream is) throws IOException {
        Bitmap bitmap = BitmapFactory.decodeStream(is);
        is.close();
        return bitmap;
    }

    public static Bitmap getBitMap(String urlStr) throws Exception {
        return getBitMap(urlInputStream(urlStr));
    }

    public static String get(String host, String query) throws IOException{
        Socket socket = new Socket(host, 80);
        StringBuilder html = new StringBuilder();
        //socket.connect(new InetSocketAddress("116.251.214.93", 80));
        // 写与的内容就是遵循HTTP请求协议格式的内容，请求百度
        StringBuilder requestHeader = new StringBuilder();
        requestHeader.append("GET " + "http://"+host+ query + " HTTP/1.0\r\n");
        requestHeader.append("Proxy-Connection: Keep-Alive\r\n");
        requestHeader.append("Host: " + host + "\r\n");
        requestHeader.append("Accept: Accept: textml,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n");
        requestHeader.append("User-Agent: Mozilla/5.0 (Windows NT 5.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/33.0.1707.0 Safari/537.36\r\n");
        requestHeader.append("Accept-Encoding: deflate,sdch\r\n");
        requestHeader.append("Accept-Language: zh-CN,zh;q=0.8\r\n");
        requestHeader.append("Referer: http://"+ host +"/\r\n");
        requestHeader.append("\r\n");
        socket.getOutputStream().write(requestHeader.toString().getBytes());

        byte[] buf = new byte[2048];
        InputStream is = socket.getInputStream();
        int i;

        while ((i = is.read(buf)) > 0) {
            html.append(new String(buf, 0, i, "UTF-8"));
        }
        is.close();
        socket.close();
        return html.toString();
    }

    public static String get(String https, Pair<String, String>[] headers){
        return https(https, "GET", headers, null);
    }
    public static String post(String https, Pair<String, String>[] headers, String body){
        return https(https, "POST", headers, body);
    }
    public static String put(String https, Pair<String, String>[] headers, String body){
        return https(https, "PUT", headers, body);
    }
    public static String delete(String https, Pair<String, String>[] headers){
        return https(https, "DELETE", headers, null);
    }
    public static String https(String https, String method, Pair<String, String>[] headers, String body){
//        try {
//            SSLContext sc = SSLContext.getInstance("TLS");
//            sc.init(null, new TrustManager[]{new MyTrustManager()}, new SecureRandom());
//            HttpsURLConnection.setDefaultSSLSocketFactory(sc.getSocketFactory());
//            HttpsURLConnection.setDefaultHostnameVerifier(new MyHostnameVerifier());
//            HttpURLConnection.setFollowRedirects(true);
//            HttpsURLConnection conn = (HttpsURLConnection)new URL(https).openConnection();
//            conn.setRequestMethod(method);
//            for (int i=0; i<headers.length; i++){
//                conn.setRequestProperty(headers[i].first, headers[i].second);
//            }
//            conn.setDoOutput(true);
//            conn.setDoInput(true);
//            if (body!=null){
//                DataOutputStream out = new DataOutputStream(conn.getOutputStream());
//                out.write(body.getBytes());
//                out.flush();
//                out.close();
//            }
//
//            conn.connect();
//            if (conn.getResponseCode() != 200) {
//                throw new Exception("服务器错误:"+conn.getResponseCode());
//            }
//
//            BufferedReader br = new BufferedReader(new InputStreamReader(conn.getInputStream()));
//            StringBuffer sb = new StringBuffer();
//            String line;
//            while ((line = br.readLine()) != null)
//                sb.append(line);
//
//            return sb.toString();
//        } catch (Exception e) {
//            e.printStackTrace();
//        }
        return null;
    }
//    static private class MyHostnameVerifier implements HostnameVerifier {
//
//        @Override
//        public boolean verify(String hostname, SSLSession session) {
//            // TODO Auto-generated method stub
//            return true;
//        }
//    }
//    static private class MyTrustManager implements X509TrustManager {
//
//        @Override
//        public void checkClientTrusted(X509Certificate[] chain, String authType)
//                throws CertificateException {
//            // TODO Auto-generated method stub
//
//        }
//
//        @Override
//        public void checkServerTrusted(X509Certificate[] chain, String authType)
//                throws CertificateException {
//            // TODO Auto-generated method stub
//
//        }
//
//        @Override
//        public X509Certificate[] getAcceptedIssuers() {
//            // TODO Auto-generated method stub
//            return null;
//        }
//    }
}
