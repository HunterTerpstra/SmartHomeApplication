/**
 * @file logger.h
 * @author Mohammed Al Darwish
 * @brief logger class used to log what's happening in the backend
 * @date 2022-11-29
 * 
 */

#ifndef logger_h
#define logger_h

#include <iostream>
#include <string.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 
#include <time.h>
#include<ctime>
#include <vector>
#include "log_message.h"

#ifndef DEBUG
#define DEBUG 0
#endif

/**
 * @brief logger class used to log what's happening in the backend
 * a logger class that uses a sqlite3 database to store messages of type log_message
 * into a database that refreshes every day the program is run. It can write a message,
 * print the logs to console, or return the logs as a vector to be used elsewhere.
 */
class logger{

    private:
        sqlite3 *db; // holds the sqlite3 db object
        std::string appName; // holds the app name
        std::string getTime() const; // function to get system time
        std::string getDate() const; // function to get date

        // Prevent copying and assignment
        /**
         * @brief Construct a new logger object
         * 
         * @param other 
         */
        logger(const logger& other) { };
        static const logger* _instance; // holds the logger singleton instance
    
    public:
        /**
         * @brief Destroy the logger object
         * 
         */
        virtual ~logger();

        /**
         * @brief instanciator for singleton object
         * creates the logger object if it does not exist
         * 
         * @return const logger& the address of the singleton logger object
         */
        static const logger& instance();

        /**
         * @brief write message to log
         * 
         * @param callerName the name of the caller class
         * @param msg message to be logged
         * @return const logger& the address of the singleton logger object
         */
        const logger& write(std::string callerName,std::string msg) const; // method to write to db
        
        /**
         * @brief creates a vector of messages from the db
         * accesses the db and saves the messages into a vector
         * 
         * @return std::vector<log_message> a vector of log messages
         */
        std::vector<log_message> read_all() const; // method to read from db

        /**
         * @brief prints log to console
         * grabs logs from db and print s them to console
         * 
         * @return const logger& the address of the singleton logger object
         */
        const logger& printToConsole() const; // method to print event log to console
    
    protected:
        /**
         * @brief Construct a new logger object
         * creates the logger object along with the database
         */
        logger(); // Prevent instantiation

};

#endif
// end of file