#include <cstring>
#include <sys/stat.h>

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>

/* Define flags that are absent in linux */
#ifndef O_BINARY
#define O_BINARY 0
#define O_TEXT 0
#endif

#endif

#define MAX_EXT 64 /* Maximum length of file extension that we can handle */

inline bool file_exists(const char* filename) {
  struct stat buffer;   
  return (stat (filename, &buffer) == 0); 
}

inline const char* get_extension(const char* filename) {
   return strrchr(filename, '.');
}

inline long filelength(int fp) {
   long pos = lseek (fp, 0L, SEEK_CUR);
   long sz = lseek (fp, 0L, SEEK_END);
   lseek (fp, pos, SEEK_SET);
   return sz;
}

inline void make_uppercase(char* str) {
	while (*str != 0) {
		if (*str >= 'a' && *str <= 'z')
			*str -= 'a' - 'A';
		str++;
	}
}
