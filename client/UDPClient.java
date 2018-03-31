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
    private double failureRate;
    private int idCounter;
    private int semInvo;
    private Map<Integer, Boolean> handledResponse;

    public UDPClient(String ip, int port) throws SocketException, UnknownHostException{
        this.clientSocket = new DatagramSocket();
        this.clientSocket.setSoTimeout(Constants.TIMEOUT);
        this.IPAddress = InetAddress.getByName(ip);
        this.port = port;
        this.failureRate = Constants.FAILURE_RATE;
        this.idCounter = 0;
        this.semInvo = Constants.NO_SEM_INVO;
        this.handledResponse = new HashMap<Integer, Boolean>();
    }

    public void setFailureRate(double failureRate){
        this.failureRate = failureRate;
    }

    public void setSemInvo(int semInvo){
        this.semInvo = semInvo;
    }

    public int getID(){
        this.idCounter++;
        return this.idCounter;
    }

    public int getSemInvo(){
        return this.semInvo;
    }

    public void send(byte[] message) throws IOException, InterruptedException{
        if (Math.random() > this.failureRate){
            return;
        }

        byte[] header = Utils.marshal(message.length);
        DatagramPacket headerPacket = new DatagramPacket(header, header.length, this.IPAddress, this.port);
        this.clientSocket.send(headerPacket);

        DatagramPacket sendPacket = new DatagramPacket(message, message.length, this.IPAddress, this.port);
        this.clientSocket.send(sendPacket);
    }

    public byte[] receive() throws IOException{
        int responseID;
        int messageLength;
        DatagramPacket receivePacket;
        do{
            byte[] header = new byte[4];
            DatagramPacket headerPacket = new DatagramPacket(header, header.length);
            this.clientSocket.receive(headerPacket);

            messageLength = Utils.unmarshalInteger(headerPacket.getData(), 0);

            byte[] receiveData = new byte[messageLength];
            receivePacket = new DatagramPacket(receiveData, receiveData.length);
            this.clientSocket.receive(receivePacket);
            responseID = Utils.unmarshalInteger(receivePacket.getData(), 0);

            if (this.handledResponse.containsKey(responseID)){
                this.sendACK(responseID);
            }else{
                break;
            }
        } while(true);

        this.handledResponse.put(responseID, true);
        return Arrays.copyOfRange(receivePacket.getData(), Constants.INT_SIZE, messageLength);
    }

    public void sendACK(int curID) throws IOException, InterruptedException{
        List message = new ArrayList();
        Utils.append(message, Constants.ACK_CHAR);
        Utils.append(message, curID);

        this.send(Utils.byteUnboxing(message));
    }

    public byte[] sendAndReceive(byte[] packageByte, int curID) throws IOException, InterruptedException{
        byte[] response = new byte[0];
        do{
            try{
                this.send(packageByte);
                response = this.receive();
                break;
            } catch(SocketTimeoutException e){
                System.out.println(Constants.TIMEOUT_MSG);
                continue;
            }
        } while(this.getSemInvo() >= Constants.AT_LEAST_ONE_SEM_INVO);

        if(this.getSemInvo() >= Constants.AT_LEAST_ONE_SEM_INVO){
            this.sendACK(curID);
        }
        return response;
    }

    public static void main(String args[]){
        if(args.length < 2){
            System.out.println("Usage: UDPClient <IP ADDRESS> <PORT>");
            return;
        }
        try{
            System.out.println(Constants.SEPARATOR);
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

                byte[] packageByte;
                int curID = udpClient.getID();
                switch(serviceType){
                    case Constants.SERVICE_OPEN_ACCOUNT:
                        packageByte = HandleOpenAccount.createMessage(scanner, curID);
                        if (packageByte.length > 0){
                            byte[] response = udpClient.sendAndReceive(packageByte, curID);
                            try{
                                HandleOpenAccount.handleResponse(response);
                            } catch (Exception e){
                                System.out.println(Constants.SEPARATOR);
                                System.out.printf(Constants.ERR_MSG, e.getMessage());
                                System.out.println(Constants.SEPARATOR);
                            }
                        }
                        break;
                    case Constants.SERVICE_CLOSE_ACCOUNT:
                        packageByte = HandleCloseAccount.createMessage(scanner, curID);
                        if (packageByte.length != 0){
                            byte[] response = udpClient.sendAndReceive(packageByte, curID);
                            try{
                                HandleCloseAccount.handleResponse(response);
                            } catch (Exception e){
                                System.out.println(Constants.SEPARATOR);
                                System.out.printf(Constants.ERR_MSG, e.getMessage());
                                System.out.println(Constants.SEPARATOR);
                            }
                        }
                        break;
                    case Constants.SERVICE_DEPOSIT_MONEY:
                        packageByte = HandleDepositMoney.createMessage(scanner, curID);
                        if (packageByte.length != 0){
                            byte[] response = udpClient.sendAndReceive(packageByte, curID);
                            try{
                                HandleDepositMoney.handleResponse(response);
                            } catch (Exception e){
                                System.out.println(Constants.SEPARATOR);
                                System.out.printf(Constants.ERR_MSG, e.getMessage());
                                System.out.println(Constants.SEPARATOR);
                            }
                        }
                        break;
                    case Constants.SERVICE_WITHDRAW_MONEY:
                        packageByte = HandleWithdrawMoney.createMessage(scanner, curID);
                        if (packageByte.length != 0){
                            byte[] response = udpClient.sendAndReceive(packageByte, curID);
                            try{
                                HandleWithdrawMoney.handleResponse(response);
                            } catch (Exception e){
                                System.out.println(Constants.SEPARATOR);
                                System.out.printf(Constants.ERR_MSG, e.getMessage());
                                System.out.println(Constants.SEPARATOR);
                            }
                        }
                        break;
                    case Constants.SERVICE_MONITOR_UPDATE:
                        System.out.printf("Service %s, comming soon\n", message);
                        break;
                    case Constants.SERVICE_TRANSFER_MONEY:
                        packageByte = HandleTransferMoney.createMessage(scanner, curID);
                        if (packageByte.length != 0){
                            byte[] response = udpClient.sendAndReceive(packageByte, curID);
                            try{
                                HandleTransferMoney.handleResponse(response);
                            } catch (Exception e){
                                System.out.println(Constants.SEPARATOR);
                                System.out.printf(Constants.ERR_MSG, e.getMessage());
                                System.out.println(Constants.SEPARATOR);
                            }
                        }
                        break;
                    case Constants.SERVICE_CHANGE_PASSWORD:
                        packageByte = HandleChangePassword.createMessage(scanner, curID);
                        if (packageByte.length != 0){
                            byte[] response = udpClient.sendAndReceive(packageByte, curID);
                            try{
                                HandleChangePassword.handleResponse(response);
                            } catch (Exception e){
                                System.out.println(Constants.SEPARATOR);
                                System.out.printf(Constants.ERR_MSG, e.getMessage());
                                System.out.println(Constants.SEPARATOR);
                            }
                        }
                        break;
                    case Constants.SERVICE_EXIT:
                        System.out.println(Constants.EXIT_MSG);
                        exit = true;
                        break;
                    default:
                        System.out.println(Constants.UNRECOGNIZE_SVC_MSG);
                }
                System.out.println(Constants.SEPARATOR);
            }
        }
        catch(Exception e){
            System.out.println(Constants.SEPARATOR);
            System.out.printf(Constants.ERR_MSG, e.getMessage());
            System.out.println(Constants.SEPARATOR);
        }
    }
}
