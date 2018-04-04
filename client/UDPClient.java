package client;

import java.io.*;
import java.net.*;
import java.lang.*;
import java.util.*;
import java.util.concurrent.TimeoutException;

import org.apache.commons.cli.*;

class UDPClient
{
    private DatagramSocket clientSocket;
    private InetAddress IPAddress;
    private int port;
    private double failureRate;
    private int idCounter;
    private int semInvo;
    private int maxTimeout;
    private int timeout;
    private Map<Integer, Boolean> handledResponse;

    public UDPClient(String ip, int port) throws SocketException, UnknownHostException{
        this.clientSocket = new DatagramSocket();
        this.setTimeout(Constants.DEFAULT_NO_TIMEOUT, Constants.DEFAULT_MAX_TIMEOUT);

        this.IPAddress = InetAddress.getByName(ip);
        this.port = port;
        this.failureRate = Constants.DEFAULT_FAILURE_RATE;
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

    public void setTimeout(int timeout) throws SocketException{
        clientSocket.setSoTimeout(timeout);
        this.timeout = timeout;
    }

    public void setTimeout(int timeout, int maxTimeout) throws SocketException{
        clientSocket.setSoTimeout(timeout);
        this.timeout = timeout;
        this.maxTimeout = maxTimeout;
    }

    public int getID(){
        this.idCounter++;
        return this.idCounter;
    }

    public int getSemInvo(){
        return this.semInvo;
    }

    public int getTimeout(){
        return this.timeout;
    }

    public void send(byte[] message) throws IOException, InterruptedException{
        if (Math.random() < this.failureRate){
            System.out.println("SIMULATING SENDING FAILURE");
            return;
        }

        byte[] header = Utils.marshal(message.length);
        DatagramPacket headerPacket = new DatagramPacket(header, header.length, this.IPAddress, this.port);
        this.clientSocket.send(headerPacket);

        DatagramPacket sendPacket = new DatagramPacket(message, message.length, this.IPAddress, this.port);
        this.clientSocket.send(sendPacket);
    }

    public byte[] receive() throws IOException, InterruptedException{
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

            System.out.printf("RECEIVE RESPONSE: %d\n", responseID);
            if (this.semInvo >= Constants.AT_LEAST_ONE_SEM_INVO && this.handledResponse.containsKey(responseID)){
                this.sendACK(responseID);
            }else{
                break;
            }
        } while(this.semInvo >= Constants.AT_MOST_ONE_SEM_INVO);

        if(this.getSemInvo() >= Constants.AT_LEAST_ONE_SEM_INVO){
            System.out.printf("SEND ACK: %d\n", responseID);
            this.sendACK(responseID);
        }

        this.handledResponse.put(responseID, true);
        return Arrays.copyOfRange(receivePacket.getData(), Constants.INT_SIZE, messageLength);
    }

    public void sendACK(int curID) throws IOException, InterruptedException{
        List message = new ArrayList();
        Utils.append(message, curID);
        Utils.append(message, Constants.ACK_CHAR);

        this.send(Utils.byteUnboxing(message));
    }

    public byte[] sendAndReceive(byte[] packageByte, int curID) throws IOException, InterruptedException, TimeoutException{
        byte[] response = new byte[0];
        int timeoutCount = 0;
        do{
            try{
                this.send(packageByte);
                response = this.receive();
                break;
            } catch(SocketTimeoutException e){
                timeoutCount++;
                System.out.printf(Constants.TIMEOUT_MSG, timeoutCount, this.maxTimeout);
                if (this.maxTimeout > 0 && timeoutCount >= this.maxTimeout){
                    throw new TimeoutException(Constants.MAX_TIMEOUT_MSG);
                }
                continue;
            }
        } while(this.getSemInvo() >= Constants.AT_LEAST_ONE_SEM_INVO);
        return response;
    }

    public static void main(String[] args)throws Exception{
        Options options = new Options();

        Option opHost = new Option("h", "host", true, "Server host");
        opHost.setRequired(true);
        options.addOption(opHost);

        Option opPort = new Option("p", "port", true, "Server port");
        opPort.setRequired(true);
        opPort.setType(Integer.TYPE);
        options.addOption(opPort);

        Option opAtLeastOnce = new Option("al", "atleast", false, "Enable at least once invocation semantic");
        options.addOption(opAtLeastOnce);

        Option opAtMostOnce = new Option("am", "atmost", false, "Enable at most once invocation semantic");
        options.addOption(opAtMostOnce);

        Option opFailureRate = new Option("fr", "failurerate", true, "Set failure rate (float)");
        opFailureRate.setType(Double.TYPE);
        options.addOption(opFailureRate);

        Option opTimeout = new Option("to", "timeout", true, "Set timeout in millisecond");
        opTimeout.setType(Integer.TYPE);
        options.addOption(opTimeout);

        Option opTimeoutCount = new Option("mt", "maxtimeout", true, "Set timeout max count");
        opTimeoutCount.setType(Integer.TYPE);
        options.addOption(opTimeoutCount);

        CommandLineParser parser = new DefaultParser();
        HelpFormatter formatter = new HelpFormatter();
        CommandLine cmd;

        String host = Constants.DEFAULT_HOST;
        int port = Constants.DEFAULT_PORT;
        boolean atLeastOnce = false;
        boolean atMostOnce = false;
        double failureRate = Constants.DEFAULT_FAILURE_RATE;
        int timeout = Constants.DEFAULT_NO_TIMEOUT;
        int maxTimeout = Constants.DEFAULT_MAX_TIMEOUT;

        try {
            cmd = parser.parse(options, args);
            System.out.println(cmd.getParsedOptionValue("failurerate"));
            host = cmd.getOptionValue("host");
            port = Integer.parseInt(cmd.getOptionValue("port"));
            atLeastOnce = cmd.hasOption("atleast");
            if (atLeastOnce){
                timeout = Constants.DEFAULT_TIMEOUT;
            }
            atMostOnce = cmd.hasOption("atmost");
            if (atMostOnce){
                timeout = Constants.DEFAULT_TIMEOUT;
            }
            if (cmd.hasOption("failurerate")){
                failureRate = Double.parseDouble(cmd.getOptionValue("failurerate"));
            }
            if (cmd.hasOption("timeout")){
                timeout = Integer.parseInt(cmd.getOptionValue("timeout"));
            }
            if (cmd.hasOption("maxtimeout")){
                maxTimeout = Integer.parseInt(cmd.getOptionValue("maxtimeout"));
            }
        } catch (ParseException e) {
            System.out.println(e.getMessage());
            formatter.printHelp("UDPClient", options);

            System.exit(1);
            return;
        }

        try{
            System.out.print(Constants.SEPARATOR);
            System.out.println(Constants.WELCOME_MSG);
            System.out.println(Constants.SEPARATOR);

            UDPClient udpClient = new UDPClient(host, port);
            if (atLeastOnce){
                udpClient.setSemInvo(Constants.AT_LEAST_ONE_SEM_INVO);
                udpClient.setTimeout(timeout, maxTimeout);
            }
            if (atMostOnce){
                udpClient.setSemInvo(Constants.AT_MOST_ONE_SEM_INVO);
                udpClient.setTimeout(timeout, maxTimeout);
            }
            udpClient.setFailureRate(failureRate);

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
                        try{
                            packageByte = HandleOpenAccount.createMessage(scanner, curID);
                            if (packageByte.length > 0){
                                byte[] response = udpClient.sendAndReceive(packageByte, curID);
                                HandleOpenAccount.handleResponse(response);
                            }
                        } catch (Exception e){
                            System.out.print(Constants.SEPARATOR);
                            System.out.printf(Constants.ERR_MSG, e.getMessage());
                            throw(e);
                        }
                        break;
                    case Constants.SERVICE_CLOSE_ACCOUNT:
                        try{
                            packageByte = HandleCloseAccount.createMessage(scanner, curID);
                            if (packageByte.length != 0){
                                byte[] response = udpClient.sendAndReceive(packageByte, curID);
                                HandleCloseAccount.handleResponse(response);
                            }
                        } catch (Exception e){
                            System.out.print(Constants.SEPARATOR);
                            System.out.printf(Constants.ERR_MSG, e.getMessage());
                        }
                        break;
                    case Constants.SERVICE_DEPOSIT_MONEY:
                        try{
                            packageByte = HandleDepositMoney.createMessage(scanner, curID);
                            if (packageByte.length != 0){
                                byte[] response = udpClient.sendAndReceive(packageByte, curID);
                                HandleDepositMoney.handleResponse(response);
                            }
                        } catch (Exception e){
                            System.out.print(Constants.SEPARATOR);
                            System.out.printf(Constants.ERR_MSG, e.getMessage());
                        }
                        break;
                    case Constants.SERVICE_WITHDRAW_MONEY:
                        try{
                            packageByte = HandleWithdrawMoney.createMessage(scanner, curID);
                            if (packageByte.length != 0){
                                byte[] response = udpClient.sendAndReceive(packageByte, curID);
                                HandleWithdrawMoney.handleResponse(response);
                            }
                        } catch (Exception e){
                            System.out.print(Constants.SEPARATOR);
                            System.out.printf(Constants.ERR_MSG, e.getMessage());
                        }
                        break;
                    case Constants.SERVICE_MONITOR_UPDATE:
                        int realTimeout = udpClient.getTimeout();
                        try{
                            packageByte = HandleMonitorUpdate.createMessage(scanner, curID);
                            if (packageByte.length != 0){
                                System.out.println(Constants.SEPARATOR);
                                byte[] response = udpClient.sendAndReceive(packageByte, curID);
                                int remainingDuration = HandleMonitorUpdate.handleResponse(response);
                                do{
                                    udpClient.setTimeout(remainingDuration);
                                    byte[] update = udpClient.receive();
                                    remainingDuration = HandleMonitorUpdate.handleResponse(update);
                                } while(true);
                            }
                        } catch (SocketTimeoutException e){
                            udpClient.setTimeout(realTimeout);
                            System.out.println(Constants.MONITORING_FINISH_MSG);
                            System.out.println();
                            System.out.println(Constants.SEPARATOR);
                        } catch (Exception e){
                            udpClient.setTimeout(realTimeout);
                            System.out.print(Constants.SEPARATOR);
                            System.out.printf(Constants.ERR_MSG, e.getMessage());
                        }
                        break;
                    case Constants.SERVICE_TRANSFER_MONEY:
                        try{
                            packageByte = HandleTransferMoney.createMessage(scanner, curID);
                            if (packageByte.length != 0){
                                byte[] response = udpClient.sendAndReceive(packageByte, curID);
                                HandleTransferMoney.handleResponse(response);
                            }
                        } catch (Exception e){
                            System.out.print(Constants.SEPARATOR);
                            System.out.printf(Constants.ERR_MSG, e.getMessage());
                        }
                        break;
                    case Constants.SERVICE_CHANGE_PASSWORD:
                        try{
                            packageByte = HandleChangePassword.createMessage(scanner, curID);
                            if (packageByte.length != 0){
                                    byte[] response = udpClient.sendAndReceive(packageByte, curID);
                                    HandleChangePassword.handleResponse(response);
                            }
                        } catch (Exception e){
                            System.out.print(Constants.SEPARATOR);
                            System.out.printf(Constants.ERR_MSG, e.getMessage());
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
            System.out.print(Constants.SEPARATOR);
            System.out.printf(Constants.ERR_MSG, e.getMessage());
            System.out.println(Constants.EXIT_MSG);
            System.out.println(Constants.SEPARATOR);
            throw(e);
        }
    }
}
