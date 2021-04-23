#ifndef SYNCRHOSAURUS_CORE_FILE_MODE_PRINTER_HPP_INCLUDED
#define SYNCRHOSAURUS_CORE_FILE_MODE_PRINTER_HPP_INCLUDED

#include <string>

#include <sys/types.h>

/**
 * @brief getFileModeString Get the file permissions as output by ls -l
 * @param st_mode Output from stat command
 * @return String with the format TUUUGGGOOO (T=type, U=user, G=group, O=others)
 */
std::string getFileModeString(mode_t st_mode);

#endif // SYNCRHOSAURUS_CORE_FILE_MODE_PRINTER_HPP_INCLUDED
