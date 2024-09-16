// Logan Wilmoth
// ECE 4470
// Lab 6

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int sequenceNumber = 0;

int extractSequenceNumber(int *data, int dataSize) {
    int sequenceNum = 0;
    sequenceNum += (data[0] * 4);
    sequenceNum += (data[1] * 2);
    sequenceNum += data[2];

    return sequenceNum;
}

void simulateBitError(int *data, int dataSize) {
    for (int i = 0; i < dataSize; i++) {
        if (rand() < (RAND_MAX / 512)) {
            data[i] ^= 1;
        }
    }
}

void calculateCRC(int *data, int dataSize, int *crc) {
    int polynomial[13] = {1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1}; // Polynomial 0x180D
    int crcSize = 12;

    for (int i = 0; i < crcSize; i++) {
        crc[i] = 0;
    }

    for (int i = 0; i < dataSize; i++) {
        if (data[i] == 1) {
            for (int j = 0; j < crcSize; j++) {
                crc[j] ^= polynomial[j];
            }
        }
        int carry = crc[0];
        for (int j = 0; j < crcSize - 1; j++) {
            crc[j] = crc[j + 1];
        }
        crc[crcSize - 1] = data[i];
        if (carry == 1) {
            for (int j = 0; j < crcSize; j++) {
                crc[j] ^= polynomial[j];
            }
        }
    }
}

int generateRandomBytes() {
    return rand() % 2;
}

void transmission2(int *data, int dataSize, int *output, int *outputSize) {
    *outputSize = dataSize + 6; 
    int outputIndex = 0;

    output[outputIndex++] = 0;
    output[outputIndex++] = 0;
    output[outputIndex++] = 0;
    output[outputIndex++] = 0;

    for (int i = 0; i < dataSize; i++) {
        output[outputIndex++] = data[i];
    }

    output[outputIndex++] = 0;
    output[outputIndex++] = 0;
}

void transmission3(int *data, int dataSize, int *output, int *outputSize) {
    if (sequenceNumber % 8 == 0) {
        output[0] = 0;
        output[1] = 0;
        output[2] = 0;
    } else if (sequenceNumber % 8 == 1) {
        output[0] = 0;
        output[1] = 0;
        output[2] = 1;
    } else if (sequenceNumber % 8 == 2) {
        output[0] = 0;
        output[1] = 1;
        output[2] = 0;
    } else if (sequenceNumber % 8 == 3) {
        output[0] = 0;
        output[1] = 1;
        output[2] = 1;
    } else if (sequenceNumber % 8 == 4) {
        output[0] = 1;
        output[1] = 0;
        output[2] = 0;
    } else if (sequenceNumber % 8 == 5) {
        output[0] = 1;
        output[1] = 0;
        output[2] = 1;
    } else if (sequenceNumber % 8 == 6) {
        output[0] = 1;
        output[1] = 1;
        output[2] = 0;
    } else if (sequenceNumber % 8 == 7) {
        output[0] = 1;
        output[1] = 1;
        output[2] = 1;
    }
    
    int outputIndex = 3;

    output[outputIndex++] = 0; // Change for character or bit stuffing
    output[outputIndex++] = 1;
    output[outputIndex++] = 0;
    output[outputIndex++] = 1;
    
    // Character stuffing
    if (output[3] == 0) {
        for (int i = 0; i < dataSize; i++) {
            if ((data[i] == 0 && data[i + 1] == 1 && data[i + 2] == 1 && data[i + 3] == 1 && data[i + 4] == 0) || (data[i] == 0 && data[i + 1] == 1 && data[i + 2] == 1 && data[i + 3] == 1 && data[i + 4] == 1)) {
                output[outputIndex++] = 0;
                output[outputIndex++] = 1;
                output[outputIndex++] = 1;
                output[outputIndex++] = 1;
                output[outputIndex++] = 1;
            }
            output[outputIndex++] = data[i];
        }
    }
    // Bit stuffing
    else {
        int outputIndex = 0;
        for (int i = 0; i < dataSize; i++) {
            if (data[i] == 0 && data[i + 1] == 1 && data[i + 2] == 1) {
                output[outputIndex++] = data[i];
                output[outputIndex++] = data[i + 1];
                output[outputIndex++] = data[i + 2];
                output[outputIndex++] = 0;
                i += 2; // Not sure why it is 2 but the output works with 2 and not with 3 or 4
            } else {
                output[outputIndex++] = data[i];
            }
        }
    }
    output[outputIndex++] = 0;
    output[outputIndex++] = 1;
    output[outputIndex++] = 0;
    output[outputIndex++] = 1;

    *outputSize = outputIndex;
}


void transmission4(int *data, int dataSize, int *output, int *outputSize) {
    *outputSize = dataSize + 22;

    for (int i = 0; i < dataSize; ++i) {
        output[i + 5] = data[i];
    }

    int crcArray[12];
    calculateCRC(data, dataSize, crcArray);

    for (int i = 0; i < 12; ++i) {
        output[dataSize + 5 + i] = crcArray[i];
    }

    output[0] = 0;
    output[1] = 1;
    output[2] = 1;
    output[3] = 1;
    output[4] = 0;

    output[*outputSize - 5] = 0;
    output[*outputSize - 4] = 1;
    output[*outputSize - 3] = 1;
    output[*outputSize - 2] = 1;
    output[*outputSize - 1] = 0;
}

int receiver4(int *data, int dataSize, int *output, int *outputSize) {
    int receivedSequenceNumber = extractSequenceNumber(data + 5, dataSize - 10);

    if (!(data[0] == 0 && data[1] == 1 && data[2] == 1 && data[3] == 1 && data[4] == 0)) {
        printf("Host B: Packet %d dropped due to bit error\n", receivedSequenceNumber);
        return 1;
    }

    if (!(data[dataSize - 5] == 0 && data[dataSize - 4] == 1 && data[dataSize - 3] == 1 && data[dataSize - 2] == 1 && data[dataSize - 1] == 0)) {
        printf("Host B: Packet %d dropped due to bit error\n", receivedSequenceNumber);
        return 1;
    }

    if (receivedSequenceNumber != (sequenceNumber % 8)) {
        printf("Host B: Packet %d dropped due to bit error\n", receivedSequenceNumber);
        return 1;
    }

    // Simulate probability of packet loss
    if (rand() % 25 == 0) {
        printf("Host B: Packet %d Lost, timeout occured\n", receivedSequenceNumber);
        return 2;
    }

    if (rand() % 25 == 0) {
        printf("Host B: ACK %d Lost, timeout occured\n", receivedSequenceNumber);
        return 2;
    }

    *outputSize = dataSize - 10;
    for (int i = 5; i < dataSize - 10; ++i) {
        output[i - 5] = data[i];
    }

    int polynomial[13] = {1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1};
    int crcSize = 12;

    int receivedCRC[crcSize];
    for (int i = 0; i < crcSize; i++) {
        receivedCRC[i] = data[dataSize - 5 - crcSize + i];
    }

    int calculatedCRC[crcSize];
    calculateCRC(data + 5, dataSize - crcSize - 10, calculatedCRC);

    for (int i = 0; i < crcSize; i++) {
        if (receivedCRC[i] != calculatedCRC[i]) {
            printf("Host B: Packet %d dropped due to bit error\n", receivedSequenceNumber);
            return 1;
        }
    }

    *outputSize = dataSize - crcSize - 10;
    for (int i = 0; i < *outputSize; i++) {
        output[i] = data[i + 5];
    }
    
    return 0;
}



void receiver3(int *data, int dataSize, int *output, int *outputSize) {
    if (data[3] == 0) {
        int outputIndex = 0;
        for (int i = 7; i < dataSize - 4; i++) {
            if ((data[i] == 0 && data[i + 1] == 1 && data[i + 2] == 1 && data[i + 3] == 1 && data[i + 4] == 0) || (data[i] == 0 && data[i + 1] == 1 && data[i + 2] == 1 && data[i + 3] == 1 && data[i + 4] == 1)) {
                i += 5;
            }
            output[outputIndex++] = data[i];
        }
        *outputSize = outputIndex;  
    } else {
        int outputIndex = 0;
        for (int i = 7; i < dataSize - 4; i++) {
            if (data[i] == 0 && data[i + 1] == 1 && data[i + 2] == 1) {
                output[outputIndex++] = data[i];
                output[outputIndex++] = data[i + 1];
                output[outputIndex++] = data[i + 2];
                i += 4; // Also not sure why 4 works
            } 
            output[outputIndex++] = data[i];
        }
        *outputSize = outputIndex;
    }  
} 

void receiver2(int *data, int dataSize, int *output, int *outputSize) {
    *outputSize = dataSize - 6;
    for (int i = 0; i < dataSize - 6; i++) {
        output[i] = data[i + 4];
    }
}

int main() {
    srand(time(NULL));
    int data[64];
    int dataSize = 64;

    printf("Host A: Sending 20 packets from A to B\n");

    int windowStart = 0;
    int windowEnd = 2;
    int receiverWindowStart = 0;
    int receiverWindowEnd = 2;
    int buffer[3][150];
    bool retransmit = false;
    int success[3];

    while (sequenceNumber < 20) {

        int originalSequenceNumber = sequenceNumber;

        for (int i = windowStart; i <= windowEnd && i < 20; i++) {
            for (int i = 0; i < 64; i++) {
                data[i] = generateRandomBytes();
            }

            int level2Output[70]; 
            int level2OutputSize;
            transmission2(data, 64, level2Output, &level2OutputSize);

            int transmission3OutputSize;
            int transmission3Output[150];
            transmission3(level2Output, level2OutputSize, transmission3Output, &transmission3OutputSize);

            int transmission4Output[transmission3OutputSize + 22];
            int transmission4OutputSize;
            transmission4(transmission3Output, transmission3OutputSize, transmission4Output, &transmission4OutputSize);
            
            simulateBitError(transmission4Output, transmission4OutputSize);

            printf("Host A: Packet %d Sent\n", sequenceNumber % 8);

            int receiver4Output[transmission4OutputSize];
            int receiver4OutputSize;
            int result = receiver4(transmission4Output, transmission4OutputSize, receiver4Output, &receiver4OutputSize);
            if (result == 1 || result == 2) {
                retransmit = true;
            } else { 
                for (int i = 0; i < receiver4OutputSize; ++i) {
                    buffer[sequenceNumber % 3][i] = receiver4Output[i];
                }
                success[sequenceNumber % 3] = sequenceNumber;
                receiverWindowStart++;
                receiverWindowEnd++;
            }
        sequenceNumber++;
        }
        
        if (retransmit) {
            if (((success[0] == sequenceNumber - 3) && (success[1] == sequenceNumber - 2) && (success[2] == sequenceNumber -1)) || ((success[0] == 18) && (success[1] == 19))) {
                sequenceNumber -= 3;
                for (int i = 0; i < 3; i++) {
                    int receiver4OutputSize = sizeof(buffer[i]);
                    int receiver4Output[150];
                    for (int j = 0; j < 150; j++) {
                        receiver4Output[j] = buffer[i][j];
                    }

                    int receiver3Output[70];
                    int receiver3OutputSize;
                    receiver3(receiver4Output, receiver4OutputSize, receiver3Output, &receiver3OutputSize);

                    int receiver2Output[64];
                    int receiver2OutputSize;
                    receiver2(receiver3Output, receiver3OutputSize, receiver2Output, &receiver2OutputSize);

                    sequenceNumber++;
                }
                printf("Host A: ACK %d received\n", (sequenceNumber - 1) % 8);
                windowStart += 3;
                windowEnd += 3;
                printf("Host A: Press Enter to proceed with the next window\n");
                getchar();
            } else {
                sequenceNumber = originalSequenceNumber;
                retransmit = false;
                printf("Host A: ACK Not Received, Timeout Occured: Press Enter to retransmit\n");
                getchar();
            }
        } else {
            sequenceNumber -= 3;
            for (int i = 0; i < 3; i++) {
                int receiver4OutputSize = sizeof(buffer[i]);
                int receiver4Output[150];
                for (int j = 0; j < 150; j++) {
                    receiver4Output[j] = buffer[i][j];
                }

                int receiver3Output[70];
                int receiver3OutputSize;
                receiver3(receiver4Output, receiver4OutputSize, receiver3Output, &receiver3OutputSize);

                int receiver2Output[64];
                int receiver2OutputSize;
                receiver2(receiver3Output, receiver3OutputSize, receiver2Output, &receiver2OutputSize);

                sequenceNumber++;
            }
            printf("Host A: ACK %d received\n", (sequenceNumber - 1) % 8);
            windowStart += 3;
            windowEnd += 3;
            printf("Press Enter to proceed with the next window\n");
            getchar();
        }
    }
    printf("Transmission completed.\n");   
    return 0;  
}