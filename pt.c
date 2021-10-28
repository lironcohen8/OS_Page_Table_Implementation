#include "os.h"

int NUM_OF_LEVELS = 5;
uint64_t* ptLevel1 = uint64_t[512];
uint64_t* ptLevel2 = uint64_t[512];
uint64_t* ptLevel3 = uint64_t[512];
uint64_t* ptLevel4 = uint64_t[512];
uint64_t* ptLevel5 = uint64_t[512];

uint64_t** pt = { ptLevel1, ptLevel2, ptLevel3, ptLevel4, ptLevel5 };

void page_table_update(uint64_t pt, uint64_t vpn, uint64_t ppn)
{
    uint64_t* vpnParts = break_vpn_to_parts(vpn);
    uint64_t = vpn & 1;
	if (ppn == NO_MAPPING) // invalidating the mapping, if exists
    {
        for (int i = 0; i < NUM_OF_LEVELS; i++)
        {
            uint64_t vpnParts[i] = 
        }
    }
    else // creating a new mapping, if needed
    {
        pt[vpn] = ppn;
    }
}

/*uint64_t page_table_query(uint64_t pt, uint64_t vpn)
{
    
}*/

uint64_t* break_vpn_to_parts(uint64_t vpn)
{
    uint64_t vpnPart1 = 0x1FF000000000000 & vpn; // bits 48 to 56
    uint64_t vpnPart2 = 0xFF8000000000 & vpn; // bits 39 to 47
    uint64_t vpnPart3 = 0x7FC0000000 & vpn; // bits 30 to 38
    uint64_t vpnPart4 = 0x3FE00000 & vpn; // bits 21 to 29
    uint64_t vpnPart5 = 0x1FF000 & vpn; // bits 12 to 20
    uint64_t* result = { vpnPart1, vpnPart2, vpnPart3, vpnPart4, vpnPart5 };
    return result;
}

/*break_vpn_to_parts(180);*/

