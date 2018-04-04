package client;


import java.io.*;
import java.lang.*;
import java.util.*;

class HandleOpenAccount{

    public static byte[] createMessage(Scanner scanner, int id)throws UnsupportedEncodingException{
        System.out.println(Constants.SEPARATOR);
        System.out.println(Constants.OPEN_MSG);

        // Enter Name
        System.out.print(Constants.OPEN_NAME_MSG);
        String name = scanner.nextLine();
        while(name.length() == 0){
            System.out.println(Constants.ERR_NAME_INPUT);
            System.out.println();
            System.out.print(Constants.OPEN_NAME_MSG);
            name = scanner.nextLine();
        }

        // Enter Password
        System.out.print(Constants.OPEN_PASSWORD_MSG);
        String password = scanner.nextLine();
        while(password.length() == 0){
            System.out.println(Constants.ERR_PASSWORD_INPUT);
            System.out.println();
            System.out.print(Constants.OPEN_PASSWORD_MSG);
            password = scanner.nextLine();
        }

        // Enter Default Currency
        // TODO: Handle error
        System.out.println(Constants.OPEN_SELECT_CURRENCY_MSG);
        for (int i = 1; i < Constants.CURRENCY_STR.length; i++){
            System.out.printf("%d. %s\n", i, Constants.CURRENCY_STR[i]);
        }
        System.out.print(Constants.OPEN_CURRENCY_MSG);
        String currencyStr = scanner.nextLine();
        int currency = Integer.parseInt(currencyStr);

        // Enter Starting Balance
        // TODO: Handle error
        System.out.print(Constants.OPEN_BALANCE_MSG);
        String balanceStr = scanner.nextLine();
        float balance = Float.parseFloat(balanceStr);

        System.out.println();
        boolean confirm = HandleOpenAccount.confirm(name, password, currency, balance, scanner);
        System.out.println();

        if (confirm){
            return HandleOpenAccount.constructMessage(name, password, currency, balance, id);
        }

        return new byte[0];
    }

    public static boolean confirm(String name, String password, int currency, float balance, Scanner scanner){
        System.out.println(Constants.SEPARATOR);
        System.out.println(Constants.CONFIRM_SUMMARY);
        System.out.println();
        System.out.printf(Constants.CONFIRM_NAME, name);
        System.out.printf(Constants.CONFIRM_PASSWORD, password);
        System.out.printf(Constants.CONFIRM_CURRENCY, Constants.CURRENCY_STR[currency]);
        System.out.printf(Constants.CONFIRM_BALANCE, balance);
        System.out.print(Constants.CONFIRM_MSG);
        String confirm = scanner.nextLine();

        if (confirm.equals(Constants.CONFIRM_YES)){
            return true;
        } else {
            return false;
        }
    }

    public static byte[] constructMessage(String name, String password, int currency, float balance, int id)throws UnsupportedEncodingException{
        List message = new ArrayList();

        Utils.append(message, id);
        Utils.append(message, Constants.SERVICE_OPEN_ACCOUNT);
        Utils.appendMessage(message, name);
        Utils.appendMessage(message, password);
        Utils.appendMessage(message, currency);
        Utils.appendMessage(message, balance);

        return Utils.byteUnboxing(message);
    }

    public static void handleResponse(byte[] response){
        System.out.println(Constants.SEPARATOR);
        String statusStr = Utils.unmarshalString(response, 0, Constants.RESPONSE_TYPE_SIZE);
        int status = Integer.parseInt(statusStr);
        System.out.printf("DEBUG: Status = %d\n", status);
        switch(status){
            case Constants.NAK:
                System.out.printf("DEBUG: NAK\n", status);
                String errMsg = Utils.unmarshalMsgString(response, Constants.RESPONSE_TYPE_SIZE);
                System.out.printf(Constants.ERR_MSG, errMsg);
                break;
            case Constants.ACK:
                System.out.printf("DEBUG: ACK\n", status);
                int accountNumber = Utils.unmarshalMsgInteger(response, Constants.RESPONSE_TYPE_SIZE);
                System.out.println(Constants.SUCCESS_MSG);
                System.out.printf(Constants.SUCCESSFUL_OPEN_ACCOUNT, accountNumber);
                break;
            default:
                System.out.println(Constants.INVALID_RESPONSE);
        }
        System.out.println();
        System.out.println(Constants.SEPARATOR);
    }
}