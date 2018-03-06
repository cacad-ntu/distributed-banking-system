package client;

import java.io.*;
import java.net.*;
import java.util.Scanner;

class UDPClient
{
    private DatagramSocket clientSocket;
    private InetAddress IPAddress;
    private int port;

    public UDPClient(String ip, int port) throws SocketException, UnknownHostException{
        this.clientSocket = new DatagramSocket(8000);
        this.IPAddress = InetAddress.getByName(ip);
        this.port = port;
    }

    public void send(byte[] message) throws IOException, InterruptedException{
        // TODO: convert header packet as attribute
        byte[] header = Utils.marshal(message.length);
        DatagramPacket headerPacket = new DatagramPacket(header, header.length, this.IPAddress, this.port);
        this.clientSocket.send(headerPacket);

        DatagramPacket sendPacket = new DatagramPacket(message, message.length, this.IPAddress, this.port);
        this.clientSocket.send(sendPacket);
    }

    public byte[] receive() throws IOException{
        // TODO: convert header packet as attribute
        byte[] header = new byte[4];
        DatagramPacket headerPacket = new DatagramPacket(header, header.length);
        this.clientSocket.receive(headerPacket);

        int messageLength = Utils.unmarshalInteger(headerPacket.getData());
        System.out.printf("Message length: %d\n",messageLength);

        byte[] receiveData = new byte[messageLength];
        DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
        this.clientSocket.receive(receivePacket);

        return receivePacket.getData();
    }

    public static void main(String args[]){
        if(args.length < 2){
            System.out.println("Usage: UDPClient <IP ADDRESS> <PORT>");
            return;
        }
        try{
            UDPClient udpClient = new UDPClient(args[0],Integer.parseInt(args[1]));
            Scanner scanner = new Scanner(System.in);
            while(true){
                String message = scanner.nextLine();
                udpClient.send(Utils.marshal(message));

                String response = Utils.unmarshalString(udpClient.receive());
                System.out.println(response);
            }
        }
        catch(IOException e){}
        catch(InterruptedException e){}
    }
}