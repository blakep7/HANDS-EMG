/*
 * syscalls.h
 *
 *  Created on: Mar 7, 2025
 *      Author: lilmo
 */

#ifndef SRC_SYSCALLS_H_
#define SRC_SYSCALLS_H_

#include <sys/stat.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

int _close(int file);
int _fstat(int file, struct stat *st);
int _isatty(int file);
int _lseek(int file, int ptr, int dir);
int _read(int file, char *ptr, int len);
int _write(int file, char *ptr, int len);

#ifdef __cplusplus
}
#endif

#endif /* SRC_SYSCALLS_H_ */
