#ifndef SYNCRHOSAURUS_CORE_DIRECTORY_SCANNER_HPP_INCLUDED
#define SYNCRHOSAURUS_CORE_DIRECTORY_SCANNER_HPP_INCLUDED

#include <filesystem>
#include <memory>

class Sqlite3Connection;

class DirectoryScanner
{
public:
    DirectoryScanner(std::shared_ptr<Sqlite3Connection> db_connection);
    ~DirectoryScanner();

    unsigned scan(const std::filesystem::path &path, unsigned parent_id);

private:
    std::shared_ptr<Sqlite3Connection> db_connection_;
};

#endif // SYNCRHOSAURUS_CORE_DIRECTORY_SCANNER_HPP_INCLUDED
