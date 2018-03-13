package client;

class Constants{
    // Connection Constant
    public static final int ACK = 1;
    public static final int NAK = 0;
    public static final int RESPONSE_TYPE_SIZE = 1;
    public static final String INVALID_RESPONSE = "Sorry we are having problem in the server";

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
        "0. not supported",
        "1. SGD",
        "2. USD"
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
    public static final String OPEN_MSG = "Opening new account:";
    public static final String OPEN_NAME_MSG = "Enter your name: ";
    public static final String OPEN_PASSWORD_MSG = "Enter your password: ";
    public static final String OPEN_SELECT_CURRENCY_MSG = "Choose from the following currency: ";
    public static final String OPEN_CURRENCY_MSG = "Enter your default currency: ";
    public static final String OPEN_BALANCE_MSG = "Enter your starting balance (e.g. 1000.00): ";
    public static final String SUCCESSFUL_OPEN_ACCOUNT = "Your account number: %d\n";

    // Confirmation UI Constant
    public static final String CONFIRM_SUMMARY = "Summary:";
    public static final String CONFIRM_NAME = "Name: %s\n";
    public static final String CONFIRM_PASSWORD = "Password: %s\n";
    public static final String CONFIRM_CURRENCY = "Currency: %s\n";
    public static final String CONFIRM_BALANCE = "Balance: %f\n";
    public static final String CONFIRM_ACCOUNT_NUMBER = "Account Number: %d\n";
    public static final String CONFIRM_MSG = "Are you sure? (Y/N) ";
    public static final String CONFIRM_YES = "Y";
    public static final String CONFIRM_NO = "N";

    // Error UI Constant
    public static final String ERR_NAME_INPUT = "Invalid name, must be non-empty string!";
    public static final String ERR_PASSWORD_INPUT = "Invalid password, must be non-empty string!";
}