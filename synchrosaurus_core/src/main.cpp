#include <synchrosaurus_core/directory_scanner.hpp>
#include <synchrosaurus_core/sqlite.hpp>

#include <sqlite3.h>

#include <filesystem>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " [path]" << std::endl;
        return 0;
    }

    std::cout << "Using SQLite version " << sqlite3_libversion() << std::endl;

    std::shared_ptr<Sqlite3Connection> db_connection;

    try
    {
        //Sqlite3Connection db_connection("lalala.sqlite");
        db_connection = std::make_shared<Sqlite3Connection>("lalala.sqlite");
        createFileTable(db_connection);
    }
    catch (Sqlite3Error &err)
    {
        std::cout << err.what() << std::endl;
        return -1;
    }

    fs::path scanpath(argv[1]);
    scanpath = fs::absolute(scanpath);
    std::cout << "Scanning " << scanpath << std::endl;

    DirectoryScanner scanner(db_connection);

    unsigned count = scanner.scan(scanpath, 0);
    std::cout << count << std::endl;

    return 0;
}

