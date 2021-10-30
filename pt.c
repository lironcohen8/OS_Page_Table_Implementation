#include <stdlib.h>
#include "os.h"

int NUM_OF_LEVELS = 5, shift, i;
uint64_t vpnPart, pte, newPpn, newVa, requiredPpn;
uint64_t* currentTableNode;

void page_table_update(uint64_t pt, uint64_t vpn, uint64_t ppn)
{
    currentTableNode = phys_to_virt(pt << 12);

	if (ppn == NO_MAPPING) // invalidating the mapping, if exists
    {
        for (i = 0; i < NUM_OF_LEVELS - 1; i++)
        {
            shift = 36 - (9 * i); 
            vpnPart = (vpn >> shift) & 0x1FF; // masking to get relevant 9 bits
            if ((currentTableNode[vpnPart] & 1) == 0) // reached invalid pte, no need to invalidate
            {
                return;
            }
            // if pte is valid, continue to the next level, only invalidating the last mapping
            currentTableNode = phys_to_virt(currentTableNode[vpnPart] & ~0xFFF);
        }
        vpnPart = vpn & 0x1FF; // masking to get relevant 9 bits
        if ((currentTableNode[vpnPart] & 1) == 1) // last mapping is valid
        {
            currentTableNode[vpnPart] &= (~1); // invalidate the mapping
        }
    }
    else // creating a new mapping, if needed
    {
        for (i = 0; i < NUM_OF_LEVELS - 1; i++)
        {
            shift = 36 - (9 * i); 
            vpnPart = (vpn >> shift) & 0x1FF; // masking to get relevant 9 bits
            // TODO what if currentTableNode doesn't exist?
            if ((currentTableNode[vpnPart] & 1) == 0) // reached invalid pte, creating a new pte and validating it
            {
                newPpn = alloc_page_frame(); // mapping new intermidiate pte
                currentTableNode[vpnPart] = (newPpn << 12) | 1; // nullifying the least 12 bits and validating the mapping
            }
            // if pte is valid, continue to the next level
            currentTableNode = phys_to_virt(currentTableNode[vpnPart] & ~0xFFF);
        }
        // we get to the last node and we need to create the desired mapping
        vpnPart = vpn & 0x1FF; // masking to get relevant 9 bits
        currentTableNode[vpnPart] = (ppn << 12) | 1;
    }
}

uint64_t page_table_query(uint64_t pt, uint64_t vpn)
{
    currentTableNode = phys_to_virt(pt << 12);

    for (i = 0; i < NUM_OF_LEVELS - 1; i++)
    {
        shift = 36 - (9 * i); 
        vpnPart = (vpn >> shift) & 0x1FF; // masking to get relevant 9 bits
        if ((currentTableNode[vpnPart] & 1) == 0) // reached invalid or not existing pte, no need to invalidate
        {
            return NO_MAPPING;
        }
        // if pte is valid, continue to the next level
        currentTableNode = phys_to_virt(currentTableNode[vpnPart] & ~0xFFF);
    }
    vpnPart = vpn & 0x1FF; // masking to get relevant 9 bits
    if ((currentTableNode[vpnPart] & 1) == 0) // reached invalid or not existing pte, no need to invalidate
    {
        return NO_MAPPING;
    }
    return currentTableNode[vpnPart] >> 12; // TODO: Check if that is what we need to return
}

