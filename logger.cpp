/*
Created by: Mohammed Al Darwish on November 3rd, 2022
*/

#include "logger.h"

// instantiator
const logger* logger::_instance = NULL;
const logger& logger::instance(){
    if (_instance == NULL)
    _instance = new logger();

    #if DEBUG
    printf("Logger successfully created.\n");
    #endif

    return *_instance;
}

// function to get system time
std::string logger::getTime() const{
    time_t curr_time;
    curr_time = time(NULL);
    char *t = strtok(ctime(&curr_time),"\n"); // removes the newline from the date/time
    return(t);
}

// function to get date
std::string logger::getDate() const{
    std::string time = getTime();
    std::string date;
    std::stringstream ss(time);
    std::string s;
    ss >> s; // skip day name
    for (int i = 0; i < 2; i++){
        ss >> s;
        date+=s;
        date+="-";
    }

    return date;
}

// constructor
logger::logger(){
    appName = "Bulbasoar";

    int rc; // remote call status
    char *zErrMsg = 0; // error code catcher
    std::string sql; // holds the sql query string

    std::string filename = getDate() + "Log.db";
    const char* DBFILE = filename.c_str(); // holds the db file name


    // open db
    rc = sqlite3_open(DBFILE, &db);
    
    #if DEBUG
    if( rc ) {
        std::cerr<<"Can't open database: %s\n", sqlite3_errmsg(db);
    } else {
        std::cerr<<"Opened database successfully\n";
    }
    #endif

    // create SQL statement
    sql = "create table if not exists "+ appName +" (timestamp varchar(255), caller varchar(255), message varchar(255))";

    // execute SQL statement
    rc = sqlite3_exec(db, sql.c_str(), nullptr, 0, &zErrMsg);
    
    // check for any errors
    #if DEBUG
    if( rc != SQLITE_OK ){
        std::cerr<<"SQL error: %s\n", zErrMsg;
        sqlite3_free(zErrMsg);
    } else {
        std::cout<<"Table created/opened successfully\n";
    }
    #endif

    sqlite3_close(db);
} // end of constructor

// destructor
logger::~logger(){
    sqlite3_close(db); // closes db connection
}

// method to log a msg
const logger& logger::write(std::string callerName,std::string msg) const{
    std::string appname = "Bulbasoar";
    
    // const char* DBFILE = "eventLog.db"; // holds the db file name
    std::string filename = getDate() + "Log.db";
    const char* DBFILE = filename.c_str(); // holds the db file name
    sqlite3 *db; // holds the sqlite3 db object
    int rc; // remote call status
    char *zErrMsg = 0; // error code catcher
    std::string sql; // holds the sql query string
    std::string time = getTime(); // gets and holds the system time 

    // open db
    rc = sqlite3_open(DBFILE, &db);
    
    #if DEBUG
    if( rc ) {
        std::cerr<<"Can't open database: %s\n", sqlite3_errmsg(db);
    } else {
        std::cerr<<"Opened database successfully\n";
    }
    #endif
    
    // create SQL statement
    sql = "insert into "+ appname +" values (\""+ time +"\", \""+ callerName +"\", \""+ msg +"\")";

    // execute SQL statement
    rc = sqlite3_exec(db, sql.c_str(), nullptr, 0, &zErrMsg);

    // check for any errors
    #if DEBUG
    if( rc != SQLITE_OK ){
        std::cerr<<"SQL error: %s\n", zErrMsg;
        sqlite3_free(zErrMsg);
    } else {
        std::cout<<"message written succesfully\n";
    }
    #endif

    sqlite3_close(db);
    return *this;
} // end of write

// method to read from db, returns a vector of logs from db
std::vector<log_message> logger::read_all() const{

    std::string appname = "Bulbasoar";
    // const char* DBFILE = "eventLog.db"; // holds the db file name
    std::string filename = getDate() + "Log.db";
    const char* DBFILE = filename.c_str(); // holds the db file name
    sqlite3 *db; // holds the sqlite3 db object
    int rc; // remote call status
    char *zErrMsg = 0; // error code catcher

    // open db
    rc = sqlite3_open(DBFILE, &db);
    
    #if DEBUG
    if( rc ) {
        std::cerr<<"Can't open database: %s\n", sqlite3_errmsg(db);
    } else {
        std::cerr<<"Opened database successfully\n";
    }
    #endif

    // variables needed to read from db
    sqlite3_stmt *stmnt; // holds a compiled binary version of sql statement
    std::string sql; // holds sql query
    std::vector<log_message> logmsg; // vector of log messages
    std::string clmn0; // holds the content of column 0, the timestamp
    std::string clmn1; // holds the content of column 1, the caller name
    std::string clmn2; // holds the content of column 2, the message

    // create SQL statement
    sql = "select * from "+appname;
    
    // prepare/compile sql statement 
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmnt, 0);    
    
    // check for any errors
    #if DEBUG
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
    }    
    #endif

    // execute/step the compiled statement
    rc = sqlite3_step(stmnt);
    
    // loop through table rows
    while (rc == SQLITE_ROW) {
        clmn0 = (const char*) (sqlite3_column_text(stmnt, 0)); // retrieve col0 content
        clmn1 = (const char*) (sqlite3_column_text(stmnt, 1)); // retrieve col1 content
        clmn2 = (const char*) (sqlite3_column_text(stmnt, 2)); // retrieve col1 content
        log_message log(clmn0, clmn1, clmn2); // create a log object with retrieved data
        logmsg.push_back(log); // push log object to vector

        rc = sqlite3_step(stmnt); // read next row if any
    }
    
    // release sql query statement and free any data allocations
    sqlite3_finalize(stmnt);

    // close db
    sqlite3_close(db);
    // return vector of log messages
    return logmsg;
} // end of read_all()

const logger& logger::printToConsole() const{
    
    // variables needed to open and interact with db
    std::string appname = "Bulbasoar";
    // const char* DBFILE = "eventLog.db"; // holds the db file name
    std::string filename = getDate() + "Log.db";
    const char* DBFILE = filename.c_str(); // holds the db file name
    sqlite3 *db; // holds the sqlite3 db object
    int rc; // remote call status
    char *zErrMsg = 0; // error code catcher
    std::string sql; // holds the sql query string

    // open db
    rc = sqlite3_open(DBFILE, &db);
    
    #if DEBUG
    if( rc ) {
        std::cerr<<"Can't open database: %s\n", sqlite3_errmsg(db);
    } else {
        std::cerr<<"Opened database successfully\n";
    }
    #endif

    // variables needed for grabbing from db
    sqlite3_stmt *stmnt; // holds a compiled binary version of sql statement
    std::vector<log_message> logmsg; // vector of log messages
    std::string clmn0; // holds the content of column 0, the timestamp
    std::string clmn1; // holds the content of column 1, the caller name
    std::string clmn2; // holds the content of column 2, the message

    // create SQL statement
    sql = "select * from "+appname;

    // prepare/compile sql statement 
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmnt, 0);    

    // check for any errors
    #if DEBUG
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
    }    
    #endif
    
    // execute/step the compiled statement
    rc = sqlite3_step(stmnt);
    
    // loop through table rows
    while (rc == SQLITE_ROW) {
        clmn0 = (const char*) (sqlite3_column_text(stmnt, 0)); // retrieve col0 content
        clmn1 = (const char*) (sqlite3_column_text(stmnt, 1)); // retrieve col1 content
        clmn2 = (const char*) (sqlite3_column_text(stmnt, 2)); // retrieve col1 content
        log_message log(clmn0, clmn1, clmn2); // create a log object with retrieved data
        logmsg.push_back(log); // push log object to vector

        rc = sqlite3_step(stmnt); // read next row if any
    }
    
    // release sql query statement and free any data allocations
    sqlite3_finalize(stmnt);

    std::vector<log_message> msg_log = logmsg; // vector of log message

    // check if table is empty
    #if DEBUG
    if (msg_log.size() == 0)
    {
        std::cout<<"Table for Application: \""<<appname<<"\" does not have any data"<<"\n";
    }
    #endif
    
    // print out table data with appropriate formatting
    for (int i = 0; i < msg_log.size(); i++){
        std::cout<<msg_log[i].get_timestamp()<<": ";
        std::cout<<msg_log[i].get_callerName()<<" - ";
        std::cout<<msg_log[i].get_message()<<"\n";
    }

    sqlite3_close(db);
    return *this;
} // end of print all

// end of file