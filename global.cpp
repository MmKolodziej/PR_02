#include "global.h"

std::vector<pthread_mutex_t> paint_mutex;
std::vector<pthread_mutex_t> brush_mutex;
std::vector<pthread_cond_t>  paint_cond;
std::vector<pthread_cond_t>  brush_cond;

std::vector<int> paint_count;
std::vector<int> brush_usage_side;

int brush_count;