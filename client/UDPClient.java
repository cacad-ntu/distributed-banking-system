import java.io.*;
import java.net.*;
import java.util.Scanner;

class UDPClient
{
   private static DatagramSocket clientSocket;
   private static InetAddress IPAddress;
   private static int port;   
   
   public UDPClient(String ip, int portz) throws SocketException, UnknownHostException{
      clientSocket = new DatagramSocket();
      IPAddress = InetAddress.getByName(ip);
      port = portz;
   }

   public static void send(byte[] message) throws IOException{
      // TODO: convert header packet as attribute
      byte[] header = marshal(message.length);
      DatagramPacket headerPacket = new DatagramPacket(header, header.length, IPAddress, port);
      clientSocket.send(headerPacket);

      DatagramPacket sendPacket = new DatagramPacket(message, message.length, IPAddress, port);
      clientSocket.send(sendPacket);
   }

   public static byte[] marshal(String s){
      return s.getBytes();
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

   public static byte[] receive() throws IOException{
      // TODO: convert header packet as attribute
      byte[] header = new byte[4];
      DatagramPacket headerPacket = new DatagramPacket(header, header.length);
      clientSocket.receive(headerPacket);

      int messageLength = unmarshalInteger(headerPacket.getData());
      System.out.printf("Message length: %d\n",messageLength);

      byte[] receiveData = new byte[messageLength];
      DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
      clientSocket.receive(receivePacket);

      return receivePacket.getData();
   }

   public static void main(String args[]){
      System.out.printf("Arg0:%s\nArg1:%s\n",args[0],args[1]);
      try{
         UDPClient client = new UDPClient(args[0],Integer.parseInt(args[1]));
         Scanner scanner = new Scanner(System.in);
         while(true){
            String message = scanner.nextLine();
            client.send(marshal(message));

            String response = unmarshalString(client.receive());
            System.out.println(response);
         }
      }
      catch(IOException e){}
   }
}