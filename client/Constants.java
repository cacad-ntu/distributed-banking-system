package client;

class Constants{
    // Connection Constant
    public static final int ACK = 1;
    public static final String ACK_CHAR = "1";
    public static final int NAK = 0;
    public static final String NAK_CHAR = "0";
    public static final double FAILURE_RATE = 1.0;
    public static final int RESPONSE_TYPE_SIZE = 1;
    public static final int TIMEOUT = 1000;
    public static final String INVALID_RESPONSE = "Sorry we are having problem in the server";
    public static final String TIMEOUT_MSG = "Timeout!, resending request ...";

    public static final int NO_SEM_INVO = 0;
    public static final int AT_LEAST_ONE_SEM_INVO = 1;
    public static final int AT_MOST_ONE_SEM_INVO = 2;

    // Type Constant
    public static final int INT_SIZE = 4;
    public static final int FLOAT_SIZE = 4;

    // Service constant
    public static final int SERVICE_OPEN_ACCOUNT = 1;
    public static final int SERVICE_CLOSE_ACCOUNT = 2;
    public static final int SERVICE_DEPOSIT_MONEY = 3;
    public static final int SERVICE_WITHDRAW_MONEY = 4;
    public static final int SERVICE_MONITOR_UPDATE = 5;
    public static final int SERVICE_TRANSFER_MONEY = 6;
    public static final int SERVICE_CHANGE_PASSWORD = 7;
    public static final int SERVICE_EXIT = 8;


    // Currency Constant
    public static final int CUR_SGD = 1;
    public static final int CUR_USD = 2;

    public static final String[] CURRENCY_STR = {
        "not supported",
        "SGD",
        "USD"
    };


    // Main UI Constant
    public static final String ERR_MSG = "Error: %s\n";
    public static final String SUCCESS_MSG = "SUCCESS!";
    public static final String SEPARATOR = "================================================================================\n";
    public static final String WELCOME_MSG = "Welcome to CACAD banking system";
    public static final String EXIT_MSG = "Thank you for using CACAD banking system!";
    public static final String SELECTION_SVC_MSG = "Select the service you want to do:";
    public static final String OPEN_ACCOUNT_SVC_MSG = "1. Open new account.";
    public static final String CLOSE_ACCOUNT_SVC_MSG = "2. Close existing account.";
    public static final String DEPOSIT_MONEY_SVC_MSG = "3. Deposit money.";
    public static final String WITHDRAW_MONEY_SVC_MSG = "4. Withdraw money.";
    public static final String MONITOR_UPDATE_SVC_MSG = "5. Monitor Updates.";
    public static final String TRANSFER_MONEY_SVC_MSG = "6. Transfer money.";
    public static final String CHANGE_PASSWORD_SVC_MSG = "7. Change password.";
    public static final String EXIT_SVC_MSG = "8. Exit.";
    public static final String CHOICE_SVC_MSG = "Your choice: ";
    public static final String UNRECOGNIZE_SVC_MSG = "Sorry we cannot recognize your service choice!";

    // Open Account UI Constant
    public static final String OPEN_MSG = "Opening new account!";
    public static final String OPEN_NAME_MSG = "Enter your name: ";
    public static final String OPEN_PASSWORD_MSG = "Enter your password: ";
    public static final String OPEN_SELECT_CURRENCY_MSG = "Choose from the following currency: ";
    public static final String OPEN_CURRENCY_MSG = "Enter default currency: ";
    public static final String OPEN_BALANCE_MSG = "Enter starting balance (e.g. 1000.00): ";
    public static final String SUCCESSFUL_OPEN_ACCOUNT = "You have successfully open new account.\nYour account number: %d\n";

    // Close Account UI Constant
    public static final String CLOSE_MSG = "Closing existing account!";
    public static final String CLOSE_NAME_MSG = "Enter your name: ";
    public static final String CLOSE_ACC_NUM_MSG = "Enter your account number: ";
    public static final String CLOSE_PASSWORD_MSG = "Enter your password: ";
    public static final String SUCCESSFUL_CLOSE_ACCOUNT = "You have successfully close your account.";

    // Change Password UI Constant
    public static final String CHANGE_MSG = "Change Password!";
    public static final String CHANGE_NAME_MSG = "Enter your name: ";
    public static final String CHANGE_ACC_NUM_MSG = "Enter your account number: ";
    public static final String CHANGE_PASSWORD_MSG = "Enter your old password: ";
    public static final String CHANGE_NEW_PASSWORD_MSG = "Enter your new password: ";
    public static final String SUCCESSFUL_CHANGE_PASSWORD = "You have successfully change your password.";

    // Deposit Money UI Constant
    public static final String DEPOSIT_MSG = "Deposit money!";
    public static final String DEPOSIT_NAME_MSG = "Enter your name: ";
    public static final String DEPOSIT_ACC_NUM_MSG = "Enter your account number: ";
    public static final String DEPOSIT_PASSWORD_MSG = "Enter your password: ";
    public static final String DEPOSIT_SELECT_CURRENCY_MSG = "Choose from the following currency: ";
    public static final String DEPOSIT_CURRENCY_MSG = "Enter currency: ";
    public static final String DEPOSIT_BALANCE_MSG = "Enter deposit balance (e.g. 1000.00): ";
    public static final String SUCCESSFUL_DEPOSIT_MONEY = "You have successfully deposited money.\nYour new balance: %s %f\n";

    // Withdraw Money UI Constant
    public static final String WITHDRAW_MSG = "Withdraw money!";
    public static final String WITHDRAW_NAME_MSG = "Enter your name: ";
    public static final String WITHDRAW_ACC_NUM_MSG = "Enter your account number: ";
    public static final String WITHDRAW_PASSWORD_MSG = "Enter your password: ";
    public static final String WITHDRAW_SELECT_CURRENCY_MSG = "Choose from the following currency: ";
    public static final String WITHDRAW_CURRENCY_MSG = "Enter currency: ";
    public static final String WITHDRAW_BALANCE_MSG = "Enter withdraw balance (e.g. 1000.00): ";
    public static final String SUCCESSFUL_WITHDRAW_MONEY = "You have successfully withdrawn money.\nYour new balance: %s %f\n";

    // Transfer Money UI Constant
    public static final String TRANSFER_MSG = "Transfer money!";
    public static final String TRANSFER_NAME_MSG = "Enter your name: ";
    public static final String TRANSFER_ACC_NUM_MSG = "Enter your account number: ";
    public static final String TRANSFER_REC_NAME_MSG = "Enter recipient name: ";
    public static final String TRANSFER_REC_ACC_NUM_MSG = "Enter recipient account number: ";
    public static final String TRANSFER_PASSWORD_MSG = "Enter your password: ";
    public static final String TRANSFER_SELECT_CURRENCY_MSG = "Choose from the following currency: ";
    public static final String TRANSFER_CURRENCY_MSG = "Enter currency: ";
    public static final String TRANSFER_BALANCE_MSG = "Enter transfer balance (e.g. 1000.00): ";
    public static final String SUCCESSFUL_TRANSFER_MONEY = "You have successfully transferred money.\nYour new balance: %s %f\n";

    // Confirmation UI Constant
    public static final String CONFIRM_SUMMARY = "Summary:";
    public static final String CONFIRM_NAME = "Name: %s\n";
    public static final String CONFIRM_REC_NAME = "Name: %s\n";
    public static final String CONFIRM_PASSWORD = "Password: %s\n";
    public static final String CONFIRM_NEW_PASSWORD = "New Password: %s\n";
    public static final String CONFIRM_CURRENCY = "Currency: %s\n";
    public static final String CONFIRM_BALANCE = "Balance: %f\n";
    public static final String CONFIRM_ACCOUNT_NUMBER = "Account Number: %d\n";
    public static final String CONFIRM_REC_ACCOUNT_NUMBER = "Recipient Account Number: %d\n";
    public static final String CONFIRM_MSG = "Are you sure? (Y/N) ";
    public static final String CONFIRM_YES = "Y";
    public static final String CONFIRM_NO = "N";

    // Error UI Constant
    public static final String ERR_NAME_INPUT = "Invalid name, must be non-empty string!";
    public static final String ERR_PASSWORD_INPUT = "Invalid password, must be non-empty string!";
}