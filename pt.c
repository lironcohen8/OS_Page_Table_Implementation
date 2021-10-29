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
        for (i = 0; i < NUM_OF_LEVELS; i++)
        {
            shift = 36 - (9 * i); 
            vpnPart = (vpn >> shift) & 0x1FF; // masking to get relevant 9 bits
            pte = currentTableNode[vpnPart];
            if ((pte & 1) == 0) // reached invalid or not existing pte, no need to invalidate
            {
                return;
            }
            // if pte is valid, continue to the next level
            // pte = pte >> 12; TODO: understand if needed - nullifying the 12 least significant bits
            currentTableNode = phys_to_virt(pte << 12);
        }
        pte = pte & (~1); // mapping exists and should be invalid
    }
    else // creating a new mapping, if needed
    {
        for (int i = 0; i < NUM_OF_LEVELS; i++)
        {
            shift = 36 - (9 * i); 
            vpnPart = (vpn >> shift) & 0x1FF; // masking to get relevant 9 bits
            pte = currentTableNode[vpnPart];
            // TODO what if mapping doesn't exist? creating new mapping
            if ((pte & 1) == 0) // reached invalid pte, writing the new ppn and validating the pte
            {
                newPpn = alloc_page_frame(); // TODO : need to understand how we continue and mapping only in the end
                currentTableNode[vpnPart] = (newPpn << 12) | 1; // nullifying the least 12 bits and validating the mapping
            }
            else // if pte is valid, continue to the next level
            {
                currentTableNode = phys_to_virt(pte << 12);
            }
        }
        // when we finished the loop, the desired mapping was created
    }
}

uint64_t page_table_query(uint64_t pt, uint64_t vpn)
{
    currentTableNode = phys_to_virt(pt << 12);

    for (int i = 0; i < NUM_OF_LEVELS; i++)
    {
        shift = 36 - (9 * i); 
        vpnPart = (vpn >> shift) & 0x1FF; // masking to get relevant 9 bits
        pte = currentTableNode[vpnPart];
        if ((pte & 1) == 0) // reached invalid or not existing pte, no need to invalidate
        {
            return NO_MAPPING;
        }
        // if pte is valid, continue to the next level
        currentTableNode = phys_to_virt(pte << 12);
    }

    return pte << 12; // TODO: Check if that is what we need to return
}

