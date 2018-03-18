package client;

import java.io.*;
import java.lang.*;
import java.util.*;

class HandleTransferMoney{
    public static byte[] createMessage(Scanner scanner)throws UnsupportedEncodingException{
        System.out.println(Constants.SEPARATOR);
        System.out.println(Constants.TRANSFER_MSG);

        // Enter Name
        System.out.print(Constants.TRANSFER_NAME_MSG);
        String name = scanner.nextLine();
        while(name.length() == 0){
            System.out.println(Constants.ERR_NAME_INPUT);
            System.out.println();
            System.out.print(Constants.TRANSFER_NAME_MSG);
            name = scanner.nextLine();
        }

        // Enter Account Number
        // TODO: Handle error
        System.out.print(Constants.TRANSFER_ACC_NUM_MSG);
        String accNumStr = scanner.nextLine();
        int accNum = Integer.parseInt(accNumStr);

        // Enter Recipient Name
        System.out.print(Constants.TRANSFER_REC_NAME_MSG);
        String recipientName = scanner.nextLine();
        while(name.length() == 0){
            System.out.println(Constants.ERR_NAME_INPUT);
            System.out.println();
            System.out.print(Constants.TRANSFER_REC_NAME_MSG);
            recipientName = scanner.nextLine();
        }

        // Enter Recipient Account Number
        // TODO: Handle error
        System.out.print(Constants.TRANSFER_REC_ACC_NUM_MSG);
        String recAccNumStr = scanner.nextLine();
        int recAccNum = Integer.parseInt(accNumStr);

        // Enter Password
        System.out.print(Constants.TRANSFER_PASSWORD_MSG);
        String password = scanner.nextLine();
        while(password.length() == 0){
            System.out.println(Constants.ERR_PASSWORD_INPUT);
            System.out.println();
            System.out.print(Constants.TRANSFER_PASSWORD_MSG);
            password = scanner.nextLine();
        }

        // Enter Currency
        // TODO: Handle error
        System.out.println(Constants.TRANSFER_SELECT_CURRENCY_MSG);
        for (int i = 1; i < Constants.CURRENCY_STR.length; i++){
            System.out.printf("%d. %s\n", i, Constants.CURRENCY_STR[i]);
        }
        System.out.print(Constants.TRANSFER_CURRENCY_MSG);
        String currencyStr = scanner.nextLine();
        int currency = Integer.parseInt(currencyStr);

        // Enter Transfer Balance
        // TODO: Handle error
        System.out.print(Constants.TRANSFER_BALANCE_MSG);
        String balanceStr = scanner.nextLine();
        float balance = Float.parseFloat(balanceStr);

        System.out.println();
        boolean confirm = HandleTransferMoney.confirm(name, accNum, recipientName, recAccNum, password, currency, balance, scanner);
        System.out.println();

        if (confirm){
            return HandleTransferMoney.constructMessage(name, accNum, recipientName, recAccNum, password, currency, balance);
        }

        return new byte[0];
    }

    public static boolean confirm(String name, int accountNumber, String recipientName, int recipientAccountNumber, String password, int currency, float balance, Scanner scanner){
        System.out.print(Constants.SEPARATOR);
        System.out.println(Constants.CONFIRM_SUMMARY);
        System.out.println(Constants.SEPARATOR);
        System.out.printf(Constants.CONFIRM_NAME, name);
        System.out.printf(Constants.CONFIRM_ACCOUNT_NUMBER, accountNumber);
        System.out.printf(Constants.CONFIRM_REC_NAME, recipientName);
        System.out.printf(Constants.CONFIRM_REC_ACCOUNT_NUMBER, recipientAccountNumber);
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

    public static byte[] constructMessage(String name, int accountNumber, String recipientName, int recipientAccountNumber, String password, int currency, float balance)throws UnsupportedEncodingException{
        List message = new ArrayList();

        Utils.appendType(message, Constants.SERVICE_TRANSFER_MONEY);
        Utils.appendMessage(message, name);
        Utils.appendMessage(message, accountNumber);
        Utils.appendMessage(message, recipientName);
        Utils.appendMessage(message, recipientAccountNumber);
        Utils.appendMessage(message, password);
        Utils.appendMessage(message, currency);
        Utils.appendMessage(message, balance);

        return Utils.byteUnboxing(message);
    }

    public static void handleResponse(byte[] response){
        System.out.println(Constants.SEPARATOR);
        String statusStr = Utils.unmarshalString(response, 0, Constants.RESPONSE_TYPE_SIZE);
        int status = Integer.parseInt(statusStr);
        switch(status){
            case Constants.NAK:
                String errMsg = Utils.unmarshalMsgString(response, Constants.RESPONSE_TYPE_SIZE);
                System.out.printf(Constants.ERR_MSG, errMsg);
                break;
            case Constants.ACK:
                // TODO: response should contain currency
                float newBalance = Utils.unmarshalMsgFloat(response, Constants.RESPONSE_TYPE_SIZE);
                System.out.println(Constants.SUCCESS_MSG);
                System.out.printf(Constants.SUCCESSFUL_TRANSFER_ACCOUNT, 0, newBalance);
                break;
            default:
                System.out.println(Constants.INVALID_RESPONSE);
        }
        System.out.println();
        System.out.println(Constants.SEPARATOR);
    }
}