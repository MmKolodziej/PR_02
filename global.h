#ifndef parallel_2_global_h
#define parallel_2_global_h

#include <vector>
#include <pthread.h>

extern std::vector<pthread_mutex_t> paint_mutex;
extern std::vector<pthread_mutex_t> brush_mutex;
extern std::vector<pthread_cond_t>  paint_cond;
extern std::vector<pthread_cond_t>  brush_cond;

extern std::vector<int> paint_count;
extern std::vector<int> brush_usage_side;

extern int brush_count;

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define HELPER_TIMEOUT 20

#endif
