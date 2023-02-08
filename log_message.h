/**
 * @file log_message.h
 * @author Mohammed Al Darwish
 * @brief log_message class that holds a message's content
 * @date 2022-11-29
 * 
 * constructs an object that holds a message timestamp, caller name, and the message to be logged
 * which is then used to add the message to the database
 */

#ifndef log_message_h
#define log_message_h

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>

/**
 * @brief log_message class that holds a message's content
 * 
 * constructs an object that holds a message timestamp, caller name, and the message to be logged
 */
class log_message {
    private:
    std::string callerName; // holds caller name 
    std::string msg; // holds msg
    std::string timestamp; // holds timestamp

    public:
    /**
     * @brief Construct a new log message object
     * 
     * creates the message object with given parameters
     * 
     * @param new_timestamp the system time of the message
     * @param new_callerName the na,e of the one logging
     * @param new_msg the message to be logged
     */
    log_message(std::string new_timestamp, std::string new_callerName, std::string new_msg);
    
    /**
     * @brief Destroy the log message object
     * 
     */
    ~log_message();

    // accessor methods

    /**
     * @brief Get the message object
     * function to get the message
     * @return std::string message
     */
    std::string get_message();

    /**
     * @brief Get the timestamp object
     * function to get the timestamp 
     * @return std::string time stamp
     */
    std::string get_timestamp();

    /**
     * @brief Get the callerName object
     * function to get the caller's name
     * @return std::string caller's names
     */
    std::string get_callerName();

};

#endif
// end of file