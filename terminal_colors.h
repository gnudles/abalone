#ifndef TERMINAL_COLORS_H
#define TERMINAL_COLORS_H

#if defined(__unix__) || defined(__linux) || (defined (__APPLE__) && defined (__MACH__))
#define __POSIX__
#endif
#if defined(WIN32) || (_WIN64 == 1) || (_WIN32 == 1)
#define __WINDOWS__
#endif


#ifdef __POSIX__
#define TERM_UNDERLINE         "\033[04m"
#define TERM_RED_COLOR         "\033[031m"
#define TERM_GREEN_COLOR       "\033[032m"
#define TERM_YELLOW_COLOR      "\033[033m"
#define TERM_DARK_BLUE_COLOR   "\033[034m"
#define TERM_PURPLE_COLOR      "\033[035m"
#define TERM_LIGHT_BLUE_COLOR  "\033[036m"
#define TERM_RESET             "\033[0m"
#else
#define TERM_UNDERLINE         ""
#define TERM_RED_COLOR         ""
#define TERM_GREEN_COLOR       ""
#define TERM_YELLOW_COLOR      ""
#define TERM_DARK_BLUE_COLOR   ""
#define TERM_PURPLE_COLOR      ""
#define TERM_LIGHT_BLUE_COLOR  ""
#define TERM_RESET             ""
#endif

#endif // TERMINAL_COLORS_H
