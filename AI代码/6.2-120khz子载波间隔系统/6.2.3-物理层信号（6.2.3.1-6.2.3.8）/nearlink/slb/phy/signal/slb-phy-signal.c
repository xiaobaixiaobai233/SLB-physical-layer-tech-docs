/**
 * @file slb-phy-signal.c
 * airMode = slb
 */
#include "slb-phy-signal.h"

int slbInitPhySignalModule(void)
{
    int status = slbInitGoldSequenceTables();
    if (status != 0) {
        return status;
    }
    status = slbInitFtsLookupTables();
    if (status != 0) {
        return status;
    }
    status = slbInitStsLookupTables();
    return status;
}
