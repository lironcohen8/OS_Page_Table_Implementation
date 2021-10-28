#include "os.h"

void page_table_update(uint64_t pt, uint64_t vpn, uint64_t ppn)
{
    
	if (ppn == NO_MAPPING)
    {
        pages[0]
    }
    else
    {
        pt[vpn] = ppn;
    }
}

