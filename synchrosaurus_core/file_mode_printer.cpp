#include <synchrosaurus_core/file_mode_printer.hpp>

#include <cstring>

#include <sys/stat.h>

static char getFileTypeLetter(mode_t st_mode)
{
    if (S_ISREG(st_mode))
        return '-';
    else if (S_ISDIR(st_mode))
        return 'd';
    else if (S_ISLNK(st_mode))
        return 'l';
    else if (S_ISBLK(st_mode))
        return 'b';
    else if (S_ISCHR(st_mode))
        return 'c';
    else if (S_ISFIFO(st_mode))
        return 'p';
    else if (S_ISSOCK(st_mode))
        return 's';
#ifdef S_ISDOOR
    else if (S_ISDOOR(st_mode))
        return 'D';
#endif
    else
        return '?';
}

std::string getFileModeString(mode_t st_mode)
{
    static const char *permissions[] = {"---", "--x", "-w-", "-wx", "r--", "r-x", "rw-", "rwx"};

    std::string mode(10, 0); // TUUUGGGOOO (T=type, U=user, G=group, O=others)
    mode[0] = getFileTypeLetter(st_mode);

    strncpy(mode.data()+1, permissions[(st_mode >> 6) & 7], 3);
    strncpy(mode.data()+4, permissions[(st_mode >> 3) & 7], 3);
    strncpy(mode.data()+7, permissions[st_mode & 7], 3);

    if (st_mode & S_ISUID)
        mode[3] = (st_mode & S_IXUSR) ? 's' : 'S';
    if (st_mode & S_ISGID)
        mode[6] = (st_mode & S_IXGRP) ? 's' : 'l';
    if (st_mode & S_ISVTX)
        mode[9] = (st_mode & S_IXOTH) ? 't' : 'T';

    return mode;
}
