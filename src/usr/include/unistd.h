#ifndef UNISTD_H
#define UNISTD_H
int getpid(void);
int open(const char *pathname, int flags);
int read(int fd, void *buf,int count);
int write(int fd, void *buf,int count);
int lseek(int fd, int offset, int whence);
int close(int fd);
#endif
