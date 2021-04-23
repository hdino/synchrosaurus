#include <synchrosaurus_core/file_mode_printer.hpp>
#include <synchrosaurus_core/sqlite.hpp>
#include <synchrosaurus_core/statx.hpp>

#include <sqlite3.h>

#include <filesystem>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

unsigned scanDirectory(const fs::path &path, unsigned parent_id)
{
    unsigned assign_id = parent_id;

    std::error_code error_code;
    fs::directory_iterator directory_iterator(path, error_code);
    if (error_code)
    {
        switch (error_code.value()) // TODO Handle errors
        {
        case ENOENT:
            std::cout << "Directory does not exist: " << path << std::endl;
            break;
        case EACCES:
            std::cout << "No permission to open " << path << std::endl;
            break;
        default:
            std::cout << "Failed to open " << path << std::endl;
            std::cout << error_code.value() << " " << error_code.message() << " " << error_code.category().name() << std::endl;
        }
        return assign_id;
    }

    for (auto& entry : directory_iterator)
    {
        // TODO statx is probably not necessary here
        struct statx statx_result;
        int result = statx(0, entry.path().c_str(), AT_SYMLINK_NOFOLLOW, STATX_ALL, &statx_result);
        if (result != 0) // TODO Handle
        {
            std::cout << "statx for " << entry.path() << " returned error code " << result << std::endl;
            return assign_id;
        }

        ++assign_id;

        auto st_type = statx_result.stx_mode & S_IFMT;
        auto st_mode = statx_result.stx_mode & ~S_IFMT;

        std::cout << assign_id << " " << entry.path().filename()
                  << " parent=" << parent_id
                  << " UID=" << statx_result.stx_uid
                  << " GID=" << statx_result.stx_gid
                  << " size=" << statx_result.stx_size
                  << " type=" << st_type
                  << " mode=" << st_mode
                  << " --> " << getFileModeString(statx_result.stx_mode) << "\n";

        if (entry.is_directory()) // TODO from statx
        {
            assign_id = scanDirectory(entry.path(), assign_id);
        }
    }

    return assign_id;
}

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

    unsigned count = scanDirectory(scanpath, 0);
    std::cout << count << std::endl;

    return 0;
}

