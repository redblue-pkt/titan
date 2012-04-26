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

//first line shows greater/same debuglevel
//second line shows only same debuglevel
//#define debug(level, fmt, args...) if(debug_level >= level) { do { printf("[%s] " fmt, PROGNAME, ##args); } while (0); printf(", file=%s, func=%s, line=%d\n", __FILE__, __FUNCTION__, __LINE__); }
#define debug(level, fmt, args...) if(debug_level == level) { do { printf("[%s] " fmt, PROGNAME, ##args); } while (0); printf(", file=%s, func=%s, line=%d\n", __FILE__, __FUNCTION__, __LINE__); }

#define err(fmt, args...) { do { fprintf(stderr, "[%s] error: " fmt, PROGNAME, ##args); } while (0); fprintf(stderr, ", file=%s, func=%s, line=%d\n", __FILE__, __FUNCTION__, __LINE__); }

#define perr(fmt, args...) { do { fprintf(stderr, "[%s] error: " fmt, PROGNAME, ##args); } while (0); fprintf(stderr, ", err=%m, file=%s, func=%s, line=%d\n", __FILE__, __FUNCTION__, __LINE__); }

#endif
