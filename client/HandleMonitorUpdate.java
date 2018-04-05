package client;

import java.io.*;
import java.lang.*;
import java.util.*;
import java.time.format.DateTimeFormatter;
import java.time.LocalDateTime;

/**
 * The static classto handle "Monitor Update" request
 * - createMessage()
 * - construcMessage()
 * - handleResponse()
 */
class HandleMonitorUpdate{

    /**
     * Create message for monitoring updates by asking user input (duration)
     * @param scanner {@code Scanner} used to get user input
     * @param id {@code int} id of the request created
     * @return {@code byte[]}, the constructed message
     * @throws UnsupportedEncodingException
     * @since 1.9
     */
    public static byte[] createMessage(Scanner scanner, int id)throws UnsupportedEncodingException{
        System.out.println(Constants.SEPARATOR);
        System.out.println(Constants.MONITOR_MSG);

        // Enter Duration
        System.out.print(Constants.MONITOR_DURATION_MSG);
        String durationStr = scanner.nextLine();
        int duration = Integer.parseInt(durationStr);

        System.out.println();
        boolean confirm = HandleMonitorUpdate.confirm(duration, scanner);
        System.out.println();

        if (confirm){
            return HandleMonitorUpdate.constructMessage(duration, id);
        }

        return new byte[0];
    }

    /**
     * Getting user confirmation about the entered detail
     * @param duration {@code int} duration in milliseconds
     * @param scanner {@code Scanner}
     * @return {@code boolean} The confirmatiion
     * @since 1.9
     */
    public static boolean confirm(int duration, Scanner scanner){
        System.out.println(Constants.SEPARATOR);
        System.out.println(Constants.CONFIRM_SUMMARY);
        System.out.println();
        System.out.printf(Constants.CONFIRM_DURATION, duration);
        System.out.print(Constants.CONFIRM_MSG);
        String confirm = scanner.nextLine();

        if (confirm.equals(Constants.CONFIRM_YES)){
            return true;
        } else {
            return false;
        }
    }

    /**
     * Constructing message based on user input
     * @param duration {@code int} duration in milliseconds
     * @param id {@code int} Id of the request
     * @return {@code boolean} The confirmatiion
     * @throws UnsupportedEncodingException
     * @since 1.9
     */
    public static byte[] constructMessage(int duration, int id)throws UnsupportedEncodingException{
        List message = new ArrayList();

        Utils.append(message, id);
        Utils.append(message, Constants.SERVICE_MONITOR_UPDATE);
        Utils.appendMessage(message, duration);

        return Utils.byteUnboxing(message);
    }

    /**
     * Handling server response (unmarshall response and shows to user)
     * @param response {@code byte[]} byte response from server (without header length and response id)
     * @param debug {@code boolean} flag whether to print debug message or not
     * @return {@code void}
     * @since 1.9
     */
    public static int handleResponse(byte[] response, boolean debug){
        String statusStr = Utils.unmarshalString(response, 0, Constants.RESPONSE_TYPE_SIZE);
        int status = Integer.parseInt(statusStr);
        if (debug) System.out.printf("[DEBUG][HandleMonitorUpdate][Status = %d]\n", status);
        switch(status){
            case Constants.NAK:
                if (debug) System.out.println("[DEBUG][HandleMonitorUpdate][Unsuccessful response]");
                String errMsg = Utils.unmarshalMsgString(response, Constants.RESPONSE_TYPE_SIZE);
                System.out.printf(Constants.ERR_MSG, errMsg);
                return 0;
            case Constants.ACK:
                if (debug) System.out.println("[DEBUG][HandleMonitorUpdate][Successful response]");
                DateTimeFormatter dtf = DateTimeFormatter.ofPattern("yyyy/MM/dd HH:mm:ss");
                LocalDateTime now = LocalDateTime.now();

                int ptr = Constants.RESPONSE_TYPE_SIZE;
                int remainingDuration = Utils.unmarshalMsgInteger(response, ptr);

                ptr += Constants.INT_SIZE + Constants.INT_SIZE;
                String updateString = Constants.MONITORING_START_MSG;
                if (ptr < response.length){
                    updateString = Utils.unmarshalMsgString(response, ptr);
                }

                System.out.printf(Constants.NEW_UPDATE, dtf.format(now), updateString);
                return remainingDuration;
            default:
                System.out.println(Constants.INVALID_RESPONSE);
                return 0;
        }
    }
}