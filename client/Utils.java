package client;

import java.io.*;

class Utils{
    public static byte[] marshal(String s) throws UnsupportedEncodingException{
        return s.getBytes("UTF-8");
     }

     public static byte[] marshal(int x){
        return new byte[]{
           (byte)(x >> 24),
           (byte)(x >> 16),
           (byte)(x >> 8),
           (byte)(x >> 0)
        };
     }

     public static int unmarshalInteger(byte[] b){
        return b[0] << 24 | (b[1] & 0xFF) << 16 | (b[2] & 0xFF) << 8 | (b[3] & 0xFF);
     }

     public static String unmarshalString(byte[] b){
        return new String(b);
     }

}