#include "os.h"

uint64_t* PTLevel1 = uint64_t[512];
uint64_t* PTLevel2 = uint64_t[512];
uint64_t* PTLevel3 = uint64_t[512];
uint64_t* PTLevel4 = uint64_t[512];
uint64_t* PTLevel5 = uint64_t[512];

void page_table_update(uint64_t pt, uint64_t vpn, uint64_t ppn)
{
    uint64_t* vpnParts = break_vpn_to_parts(vpn);
    uint64_t = vpn & 1;
	if (ppn == NO_MAPPING)
    {
        pages[0] = ppn;
    }
    else
    {
        pt[vpn] = ppn;
    }
}

/*uint64_t page_table_query(uint64_t pt, uint64_t vpn)
{
    
}*/

uint64_t* break_vpn_to_parts(uint64_t vpn)
{
    vpn = 0x180;
    uint64_t vpnPart1 = 0x1FF000000000000 & vpn; // 1st part
    uint64_t vpnPart2 = 0xFF8000000000 & vpn; // 2nd part
    uint64_t vpnPart3 = 0x7FC0000000 & vpn; // 3rd part
    uint64_t vpnPart4 = 0x3FE00000 & vpn; // 4th part
    uint64_t vpnPart5 = 0x1FF000 & vpn; // 5th part
    uint64_t* result = { vpnPart1, vpnPart2, vpnPart3, vpnPart4, vpnPart5 };
    return result;
}

/*break_vpn_to_parts(180);*/

