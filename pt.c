#include <stdlib.h>
#include "os.h"

int NUM_OF_LEVELS = 5;

uint64_t* pageTable[5];
uint64_t* vpnParts = (uint64_t*) calloc(5, sizeof(uint64_t));
void break_vpn_to_parts(uint64_t* vpnPartsAddr, uint64_t vpn);

for (int i = 0; i < NUM_OF_LEVELS; i++) // creating the page table with 5 levels
{
    pageTable[i] = (uint64_t*) calloc(512, sizeof(uint64_t));
}

void page_table_update(uint64_t pt, uint64_t vpn, uint64_t ppn)
{
    uint64_t baseVa = phys_to_virt(pt << 12);
    break_vpn_to_parts(vpnParts, vpn);
	if (ppn == NO_MAPPING) // invalidating the mapping, if exists
    {
        for (int i = 0; i < NUM_OF_LEVELS; i++)
        {
            uint64_t pte = pageTable[i][vpnParts[i]];
            if ((pte & 1) == 0) // reached invalid pte, no need to invalidate
            {
                return;
            }
            // if pte is valid, continue to the next level
        }
        uint64_t lastVpnPart = pageTable[NUM_OF_LEVELS - 1][vpnParts[NUM_OF_LEVELS - 1]];
        pageTable[NUM_OF_LEVELS - 1][vpnParts[NUM_OF_LEVELS - 1]] = lastVpnPart & (~1); // mapping exists and should be invalid
    }
    else // creating a new mapping, if needed
    {
        for (int i = 0; i < NUM_OF_LEVELS; i++)
        {
            uint64_t pte = pageTable[i][vpnParts[i]];
            if ((pte & 1) == 0) // reached invalid pte, creating new mapping
            {
                uint64_t ppn = alloc_page_frame(); // TODO : need to understand how we continue and mapping only in the end
                uint64_t va = phys_to_virt(ppn << 12);
                pageTable[i][vpnParts[i]] = (va & (~0xFFF)) | 1; // creating the mapping, nullifying the least 12 bits and validating the mapping
                i--; // making sure the loop continues from the mapping we have just created
            }
            // if pte is valid, continue to the next level
        }
        // when we finished the loop, the desired mapping was created
    }
}

uint64_t page_table_query(uint64_t pt, uint64_t vpn)
{
    break_vpn_to_parts(vpnParts, vpn);
    for (int i = 0; i < NUM_OF_LEVELS; i++)
    {
        uint64_t pte = pageTable[i][vpnParts[i]];
        if ((pte & 1) == 0) // reached invalid pte, mapping does not exist
        {
            return NO_MAPPING;
        }
        // if pte is valid, continue to the next level
    }
    uint64_t ppn = pageTable[NUM_OF_LEVELS - 1][vpnParts[NUM_OF_LEVELS - 1]];
    return ppn; // TODO: Check if that is what we need to return
}

void break_vpn_to_parts(uint64_t* vpnPartsAddr, uint64_t vpn)
{
    vpnPartsAddr[0] = 0x1FF000000000000 & vpn; // bits 48 to 56
    vpnPartsAddr[1] = 0xFF8000000000 & vpn; // bits 39 to 47
    vpnPartsAddr[2] = 0x7FC0000000 & vpn; // bits 30 to 38
    vpnPartsAddr[3] = 0x3FE00000 & vpn; // bits 21 to 29
    vpnPartsAddr[4] = 0x1FF000 & vpn; // bits 12 to 20
}

