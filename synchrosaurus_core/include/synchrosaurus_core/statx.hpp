#ifndef SYNCRHOSAURUS_CORE_STATX_HPP_INCLUDED
#define SYNCRHOSAURUS_CORE_STATX_HPP_INCLUDED

// statx was added to glibc version 2.28 (and coreutils 8.31)
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

// If glibc version is < 2.28, but the Linux kernel version is >= 4.11
// one could fall back to syscall, cf. https://github.com/torvalds/linux/blob/master/samples/vfs/test-statx.c
//#include <syscall.h>
//ssize_t statx(int dfd, const char *filename, unsigned flags,
//	      unsigned int mask, struct statx *buffer)
//{
//	return syscall(__NR_statx, dfd, filename, flags, mask, buffer);
//}

#endif // SYNCRHOSAURUS_CORE_STATX_HPP_INCLUDED
