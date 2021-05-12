#include <synchrosaurus_core/directory_scanner.hpp>

#include <synchrosaurus_core/file_mode_printer.hpp>
#include <synchrosaurus_core/sqlite.hpp>
#include <synchrosaurus_core/sqlite_result_codes.hpp>
#include <synchrosaurus_core/statx.hpp>

#include <iostream> // TODO Remove

namespace fs = std::filesystem;

DirectoryScanner::DirectoryScanner(std::shared_ptr<Sqlite3Connection> db_connection)
    : db_connection_(db_connection)
{
    //
}

DirectoryScanner::~DirectoryScanner()
{
    //
}

unsigned DirectoryScanner::scan(const fs::path &path, unsigned parent_id)
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

        std::string sql_command =
            "INSERT INTO t(id,name) VALUES(" +
            std::to_string(assign_id) +
            ",'" + entry.path().filename().string() + "');";
        Sqlite3Statement sql_stm(sql_command, db_connection_);
        auto eval_result = sql_stm.evaluate();
        if (eval_result != SqliteResultCode::DoneStep)
        {
            throw Sqlite3Error("Insertig row failed. Reason: " + getErrorString(eval_result));
        }

        if (entry.is_directory()) // TODO from statx
        {
            assign_id = scan(entry.path(), assign_id);
        }
    }

    return assign_id;
}
