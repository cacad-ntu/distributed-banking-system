package client;

import java.io.*;
import java.lang.*;
import java.util.*;

class HandleChangePassword{
    public static byte[] createMessage(Scanner scanner, int id)throws UnsupportedEncodingException{
        System.out.println(Constants.SEPARATOR);
        System.out.println(Constants.CHANGE_MSG);

        // Enter Name
        System.out.print(Constants.CHANGE_NAME_MSG);
        String name = scanner.nextLine();
        while(name.length() == 0){
            System.out.println(Constants.ERR_NAME_INPUT);
            System.out.println();
            System.out.print(Constants.CHANGE_NAME_MSG);
            name = scanner.nextLine();
        }

        // Enter Account Number
        // TODO: Handle error
        System.out.print(Constants.CHANGE_ACC_NUM_MSG);
        String accNumStr = scanner.nextLine();
        int accNum = Integer.parseInt(accNumStr);

        // Enter Password
        System.out.print(Constants.CHANGE_PASSWORD_MSG);
        String password = scanner.nextLine();
        while(password.length() == 0){
            System.out.println(Constants.ERR_PASSWORD_INPUT);
            System.out.println();
            System.out.print(Constants.CHANGE_PASSWORD_MSG);
            password = scanner.nextLine();
        }

        // Enter Password
        System.out.print(Constants.CHANGE_NEW_PASSWORD_MSG);
        String newPassword = scanner.nextLine();
        while(newPassword.length() == 0){
            System.out.println(Constants.ERR_PASSWORD_INPUT);
            System.out.println();
            System.out.print(Constants.CHANGE_NEW_PASSWORD_MSG);
            newPassword = scanner.nextLine();
        }

        System.out.println();
        boolean confirm = HandleChangePassword.confirm(name, accNum, password, newPassword, scanner);
        System.out.println();

        if (confirm){
            return HandleChangePassword.constructMessage(name, accNum, password, newPassword, id);
        }
        return new byte[0];
    }

    public static boolean confirm(String name, int accountNumber, String password, String newPassword, Scanner scanner){
        System.out.println(Constants.SEPARATOR);
        System.out.println(Constants.CONFIRM_SUMMARY);
        System.out.println();
        System.out.printf(Constants.CONFIRM_NAME, name);
        System.out.printf(Constants.CONFIRM_ACCOUNT_NUMBER, accountNumber);
        System.out.printf(Constants.CONFIRM_PASSWORD, password);
        System.out.printf(Constants.CONFIRM_NEW_PASSWORD, newPassword);
        System.out.print(Constants.CONFIRM_MSG);
        String confirm = scanner.nextLine();

        if (confirm.equals(Constants.CONFIRM_YES)){
            return true;
        } else {
            return false;
        }
    }

    public static byte[] constructMessage(String name, int accountNumber, String password, String newPassword, int id)throws UnsupportedEncodingException{
        List message = new ArrayList();

        Utils.append(message, id);
        Utils.append(message, Constants.SERVICE_CHANGE_PASSWORD);
        Utils.appendMessage(message, name);
        Utils.appendMessage(message, accountNumber);
        Utils.appendMessage(message, password);
        Utils.appendMessage(message, newPassword);

        return Utils.byteUnboxing(message);
    }

    public static void handleResponse(byte[] response, boolean debug){
        System.out.println(Constants.SEPARATOR);
        String statusStr = Utils.unmarshalString(response, 0, Constants.RESPONSE_TYPE_SIZE);
        int status = Integer.parseInt(statusStr);
        if (debug) System.out.printf("[DEBUG][HandleChangePassword][Status = %d]\n", status);
        switch(status){
            case Constants.NAK:
                if (debug) System.out.println("[DEBUG][HandleChangePassword][Unsuccessful response]");
                String errMsg = Utils.unmarshalMsgString(response, Constants.RESPONSE_TYPE_SIZE);
                System.out.printf(Constants.ERR_MSG, errMsg);
                break;
            case Constants.ACK:
                if (debug) System.out.println("[DEBUG][HandleChangePassword][Successful response]");
                System.out.println(Constants.SUCCESSFUL_CHANGE_PASSWORD);
                break;
            default:
                System.out.println(Constants.INVALID_RESPONSE);
        }
        System.out.println();
        System.out.println(Constants.SEPARATOR);
    }
}