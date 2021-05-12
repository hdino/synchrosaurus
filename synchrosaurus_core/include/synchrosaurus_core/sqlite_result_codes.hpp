#ifndef SYNCRHOSAURUS_CORE_SQLITE_RESULT_CODES_HPP_INCLUDED
#define SYNCRHOSAURUS_CORE_SQLITE_RESULT_CODES_HPP_INCLUDED

#include <sqlite3.h>

#include <string>

enum class SqliteResultCode : int
{
    OK                              = SQLITE_OK,            /* Successful result */
    Error                           = SQLITE_ERROR,         /* Generic error */
    InternalLogicError              = SQLITE_INTERNAL,      /* Internal logic error in SQLite */
    PermissionDenied                = SQLITE_PERM,          /* Access permission denied */
    AbortRequested                  = SQLITE_ABORT,         /* Callback routine requested an abort */
    BusyDbFile                      = SQLITE_BUSY,          /* The database file is locked */
    LockedTable                     = SQLITE_LOCKED,        /* A table in the database is locked */
    NoMemory                        = SQLITE_NOMEM,         /* A malloc() failed */
    ReadOnly                        = SQLITE_READONLY,      /* Attempt to write a readonly database */
    Interrupt                       = SQLITE_INTERRUPT,     /* Operation terminated by sqlite3_interrupt()*/
    IoError                         = SQLITE_IOERR,         /* Some kind of disk I/O error occurred */
    CorruptDiskImage                = SQLITE_CORRUPT,       /* The database disk image is malformed */
    NotFoundOpcode                  = SQLITE_NOTFOUND,      /* Unknown opcode in sqlite3_file_control() */
    FullDb                          = SQLITE_FULL,          /* Insertion failed because database is full */
    CannotOpen                      = SQLITE_CANTOPEN,      /* Unable to open the database file */
    ProtocolLockError               = SQLITE_PROTOCOL,      /* Database lock protocol error */
    Empty                           = SQLITE_EMPTY,         /* Internal use only */
    SchemaChanged                   = SQLITE_SCHEMA,        /* The database schema changed */
    TooBig                          = SQLITE_TOOBIG,        /* String or BLOB exceeds size limit */
    ConstraintViolation             = SQLITE_CONSTRAINT,    /* Abort due to constraint violation */
    MismatchType                    = SQLITE_MISMATCH,      /* Data type mismatch */
    Misuse                          = SQLITE_MISUSE,        /* Library used incorrectly */
    NolfsFeaturesNotSupportedByOs   = SQLITE_NOLFS,         /* Uses OS features not supported on host */
    AuthorizationDenied             = SQLITE_AUTH,          /* Authorization denied */
    Format                          = SQLITE_FORMAT,        /* Not used */
    RangeViolated                   = SQLITE_RANGE,         /* 2nd parameter to sqlite3_bind out of range */
    NotADbFile                      = SQLITE_NOTADB,        /* File opened that is not a database file */
    Notice                          = SQLITE_NOTICE,        /* Notifications from sqlite3_log() */
    Warning                         = SQLITE_WARNING,       /* Warnings from sqlite3_log() */
    RowReady                        = SQLITE_ROW,           /* sqlite3_step() has another row ready */
    DoneStep                        = SQLITE_DONE           /* sqlite3_step() has finished executing */
};

inline std::string getErrorString(SqliteResultCode code)
{
    return sqlite3_errstr(static_cast<int>(code));
}

#endif // SYNCRHOSAURUS_CORE_SQLITE_RESULT_CODES_HPP_INCLUDED
