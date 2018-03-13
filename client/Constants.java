package client;

class Constants{
    // Service constant
    public static final int SERVICE_OPEN_ACCOUNT = 1;
    public static final int SERVICE_CLOSE_ACCOUNT = 2;
    public static final int SERVICE_DEPOSIT_MONEY = 3;
    public static final int SERVICE_WITHDRAW_MONEY = 4;
    public static final int SERVICE_MONITOR_UPDATE = 5;
    public static final int SERVICE_TRANSFER_MONEY = 6;
    public static final int SERVICE_CHANGE_PASSWORD = 7;
    public static final int SERVICE_EXIT = 8;

    // UI Constant
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


}