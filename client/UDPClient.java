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

        int messageLength = Utils.unmarshalInteger(headerPacket.getData(), 0);
        System.out.printf("Message length: %d\n", messageLength);

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
            System.out.println(Constants.WELCOME_MSG);
            System.out.println(Constants.SEPARATOR);

            UDPClient udpClient = new UDPClient(args[0],Integer.parseInt(args[1]));
            Scanner scanner = new Scanner(System.in);

            boolean exit = false;

            while(!exit){
                System.out.println(Constants.SELECTION_SVC_MSG);
                System.out.println(Constants.OPEN_ACCOUNT_SVC_MSG);
                System.out.println(Constants.CLOSE_ACCOUNT_SVC_MSG);
                System.out.println(Constants.DEPOSIT_MONEY_SVC_MSG);
                System.out.println(Constants.WITHDRAW_MONEY_SVC_MSG);
                System.out.println(Constants.MONITOR_UPDATE_SVC_MSG);
                System.out.println(Constants.TRANSFER_MONEY_SVC_MSG);
                System.out.println(Constants.CHANGE_PASSWORD_SVC_MSG);
                System.out.println(Constants.EXIT_SVC_MSG);
                System.out.println();
                System.out.print(Constants.CHOICE_SVC_MSG);

                String message = scanner.nextLine();
                int serviceType = Integer.parseInt(message);
                System.out.println();

                switch(serviceType){
                    case Constants.SERVICE_OPEN_ACCOUNT:
                        byte[] packageByte = HandleOpenAccount.createMessage(scanner);
                        if (packageByte.length != 0){
                            udpClient.send(packageByte);
                            byte[] response = udpClient.receive();
                            HandleOpenAccount.handleResponse(response);
                        }
                        break;
                    case Constants.SERVICE_CLOSE_ACCOUNT:
                        System.out.printf("Doing service: %s\n", message);
                        break;
                    case Constants.SERVICE_DEPOSIT_MONEY:
                        System.out.printf("Doing service: %s\n", message);
                        break;
                    case Constants.SERVICE_WITHDRAW_MONEY:
                        System.out.printf("Doing service: %s\n", message);
                        break;
                    case Constants.SERVICE_MONITOR_UPDATE:
                        System.out.printf("Doing service: %s\n", message);
                        break;
                    case Constants.SERVICE_TRANSFER_MONEY:
                        System.out.printf("Doing service: %s\n", message);
                        break;
                    case Constants.SERVICE_CHANGE_PASSWORD:
                        System.out.printf("Doing service: %s\n", message);
                        break;
                    case Constants.SERVICE_EXIT:
                        System.out.println(Constants.EXIT_MSG);
                        exit = true;
                        break;
                    default:
                        System.out.println(Constants.UNRECOGNIZE_SVC_MSG);
                }
                // udpClient.sendMessage(message);
                // udpClient.handleResponse();
                System.out.println(Constants.SEPARATOR);
            }
        }
        catch(IOException e){}
        catch(InterruptedException e){}
    }
}
