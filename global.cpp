#include "global.h"

std::vector<pthread_mutex_t> paint_mutex;
std::vector<pthread_mutex_t> brush_mutex;
pthread_mutex_t wine_mutex;
std::vector<pthread_cond_t>  paint_cond;
std::vector<pthread_cond_t>  brush_cond;
pthread_cond_t wine_cond;


std::vector<int> paint_count;
std::vector<int> brush_usage_side;
int wine_count;

int brush_count;