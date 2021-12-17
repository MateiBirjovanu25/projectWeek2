// Logging methods by levels
void log_error(char* format, ...);
void log_warning(char* format, ...);
void log_status(char* format, ...);
void log_debug(char* format, ...);

void logger_set_log_level(const int level);

/*
  Set target type
  Default is syslog
 */
void logger_reset_state(void);
int logger_set_log_file(const char* filename);
void logger_set_out_stdout();
void go_logger();