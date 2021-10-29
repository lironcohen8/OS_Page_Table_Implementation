#include <stdlib.h>
#include "os.h"
#include "os.c"

int NUM_OF_LEVELS = 5, shift, i;
uint64_t vpnPart, pte, newPpn, newVa, requiredPpn;
uint64_t* currentVa;

void page_table_update(uint64_t pt, uint64_t vpn, uint64_t ppn)
{
    currentVa = phys_to_virt(pt << 12);

	if (ppn == NO_MAPPING) // invalidating the mapping, if exists
    {
        for (i = 0; i < NUM_OF_LEVELS; i++)
        {
            shift = 36 - (9 * i); 
            vpnPart = (vpn >> shift) & 0x1FF; // masking to get relevant 9 bits
            pte = currentVa[vpnPart];
            if ((pte & 1) == 0) // reached invalid or not existing pte, no need to invalidate
            {
                return;
            }
            // if pte is valid, continue to the next level
            // pte = pte >> 12; TODO: understand if needed - nullifying the 12 least significant bits
            currentVa = phys_to_virt(pte << 12);
        }
        pte = pte & (~1); // mapping exists and should be invalid
    }
    else // creating a new mapping, if needed
    {
        for (int i = 0; i < NUM_OF_LEVELS; i++)
        {
            shift = 36 - (9 * i); 
            vpnPart = (vpn >> shift) & 0x1FF; // masking to get relevant 9 bits
            pte = currentVa[vpnPart];
            // TODO what if mapping doesn't exist, creating new mapping
            if ((pte & 1) == 0) // reached invalid pte, validate it and write the new ppn
            {
                newPpn = alloc_page_frame(); // TODO : need to understand how we continue and mapping only in the end
                currentVa[vpnPart] = (newPpn << 12) | 1; // nullifying the least 12 bits and validating the mapping
                i--; // making sure the loop continues from the mapping we have just created
            }
            // if pte is valid, continue to the next level
        }
        // when we finished the loop, the desired mapping was created
    }
}

uint64_t page_table_query(uint64_t pt, uint64_t vpn)
{
    currentVa = phys_to_virt(pt << 12);

    for (int i = 0; i < NUM_OF_LEVELS; i++)
    {
        shift = 36 - (9 * i); 
        vpnPart = (vpn >> shift) & 0x1FF; // masking to get relevant 9 bits
        pte = pages[*currentVa+vpnPart];
        if ((pte & 1) == 0) // reached invalid or not existing pte, no need to invalidate
        {
            return NO_MAPPING;
        }
        // if pte is valid, continue to the next level
        currentVa = phys_to_virt(pte << 12);
    }

    return pte << 12; // TODO: Check if that is what we need to return
}

void break_vpn_to_parts(uint64_t* vpnPartsAddr, uint64_t vpn)
{
    vpnPartsAddr[0] = 0x1FF000000000000 & vpn; // bits 48 to 56
    vpnPartsAddr[1] = 0xFF8000000000 & vpn; // bits 39 to 47
    vpnPartsAddr[2] = 0x7FC0000000 & vpn; // bits 30 to 38
    vpnPartsAddr[3] = 0x3FE00000 & vpn; // bits 21 to 29
    vpnPartsAddr[4] = 0x1FF000 & vpn; // bits 12 to 20
}

