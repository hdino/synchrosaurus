#ifndef SYNCRHOSAURUS_CORE_SQLITE_HPP_INCLUDED
#define SYNCRHOSAURUS_CORE_SQLITE_HPP_INCLUDED

#include <memory>
#include <stdexcept>
#include <string>

typedef struct sqlite3 sqlite3;
typedef struct sqlite3_stmt sqlite3_stmt;

class Sqlite3Error : public std::runtime_error
{
public:
    Sqlite3Error(const std::string &what_arg);
};

class Sqlite3Connection
{
    friend class Sqlite3Statement;
public:
    Sqlite3Connection(const std::string &filename);
    ~Sqlite3Connection();

private:
    sqlite3 *db_handle_;
};

class Sqlite3Statement
{
public:
    Sqlite3Statement(std::string statement,
                     std::shared_ptr<Sqlite3Connection> db_connection);
    ~Sqlite3Statement();

    void evaluate();

private:
    std::string sql_statement_;
    std::shared_ptr<Sqlite3Connection> db_connection_; // TODO Does this makes sense?
    sqlite3_stmt *statement_handle_;
};

void createFileTable(std::shared_ptr<Sqlite3Connection> db_connection);

#endif // SYNCRHOSAURUS_CORE_SQLITE_HPP_INCLUDED
