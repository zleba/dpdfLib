#ifndef _EMSG_H_
#define _EMSG_H_

//#include <cstdio>
void OpenLog(char* lonam);
void CloseLog();

void emsg(int flag, const char *fmt, ...);
  /*
    Write message to stderr and log_file.
    flag values:
    0 log_file only,
    >0 both files, FATAL error, exit(flag),
    <0 both files.
  */

void require(bool cond, const char *fmt, ...);

#endif
