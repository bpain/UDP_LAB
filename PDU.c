#include "checksum.h"
#include <stdint.h> 
#include <arpa/inet.h> 
#include "string.h"
#include <stdio.h>
#include "PDU.h"

int make_PDU(uint8_t* PDU_BUFFER, uint32_t sequence_number, uint8_t flag, uint8_t* payload, int payload_len){
    memcpy(PDU_BUFFER, &sequence_number, 4); 
    PDU_BUFFER[4] = 0;
    PDU_BUFFER[5] = 0; 
    PDU_BUFFER[6] = flag; 
    memcpy(PDU_BUFFER + 7, payload, payload_len); 
    unsigned short checksum = in_cksum((unsigned short*)PDU_BUFFER, 4 + 2 + 1 + payload_len); 
    memcpy(PDU_BUFFER + 4, &checksum, 2); 
    return (4 + 2 + 1 + payload_len); 
}

void printPDU(uint8_t* aPDU, int pduLength){
    int a = in_cksum((unsigned short*)aPDU, pduLength); 
    if(a){
        printf("incorrect checksum %d\n", a); 
    }
    printf("PDU:\n"); 
    printf("sequence number: %d flag: %d payload: %s payload length: %d\n", (uint32_t)aPDU[0], aPDU[6], (aPDU+7), pduLength - 7); 
    return; 
}; 
