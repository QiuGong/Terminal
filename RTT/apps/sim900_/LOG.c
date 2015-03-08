#include "LOG.h"
#include "cellInfo_handle.h"

void LOG(int id, Time t, char arg[])
{
    addLog(id, t, arg);
}

