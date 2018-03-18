package client;

import java.io.*;
import java.lang.*;
import java.util.*;

class HandleCloseAccount{
    public static byte[] createMessage(Scanner scanner)throws UnsupportedEncodingException{
        System.out.println(Constants.SEPARATOR);
        System.out.println(Constants.CLOSE_MSG);

        // Enter Name
        System.out.print(Constants.CLOSE_NAME_MSG);
        String name = scanner.nextLine();
        while(name.length() == 0){
            System.out.println(Constants.ERR_NAME_INPUT);
            System.out.println();
            System.out.print(Constants.CLOSE_NAME_MSG);
            name = scanner.nextLine();
        }

        // Enter Account Number
        // TODO: Handle error
        System.out.print(Constants.CLOSE_ACC_NUM_MSG);
        String accNumStr = scanner.nextLine();
        int accNum = Integer.parseInt(accNumStr);

        // Enter Password
        System.out.print(Constants.CLOSE_PASSWORD_MSG);
        String password = scanner.nextLine();
        while(password.length() == 0){
            System.out.println(Constants.ERR_PASSWORD_INPUT);
            System.out.println();
            System.out.print(Constants.CLOSE_PASSWORD_MSG);
            password = scanner.nextLine();
        }

        System.out.println();
        boolean confirm = HandleCloseAccount.confirm(name, accNum, password, scanner);
        System.out.println();

        if (confirm){
            return HandleCloseAccount.constructMessage(name, accNum, password);
        }
        return new byte[0];
    }

    public static boolean confirm(String name, int accountNumber, String password, Scanner scanner){
        System.out.println(Constants.SEPARATOR);
        System.out.println(Constants.CONFIRM_SUMMARY);
        System.out.println(Constants.SEPARATOR);
        System.out.printf(Constants.CONFIRM_NAME, name);
        System.out.printf(Constants.CONFIRM_ACCOUNT_NUMBER, accountNumber);
        System.out.printf(Constants.CONFIRM_PASSWORD, password);
        System.out.print(Constants.CONFIRM_MSG);
        String confirm = scanner.nextLine();

        if (confirm.equals(Constants.CONFIRM_YES)){
            return true;
        } else {
            return false;
        }
    }

    public static byte[] constructMessage(String name, int accountNumber, String password)throws UnsupportedEncodingException{
        List message = new ArrayList();

        Utils.appendType(message, Constants.SERVICE_CLOSE_ACCOUNT);
        Utils.appendMessage(message, name);
        Utils.appendMessage(message, accountNumber);
        Utils.appendMessage(message, password);

        return Utils.byteUnboxing(message);
    }

    public static void handleResponse(byte[] response){
        System.out.println(Constants.SEPARATOR);
        String statusStr = Utils.unmarshalString(response, 0, Constants.RESPONSE_TYPE_SIZE);
        int status = Integer.parseInt(statusStr);
        switch(status){
            case Constants.NAK:
                String errMsg = Utils.unmarshalString(response, Constants.RESPONSE_TYPE_SIZE, response.length);
                System.out.printf(Constants.ERR_MSG, errMsg);
                break;
            case Constants.ACK:
                System.out.println(Constants.SUCCESSFUL_CLOSE_ACCOUNT);
                break;
            default:
                System.out.println(Constants.INVALID_RESPONSE);
        }
        System.out.println();
        System.out.println(Constants.SEPARATOR);
    }
}