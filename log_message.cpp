/*
Created by: Mohammed Al Darwish on November 3rd, 2022
*/

#include "log_message.h"

// constructor, creates a log message object, takes in timestamp and message as parameters
log_message::log_message(std::string new_timestamp, std::string new_callerName, std::string new_msg){
    msg = new_msg; // set msg
    callerName = new_callerName; // set caller name
    timestamp = new_timestamp; // set timestamp
}

// destructor, destroys log msg object
log_message::~log_message(){

}

// accessor methods

// method to retrieve a message
std::string log_message::get_message(){
    return msg;
}

// method to retrieve timestamp
std::string log_message::get_timestamp(){
    return timestamp;
}

// method to retrieve caller name
std::string log_message::get_callerName(){
    return callerName;
}

// end of file