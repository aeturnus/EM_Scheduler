//
// Created by Brandon on 7/21/2015.
//

#ifndef EM_SCHEDULER_XLSOUTPUT_H
#define EM_SCHEDULER_XLSOUTPUT_H

#include "Scheduler.h"
#include <xlslib.h>
#include <xlslib/common.h>

using namespace std;
using namespace xlslib_core;

void scheduleToXLS(Scheduler& schedule, workbook& wb);
#endif //EM_SCHEDULER_XLSOUTPUT_H

