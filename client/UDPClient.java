package client;

import java.io.*;
import java.net.*;
import java.lang.*;
import java.util.*;

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

    public void handler1(List list, String message){
        //type
        list.add(Byte.valueOf((byte)((char)
            1
        )));

        //value #bytes
        list.addAll(Arrays.asList(Utils.byteBoxing(Utils.marshal(
            4
        ))));

        //value
        list.addAll(Arrays.asList(Utils.byteBoxing(Utils.marshal(
            Float.parseFloat(message)
        ))));
    }

    public void handler2(List list, String message){
        //type
        list.add(Byte.valueOf((byte)((char)
            2
        )));

        //value #bytes
        list.addAll(Arrays.asList(Utils.byteBoxing(Utils.marshal(
            4
        ))));

        //value
        list.addAll(Arrays.asList(Utils.byteBoxing(Utils.marshal(
            Integer.parseInt(message)
        ))));
    }

    public void handler3(List list, String message) throws UnsupportedEncodingException{
        //type
        list.add(Byte.valueOf((byte)((char)
            3
        )));

        //value #bytes
        list.addAll(Arrays.asList(Utils.byteBoxing(Utils.marshal(
            message.length()
        ))));

        //value
        list.addAll(Arrays.asList(Utils.byteBoxing(Utils.marshal(
            message
        ))));
    }

	public void send(String message) throws IOException, InterruptedException{
        // TODO: convert header packet as attribute
        List list = new ArrayList();

		if(message.matches("[-+]?\\d*\\.\\d+"))
			handler1(list,message);
		else if(message.matches("[-+]?\\d+"))
			handler2(list,message);
		else
            handler3(list,message);

        byte[] bytes = Utils.byteUnboxing((Byte[])list.toArray(new Byte[list.size()]));
        byte[] header = Utils.marshal(bytes.length);
        DatagramPacket headerPacket = new DatagramPacket(header, header.length, this.IPAddress, this.port);
        this.clientSocket.send(headerPacket);

        DatagramPacket sendPacket = new DatagramPacket(bytes, bytes.length, this.IPAddress, this.port);
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

    public void handleResponse() throws IOException{
        byte[] response = this.receive();

        int type = (int)response[0];
        int length = Utils.unmarshalInteger(new byte[]{response[1],response[2],response[3],response[4]});

        byte[] value = new byte[length];
        for(int i=0;i<length;i++) value[i] = response[i+5];

        if(type == 1){
            System.out.printf("Received float: %f\n",Utils.unmarshalFloat(value));
        }
        else if(type == 2){
            System.out.printf("Received int: %d\n",Utils.unmarshalInteger(value));
        }
        else if(type == 3){
            System.out.printf("Received string: %s\n",Utils.unmarshalString(value));
        }
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
				udpClient.send(message);
                udpClient.handleResponse();
            }
        }
        catch(IOException e){}
        catch(InterruptedException e){}
    }
}
