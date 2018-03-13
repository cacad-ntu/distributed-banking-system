package client;

import java.io.*;
import java.nio.*;

class Utils{
    // Marshal and unmarshal String
    public static byte[] marshal(String s) throws UnsupportedEncodingException{
        byte[] ret = new byte[s.length()];
        for(int i=0;i<s.length();i++) ret[i] = (byte)s.charAt(i);
        return ret;
    }

    public static String unmarshalString(byte[] b){
        char[] c = new char[b.length];
        for(int i=0;i<b.length;i++) c[i] = (char)(b[i]);
        return new String(c);
    }


    // Marshal and unmarshal int
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


    // Marshal and unmarshal float
    public static byte[] marshal(float x){
        return ByteBuffer.allocate(4).putFloat(x).array();
    }

    public static float unmarshalFloat(byte[] b){
        return ByteBuffer.wrap(b).order(ByteOrder.BIG_ENDIAN).getFloat();
    }


    // Boxing and unboxing bytes
	public static Byte[] byteBoxing(byte[] b){
		Byte[] ret = new Byte[b.length];
		for(int i=0;i<b.length;i++)
			ret[i] = Byte.valueOf(b[i]);
		return ret;
	}

	public static byte[] byteUnboxing(Byte[] b){
		byte[] ret = new byte[b.length];
		for(int i=0;i<b.length;i++)
			ret[i] = b[i].byteValue();
		return ret;
	}
}
