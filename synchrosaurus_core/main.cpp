#include <cstring>
#include <filesystem>
#include <iostream>
#include <string>

#include <synchrosaurus_core/statx.hpp>

namespace fs = std::filesystem;

static int filetypeletter(int mode)
{
    char    c;

    if (S_ISREG(mode))
        c = '-';
    else if (S_ISDIR(mode))
        c = 'd';
    else if (S_ISBLK(mode))
        c = 'b';
    else if (S_ISCHR(mode))
        c = 'c';
#ifdef S_ISFIFO
    else if (S_ISFIFO(mode))
        c = 'p';
#endif  /* S_ISFIFO */
#ifdef S_ISLNK
    else if (S_ISLNK(mode))
        c = 'l';
#endif  /* S_ISLNK */
#ifdef S_ISSOCK
    else if (S_ISSOCK(mode))
        c = 's';
#endif  /* S_ISSOCK */
#ifdef S_ISDOOR
    /* Solaris 2.6, etc. */
    else if (S_ISDOOR(mode))
        c = 'D';
#endif  /* S_ISDOOR */
    else
    {
        /* Unknown type -- possibly a regular file? */
        c = '?';
    }
    return(c);
}

/* Convert a mode field into "ls -l" type perms field. */
static char *lsperms(int mode)
{
    static const char *rwx[] = {"---", "--x", "-w-", "-wx",
    "r--", "r-x", "rw-", "rwx"};
    static char bits[11];

    bits[0] = filetypeletter(mode);
    strcpy(&bits[1], rwx[(mode >> 6)& 7]);
    strcpy(&bits[4], rwx[(mode >> 3)& 7]);
    strcpy(&bits[7], rwx[(mode & 7)]);
    if (mode & S_ISUID)
        bits[3] = (mode & S_IXUSR) ? 's' : 'S';
    if (mode & S_ISGID)
        bits[6] = (mode & S_IXGRP) ? 's' : 'l';
    if (mode & S_ISVTX)
        bits[9] = (mode & S_IXOTH) ? 't' : 'T';
    bits[10] = '\0';
    return(bits);
}

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
                  << " --> " << lsperms(statx_result.stx_mode) << "\n";

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

    fs::path scanpath(argv[1]);
    scanpath = fs::absolute(scanpath);
    std::cout << "Scanning " << scanpath << std::endl;

    unsigned count = scanDirectory(scanpath, 0);
    std::cout << count << std::endl;

    return 0;
}

