/**
 * wrap statfs() calls for UnityHub as workaround for bug
 * gcc -Wall -o wrap_statfs.so -fPIC -shared -ldl wrap_statfs.c
 */

#define _GNU_SOURCE

#include <dlfcn.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/vfs.h>
#include <sys/statvfs.h>


/**
 * multiply free space with resize_factor and deliver this fake value
 */
static const int resize_factor = 16;

/**
 * the main logic/calculation
 */
#define SET_BLOCKS(BLOCKS, FREE, AVAIL) \
  (__extension__ ({ \
    uint64_t added; \
    added = (FREE) * resize_factor; \
    (BLOCKS) += added; \
    (FREE) += added; \
    AVAIL += added; \
  }))


const char wrap_statfs_version[] = "wrap_statfs.c, version 1.0, www.tuxad.com";

#define MESSAGE_STATFS "FAKE: statfs()\n"
#define MESSAGE_FSTATFS "FAKE: fstatfs()\n"
#define MESSAGE_STATVFS "FAKE: statvfs()\n"
#define MESSAGE_FSTATVFS "FAKE: fstatvfs()\n"
#define MESSAGE_STATFS64 "FAKE: statfs64()\n"
#define MESSAGE_FSTATFS64 "FAKE: fs64tatfs()\n"
#define MESSAGE_STATVFS64 "FAKE: statvfs64()\n"
#define MESSAGE_FSTATVFS64 "FAKE: fs64tatvfs()\n"

static int (*actual_statfs)(const char *path, struct statfs *buf) = 0;
static int (*actual_fstatfs)(int fd, struct statfs *buf) = 0;
static int (*actual_statvfs)(const char *path, struct statvfs *buf) = 0;
static int (*actual_fstatvfs)(int fd, struct statvfs *buf) = 0;
static int (*actual_statfs64)(const char *__file, struct statfs64 *__buf) = 0;
static int (*actual_fstatfs64)(int __fildes, struct statfs64 *__buf) = 0;
static int (*actual_statvfs64)(const char *__restrict __file, struct statvfs64 *__buf) = 0;
static int (*actual_fstatvfs64)(int __fildes, struct statvfs64 *__buf) = 0;


/* 32/64 */

int statfs(const char *path, struct statfs *buf) {
  int result;
  write(2, MESSAGE_STATFS, (sizeof MESSAGE_STATFS) - 1);
  actual_statfs = dlsym(RTLD_NEXT, "statfs");
  result = actual_statfs(path, buf);
  if (result == 0) {
    SET_BLOCKS(buf->f_blocks, buf->f_bfree, buf->f_bavail);
  }
  return result;
}

int fstatfs(int fd, struct statfs *buf) {
  int result;
  write(2, MESSAGE_FSTATFS, (sizeof MESSAGE_FSTATFS) - 1);
  actual_fstatfs = dlsym(RTLD_NEXT, "fstatfs");
  result = actual_fstatfs(fd, buf);
  if (result == 0) {
    SET_BLOCKS(buf->f_blocks, buf->f_bfree, buf->f_bavail);
  }
  return result;
}

int statvfs(const char *path, struct statvfs *buf) {
  int result;
  write(2, MESSAGE_STATVFS, (sizeof MESSAGE_STATVFS) - 1);
  actual_statvfs = dlsym(RTLD_NEXT, "statvfs");
  result = actual_statvfs(path, buf);
  if (result == 0) {
    SET_BLOCKS(buf->f_blocks, buf->f_bfree, buf->f_bavail);
  }
  return result;
}

int fstatvfs(int fd, struct statvfs *buf) {
  int result;
  write(2, MESSAGE_FSTATVFS, (sizeof MESSAGE_FSTATVFS) - 1);
  actual_fstatvfs = dlsym(RTLD_NEXT, "fstatvfs");
  result = actual_fstatvfs(fd, buf);
  if (result == 0) {
    SET_BLOCKS(buf->f_blocks, buf->f_bfree, buf->f_bavail);
  }
  return result;
}


/* 64 */

int statfs64(const char *path, struct statfs64 *buf) {
  int result;
  write(2, MESSAGE_STATFS64, (sizeof MESSAGE_STATFS64) - 1);
  actual_statfs64 = dlsym(RTLD_NEXT, "statfs64");
  result = actual_statfs64(path, buf);
  if (result == 0) {
    SET_BLOCKS(buf->f_blocks, buf->f_bfree, buf->f_bavail);
  }
  return result;
}

int fstatfs64(int fd, struct statfs64 *buf) {
  int result;
  write(2, MESSAGE_FSTATFS64, (sizeof MESSAGE_FSTATFS64) - 1);
  actual_fstatfs64 = dlsym(RTLD_NEXT, "fstatfs64");
  result = actual_fstatfs64(fd, buf);
  if (result == 0) {
    SET_BLOCKS(buf->f_blocks, buf->f_bfree, buf->f_bavail);
  }
  return result;
}

int statvfs64(const char *path, struct statvfs64 *buf) {
  int result;
  write(2, MESSAGE_STATVFS64, (sizeof MESSAGE_STATVFS64) - 1);
  actual_statvfs64 = dlsym(RTLD_NEXT, "statvfs64");
  result = actual_statvfs64(path, buf);
  if (result == 0) {
    SET_BLOCKS(buf->f_blocks, buf->f_bfree, buf->f_bavail);
  }
  return result;
}

int fstatvfs64(int fd, struct statvfs64 *buf) {
  int result;
  write(2, MESSAGE_FSTATVFS64, (sizeof MESSAGE_FSTATVFS64) - 1);
  actual_fstatvfs64 = dlsym(RTLD_NEXT, "fstatvfs64");
  result = actual_fstatvfs64(fd, buf);
  if (result == 0) {
    SET_BLOCKS(buf->f_blocks, buf->f_bfree, buf->f_bavail);
  }
  return result;
}


/* use modeline modelines=1 in vimrc */
/* vim: set ft=c sts=2 sw=2 ts=2 ai et: */
