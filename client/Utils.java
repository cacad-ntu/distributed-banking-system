package client;

import java.io.*;
import java.nio.*;
import java.lang.*;
import java.util.*;

/**
 * Utility class, contains utility method for client
 */
class Utils{
    /**
     * Marshaling String into bytes
     * @param s {@code String}
     * @return {@code byte[]} marshaled string
     * @throws UnsupportedEncodingExceptions
     * @since 1.9
     */
    public static byte[] marshal(String s) throws UnsupportedEncodingException{
        byte[] ret = new byte[s.length()];
        for(int i = 0; i < s.length(); i++) {
            ret[i] = (byte)s.charAt(i);
        }
        return ret;
    }

    /**
     * Unmarshaling bytes into String
     * @param b {@code byte[]}
     * @param start {@code int} start unmarshal index
     * @param end {@code int} end unmarshal index
     * @return {@code String} unmarshaled string
     * @since 1.9
     */
    public static String unmarshalString(byte[] b, int start, int end){
        char[] c = new char[end - start];
        for(int i = start; i < end; i++) {
            c[i-start] = (char)(b[i]);
        }
        return new String(c);
    }

    /**
     * Unmarshaling bytes into string, with header
     * @param b {@code byte[]}
     * @param start {@code int} start index of string header
     * @return {@code String} unmarshaled string
     * @since 1.9
     */
    public static String unmarshalMsgString(byte[] b, int start){
        int len = Utils.unmarshalInteger(b, start);
        int resStart = start + Constants.INT_SIZE;
        return Utils.unmarshalString(b, resStart, resStart+len);
    }


    /**
     * Marshaling int into bytes
     * @param x {@code int}
     * @return {@code byte[]} marshaled integer
     * @since 1.9
     */
    public static byte[] marshal(int x){
        return new byte[]{
          (byte)(x >> 24),
          (byte)(x >> 16),
          (byte)(x >> 8),
          (byte)(x >> 0)
        };
    }

    /**
     * Unmarshaling bytes into int
     * @param b {@code byte[]}
     * @param start {@code int} start unmarshal index
     * @return {@code int} unmarshaled int
     * @since 1.9
     */
    public static int unmarshalInteger(byte[] b, int start){
        return b[start] << 24 | (b[start+1] & 0xFF) << 16 | (b[start+2] & 0xFF) << 8 | (b[start+3] & 0xFF);
    }

    /**
     * Unmarshaling bytes into int, with header
     * @param b {@code byte[]}
     * @param start {@code int} start index of int header
     * @return {@code float} unmarshaled int
     * @since 1.9
     */
    public static int unmarshalMsgInteger(byte[] b, int start){
        return Utils.unmarshalInteger(b, start+Constants.INT_SIZE);
    }


    /**
     * Marshaling float into bytes
     * @param f {@code float}
     * @return {@code byte[]} marshaled float
     * @since 1.9
     */
    public static byte[] marshal(float f){
        return ByteBuffer.allocate(Constants.FLOAT_SIZE).putFloat(f).array();
    }

    /**
     * Unmarshaling bytes into float
     * @param b {@code byte[]}
     * @param start {@code float} start unmarshal index
     * @return {@code float} unmarshaled float
     * @since 1.9
     */
    public static float unmarshalFloat(byte[] b, int start){
        byte[] content = new byte[]{
            b[start], b[start+1], b[start+2], b[start+3]
        };
        return ByteBuffer.wrap(content).order(ByteOrder.BIG_ENDIAN).getFloat();
    }

    /**
     * Unmarshaling bytes into float, with header
     * @param b {@code byte[]}
     * @param start {@code int} start index of float header
     * @return {@code float} unmarshaled float
     * @since 1.9
     */
    public static float unmarshalMsgFloat(byte[] b, int start){
        return Utils.unmarshalFloat(b, start+Constants.INT_SIZE);
    }


    /**
     * Convert array of byte into array of Byte (Class)
     * @param b {@code byte[]}
     * @return {@code Byte[]} Bytes object
     * @since 1.9
     */
	public static Byte[] byteBoxing(byte[] b){
		Byte[] ret = new Byte[b.length];
		for(int i = 0; i < b.length; i++)
			ret[i] = Byte.valueOf(b[i]);
		return ret;
	}

    /**
     * Convert array of Byte (Class) into array of byte
     * @param b {@code Byte[]}
     * @return {@code byte[]} bytes
     * @since 1.9
     */
	public static byte[] byteUnboxing(Byte[] b){
		byte[] ret = new byte[b.length];
		for(int i = 0; i < b.length; i++)
			ret[i] = b[i].byteValue();
		return ret;
    }

    /**
     * Convert list into array of byte
     * @param list {@code List}
     * @return {@code byte[]} converted list
     * @since 1.9
     */
    public static byte[] byteUnboxing(List list){
        return Utils.byteUnboxing((Byte[])list.toArray(new Byte[list.size()]));
    }

    /**
     * Append String to list (as bytes) with the header length
     * @param list {@code List} original list
     * @param s {@code String} String to be appended
     * @return {@code void}
     * @throws UnsupportedEncodingException
     * @since 1.9
     */
    public static void appendMessage(List list, String s)throws UnsupportedEncodingException{
        list.addAll(Arrays.asList(Utils.byteBoxing(Utils.marshal(
            s.length()
        ))));

        list.addAll(Arrays.asList(Utils.byteBoxing(Utils.marshal(
            s
        ))));
    }

    /**
     * Append int to list (as bytes) with the header length
     * @param list {@code List} original list
     * @param x {@code int} int to be appended
     * @return {@code void}
     * @throws UnsupportedEncodingException
     * @since 1.9
     */
    public static void appendMessage(List list, int x)throws UnsupportedEncodingException{
        list.addAll(Arrays.asList(Utils.byteBoxing(Utils.marshal(
            Constants.INT_SIZE
        ))));

        list.addAll(Arrays.asList(Utils.byteBoxing(Utils.marshal(
            x
        ))));
    }

    /**
     * Append float to list (as bytes) with the header length
     * @param list {@code List} original list
     * @param f {@code float} float to be appended
     * @return {@code void}
     * @throws UnsupportedEncodingException
     * @since 1.9
     */
    public static void appendMessage(List list, float f)throws UnsupportedEncodingException{
        list.addAll(Arrays.asList(Utils.byteBoxing(Utils.marshal(
            Constants.FLOAT_SIZE
        ))));

        list.addAll(Arrays.asList(Utils.byteBoxing(Utils.marshal(
            f
        ))));
    }

    /**
     * Append bytes to list (as bytes) with the header length
     * @param list {@code List} original list
     * @param b {@code byte[]} bytes to be appended
     * @return {@code void}
     * @throws UnsupportedEncodingException
     * @since 1.9
     */
    public static void appendMessage(List list, byte[] b)throws UnsupportedEncodingException{
        list.addAll(Arrays.asList(Utils.byteBoxing(Utils.marshal(
            b.length
        ))));

        list.addAll(Arrays.asList(Utils.byteBoxing(
            b
        )));
    }

    /**
     * Append String to list (as bytes) without the header length
     * @param list {@code List} original list
     * @param s {@code String} String to be appended
     * @return {@code void}
     * @throws UnsupportedEncodingException
     * @since 1.9
     */
    public static void append(List list, String s)throws UnsupportedEncodingException{
        list.addAll(Arrays.asList(Utils.byteBoxing(Utils.marshal(
            s
        ))));
    }

    /**
     * Append int to list (as bytes) without the header length
     * @param list {@code List} original list
     * @param x {@code int} int to be appended
     * @return {@code void}
     * @throws UnsupportedEncodingException
     * @since 1.9
     */
    public static void append(List list, int x)throws UnsupportedEncodingException{
        list.addAll(Arrays.asList(Utils.byteBoxing(Utils.marshal(
            x
        ))));
    }

    /**
     * Append float to list (as bytes) without the header length
     * @param list {@code List} original list
     * @param f {@code float} float to be appended
     * @return {@code void}
     * @throws UnsupportedEncodingException
     * @since 1.9
     */
    public static void append(List list, float f)throws UnsupportedEncodingException{
        list.addAll(Arrays.asList(Utils.byteBoxing(Utils.marshal(
            f
        ))));
    }

    /**
     * Append bytes to list (as bytes) without the header length
     * @param list {@code List} original list
     * @param b {@code byte[]} bytes to be appended
     * @return {@code void}
     * @throws UnsupportedEncodingException
     * @since 1.9
     */
    public static void append(List list, byte[] b)throws UnsupportedEncodingException{
        list.addAll(Arrays.asList(Utils.byteBoxing(
            b
        )));
    }

}
