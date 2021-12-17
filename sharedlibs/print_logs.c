#include <syslog.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <stdarg.h>
#include <string.h>
#include "print_logs.h"

/*
 * Program name variable is provided by the libc
 */
extern const char* __progname;

/*
 * Logger internal sctructure
 */
struct logger_t 
{
    int max_log_level;
    int use_stdout;
    FILE* out_file;
    void (*logger_func) (const int level, const char*);
};

static struct logger_t log_global_set;

#define PROGRAM_NAME __progname
#define FUNCTION_PATH __func__ 

#define LOG_LEVEL_ERROR 0
#define LOG_LEVEL_WARNING 1
#define LOG_LEVEL_STATUS 2
#define LOG_LEVEL_DEBUG 3


static const char* LOG_LEVELS[] = 
{ 
                    "ERROR",
                    "WARNING",
                    "STATUS",
                    "DEBUG"    };


void print_to_syslog(const int level, const char* message);
void print_to_file(const int level, const char* message);

/*  Close remaining file descriptor and reset global params     */

void cleanup_internal()
{
    if (log_global_set.out_file) 
    {
        if (!log_global_set.use_stdout) {
            fclose(log_global_set.out_file);
        }

        log_global_set.use_stdout = 0;
        log_global_set.out_file = NULL;
    }
}

/*   Reset internal state and set syslog as default target  */ 
void logger_reset_state(void)
{
    log_global_set.max_log_level = 1;
    cleanup_internal();
    log_global_set.logger_func = print_to_syslog;
}

/*  Print to syslog */

void print_to_syslog(const int level, const char* message)
{
    syslog(LOG_INFO, "[%s] %s\n", LOG_LEVELS[level], message);
}

/*   Print to file which can be a regular text file     */

void print_to_file(const int level, const char* message)
{
    struct tm* current_tm;
    time_t time_now;

    time(&time_now);
    current_tm = localtime(&time_now);

    int res = fprintf(log_global_set.out_file,
            " from function: %s %s: %02i:%02i:%02i [%s] %s\n"
                , PROGRAM_NAME
                , __func__
                , current_tm->tm_hour
                , current_tm->tm_min
                , current_tm->tm_sec
                , LOG_LEVELS[level]
                , message );

    if (res == -1) 
    {
        print_to_syslog(LOG_LEVEL_ERROR, "Unable to write to log file!");
        return;
    }

    fflush(log_global_set.out_file);
}


void logger_set_log_level(const int level)
{
    log_global_set.max_log_level = level;
}


// Setting file as the output
int logger_set_log_file(const char* filename)
{
    cleanup_internal();

    log_global_set.out_file = fopen(filename, "a");

    if (log_global_set.out_file == NULL) {
        log_error("Failed to open file %s error %s", filename, strerror(errno));
        return -1;
    }

    log_global_set.logger_func = print_to_file;

    return 0;
}

/* Setting stdout as the output*/
void logger_set_out_stdout()
{
    cleanup_internal();

    log_global_set.use_stdout = 1;
    log_global_set.logger_func = print_to_file;
    log_global_set.out_file = stdout;
}

/*   Logging functions  */
void log_generic(const int level, const char* format, va_list args)
{
    char buffer[256];
    vsprintf(buffer, format, args);
    log_global_set.logger_func(level, buffer);
}

void log_error(char *format, ...)
{
    va_list args;
    va_start(args, format);
    log_generic(LOG_LEVEL_ERROR, format, args);
    va_end(args);
}

void log_warning(char *format, ...)
{
    if (log_global_set.max_log_level < 1) {
        return;
    }

    va_list args;
    va_start(args, format);
    log_generic(LOG_LEVEL_WARNING, format, args);
    va_end(args);
}

void log_status(char *format, ...)
{
    if (log_global_set.max_log_level < 1) {
        return;
    }

    va_list args;
    va_start(args, format);
    log_generic(LOG_LEVEL_STATUS, format, args);
    va_end(args);
}

void log_debug(char *format, ...)
{
    if (log_global_set.max_log_level <  2) {
        return;
    }

    va_list args;
    va_start(args, format);
    log_generic(LOG_LEVEL_DEBUG, format, args);
    va_end(args);
}

void go_logger()
{
}

int main()
{
    /*
    logger_reset_state();

    log_warning("This message goes to syslog");
    log_error("ERROR!");
    log_status("Status!");
    log_debug("DEBUG!");

    logger_set_out_stdout();

    log_status("Hello!");
    log_error("ERROR!");
    log_status("Status!");
    log_debug("DEBUG!");

    logger_set_log_file("logger1.txt");

    log_error("Logger in a file mode!");
    log_status("Status!");
    log_debug("DEBUG!");
    */

    return 0;
}