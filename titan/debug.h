/*****************************************************/
/* this file is part of the tiTan / tiTanNIT Project */
/* and allowed only for use with this.               */
/*                                                   */
/* copyright by NIT                                  */
/*****************************************************/

#ifndef DEBUG_H
#define DEBUG_H

short debug_level = 10;

// mc debug_level = 50;
// panel debug_level = 60;

//debug
//first line shows greater/same debuglevel
//second line shows only same debuglevel
//#define debug(level, fmt, args...) if(debug_level >= level) { do { printf("[%s] " fmt, PROGNAME, ##args); } while (0); printf(", file=%s, func=%s, line=%d\n", __FILE__, __FUNCTION__, __LINE__); }
//#define debug(level, fmt, args...) if(debug_level == level) { do { printf("[%s] " fmt, PROGNAME, ##args); } while (0); printf(", file=%s, func=%s, line=%d\n", __FILE__, __FUNCTION__, __LINE__); }
void debugfunc(int level, char* file, const char* function, int line, char* msg, ...)
{
	if(debug_level == level)
	{
		va_list ap;
		va_start(ap, msg);
		printf("[%s] ", PROGNAME);
		vfprintf(stdout, msg, ap);
		va_end(ap);
		printf(", file=%s, func=%s, line=%d\n", file, function, line);
	}
}
#define debug(level, msg...) debugfunc(level, __FILE__, __FUNCTION__, __LINE__, msg);

//err
//#define err(fmt, args...) { do { fprintf(stderr, "[%s] error: " fmt, PROGNAME, ##args); } while (0); fprintf(stderr, ", file=%s, func=%s, line=%d\n", __FILE__, __FUNCTION__, __LINE__); }
void errfunc(char* file, const char* function, int line, char* msg, ...)
{
	va_list ap;
	va_start(ap, msg);
	fprintf(stderr, "[%s] error: ", PROGNAME);
	vfprintf(stderr, msg, ap);
	va_end(ap);
	fprintf(stderr, ", file=%s, func=%s, line=%d\n", file, function, line);
}
#define err(msg...) errfunc(__FILE__, __FUNCTION__, __LINE__, msg);

//perr
//#define perr(fmt, args...) { do { fprintf(stderr, "[%s] error: " fmt, PROGNAME, ##args); } while (0); fprintf(stderr, ", err=%m, file=%s, func=%s, line=%d\n", __FILE__, __FUNCTION__, __LINE__); }
void perrfunc(char* file, const char* function, int line, char* msg, ...)
{
	va_list ap;
	va_start(ap, msg);
	fprintf(stderr, "[%s] error: ", PROGNAME);
	vfprintf(stderr, msg, ap);
	va_end(ap);
	fprintf(stderr, ", err=%m, file=%s, func=%s, line=%d\n", file, function, line);
}
#define perr(msg...) perrfunc(__FILE__, __FUNCTION__, __LINE__, msg);

//filedebug
//#define filedebug(file, fmt, args...) { FILE* fd = fopen(file, "a"); if(fd != NULL) { do { fprintf(fd, "" fmt, ##args); } while (0); fprintf(fd, "\n"); }}
void filedebugfunc(char* file, char* msg, ...)
{
	FILE* fd = fopen(file, "a"); 
	if(fd != NULL)
	{
		va_list ap;
		va_start(ap, msg);
		vfprintf(fd, msg, ap);
		va_end(ap);
		fprintf(fd, "\n");
		fclose(fd);
	}
}
#define filedebug(file, msg...) filedebugfunc(file, msg);

#endif
