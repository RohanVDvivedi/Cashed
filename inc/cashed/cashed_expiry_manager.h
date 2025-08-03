#ifndef CASHED_EXPIRY_MANAGER_H
#define CASHED_EXPIRY_MANAGER_H

#include<cutlery/cashed_hashtable.h>

// expiry_manager's alarm_job function resides here

uint64_t expiry_manager_alarm_job_function(void* cashed_hashtable_vp);

#endif