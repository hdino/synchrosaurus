#include <synchrosaurus_core/sqlite.hpp>

#include <sqlite3.h>

#include <iostream>

Sqlite3Error::Sqlite3Error(const std::string &what_arg)
    : std::runtime_error(what_arg)
{}

Sqlite3Connection::Sqlite3Connection(const std::string &filename)
{
    int result = sqlite3_open_v2(filename.c_str(),
                                 &db_handle_,
                                 SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
                                 NULL);
    if (result != SQLITE_OK)
    {
        sqlite3_close(db_handle_);
        throw Sqlite3Error("Cannot connect to database " + filename +
                           " - Reason: " + sqlite3_errstr(result));
    }
}

Sqlite3Connection::~Sqlite3Connection()
{
    /** TODO: Make sure that the following steps are done prior to closing
     *  - finalize all prepared statements (sqlite3_finalize)
     *      --> Ensured by Sqlite3Statement holding a shared_ptr to Sqlite3Connection
     *  - close all BLOB handles (sqlite3_blob_close)
     *  - finish all sqlite3_backup objects (sqlite3_backup_finish)
     **/

    int result = sqlite3_close_v2(db_handle_);
    if (result != SQLITE_OK)
    {
        std::cerr << "Closing the database connection failed."
                  << " Reason: " << sqlite3_errstr(result) << std::endl;
    }
}

Sqlite3Statement::Sqlite3Statement(std::string statement,
                                   std::shared_ptr<Sqlite3Connection> db_connection)
    : sql_statement_(statement)
    , db_connection_(db_connection)
{
    // From the SQLite documentation:
    // "Passing an nByte parameter that is the number of bytes in the input string
    //  including the nul-terminator" gives a small performance advantage.
    int result = sqlite3_prepare_v2(db_connection_->db_handle_,
                                    sql_statement_.c_str(), sql_statement_.size()+1,
                                    &statement_handle_, NULL);
    if (result != SQLITE_OK)
    {
        throw Sqlite3Error("Cannot compile statement \"" + sql_statement_ + "\" Reason: " +
                           sqlite3_errstr(result));
    }
}

Sqlite3Statement::~Sqlite3Statement()
{
    int result = sqlite3_finalize(statement_handle_);
    if (result != SQLITE_OK)
    {
        std::cerr << "Deleting compiled statement \"" << sql_statement_ << "\" failed."
                  << " Reason: " << sqlite3_errstr(result) << std::endl;
    }
}

void Sqlite3Statement::evaluate()
{
    int result = sqlite3_step(statement_handle_);
    //SQLITE_BUSY
}

void createFileTable(std::shared_ptr<Sqlite3Connection> db_connection)
{

}
