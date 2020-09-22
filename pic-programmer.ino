#include <avr/pgmspace.h>

#define CLK_OUT 4
#define DAT_OUT 7
#define DAT_IN 3
#define VDD_OUT 2
#define VPP_OUT A1
#define PGM_OUT 5
#define STATUS_LED 13

#define VPP_ON 0
#define VPP_OFF 1

#define CMD_LOAD_CONFIG         0x00    // Load (write) to config memory
#define CMD_LOAD_PROGRAM_MEMORY 0x02    // Load to program memory
#define CMD_LOAD_DATA_MEMORY    0x03    // Load to data memory
#define CMD_INCREMENT_ADDRESS   0x06    // Increment the PC
#define CMD_READ_PROGRAM_MEMORY 0x04    // Read from program memory
#define CMD_READ_DATA_MEMORY    0x05    // Read from data memory
#define CMD_BEGIN_PROGRAM       0x08    // Begin programming with erase cycle
#define CMD_BULK_ERASE_PROGRAM  0x09    // Bulk erase program memory
#define CMD_BULK_ERASE_DATA     0x0B    // Bulk erase data memory

#define T_PROG_MS 4
#define T_DPROG_MS 6
#define T_ERA_MS 6

const uint16_t program_segment[] PROGMEM = {    
    0x0000, 0x2802, 0x3005, 0x00fe, 0x3001, 0x00ff, 0x3080, 0x0000, 0x0000, 0x20b1, 0x0000, 0x0000,
    0x1283, 0x1703, 0x00a1, 0x087f, 0x00a0, 0x3007, 0x00a2, 0x3001, 0x00a3, 0x1283, 0x1703, 0x0820,
    0x00a4, 0x0821, 0x00a5, 0x30ff, 0x07a0, 0x1c03, 0x03a1, 0x0825, 0x0424, 0x1903, 0x289a, 0x3004,
    0x0722, 0x00a4, 0x01a5, 0x0da5, 0x0823, 0x07a5, 0x0824, 0x00fe, 0x0825, 0x00ff, 0x3080, 0x0000,
    0x0000, 0x20b1, 0x0000, 0x0000, 0x1283, 0x1703, 0x00a7, 0x087f, 0x00a6, 0x0822, 0x00fe, 0x0823,
    0x00ff, 0x3080, 0x0000, 0x0000, 0x20b1, 0x0000, 0x0000, 0x1283, 0x1703, 0x00a9, 0x00a5, 0x087f,
    0x00a4, 0x00a8, 0x3002, 0x0722, 0x00a4, 0x01a5, 0x0da5, 0x0823, 0x07a5, 0x0824, 0x00fe, 0x0825,
    0x00ff, 0x3080, 0x0000, 0x0000, 0x20b1, 0x0000, 0x0000, 0x1283, 0x1703, 0x00a5, 0x00ab, 0x087f,
    0x00aa, 0x00a4, 0x1283, 0x1703, 0x0826, 0x00aa, 0x0827, 0x00ab, 0x30ff, 0x07a6, 0x1c03, 0x03a7,
    0x082b, 0x042a, 0x1903, 0x2893, 0x0828, 0x00fe, 0x0829, 0x00ff, 0x3080, 0x0000, 0x0000, 0x209e,
    0x0000, 0x0000, 0x1283, 0x1703, 0x00aa, 0x0824, 0x1283, 0x1303, 0x0084, 0x1383, 0x1283, 0x1703,
    0x1825, 0x1783, 0x082a, 0x1283, 0x1303, 0x0080, 0x1283, 0x1703, 0x0aa8, 0x1903, 0x0aa9, 0x0aa4,
    0x1903, 0x0aa5, 0x2862, 0x3006, 0x1283, 0x1703, 0x07a2, 0x1803, 0x0aa3, 0x2815, 0x0000, 0x0000,
    0x28d2, 0x0008, 0x3a00, 0x1903, 0x28a5, 0x3a80, 0x1903, 0x28ac, 0x3400, 0x087e, 0x0084, 0x1383,
    0x187f, 0x1783, 0x0800, 0x0008, 0x087f, 0x008a, 0x087e, 0x0082, 0x0008, 0x3a00, 0x1903, 0x28b9,
    0x3a80, 0x1903, 0x28c3, 0x01ff, 0x3400, 0x087e, 0x0084, 0x1383, 0x187f, 0x1783, 0x0800, 0x00ff,
    0x0a84, 0x0800, 0x0008, 0x0000, 0x0000, 0x20ac, 0x00fd, 0x0ffe, 0x03ff, 0x0aff, 0x0000, 0x0000,
    0x20ac, 0x00fc, 0x087d, 0x00ff, 0x087c, 0x0008, 0x1683, 0x1303, 0x1006, 0x1283, 0x1703, 0x01ac,
    0x01ad, 0x3027, 0x1283, 0x1703, 0x022d, 0x1d03, 0x28e1, 0x3010, 0x022c, 0x1803, 0x28e9, 0x1283,
    0x1703, 0x0aac, 0x1903, 0x0aad, 0x28d9, 0x1283, 0x1303, 0x1406, 0x1283, 0x1703, 0x01ac, 0x01ad,
    0x3027, 0x1283, 0x1703, 0x022d, 0x1d03, 0x28f8, 0x3010, 0x022c, 0x1803, 0x2900, 0x1283, 0x1703,
    0x0aac, 0x1903, 0x0aad, 0x28f0, 0x1283, 0x1303, 0x1006, 0x28d5, 0x0008, 0x3401, 0x3400, 0x340d,
    0x3401, 0x342e, 0x3401, 0x3401, 0x3400, 0x3400 };


uint16_t config_word = 0x3fb8;

uint8_t data_segment[] = { };

void enterHVP() {

    digitalWrite(PGM_OUT, LOW);
    delay(1);
    digitalWrite(VPP_OUT, VPP_ON);
    delay(1);
    digitalWrite(VDD_OUT, HIGH);
    delay(1);
}

void exitHVP() {

    digitalWrite(PGM_OUT, LOW);
    digitalWrite(VPP_OUT, VPP_OFF);
    digitalWrite(VDD_OUT, LOW);
    digitalWrite(DAT_OUT, LOW);
    digitalWrite(CLK_OUT, LOW);
}


void sendCommand(uint8_t cmd) {

    for (int i = 0; i < 6; i++) {
        digitalWrite(CLK_OUT, HIGH);
        digitalWrite(DAT_OUT, (cmd >> i) & 0x1);
        delay(1);
        digitalWrite(CLK_OUT, LOW);
        delay(1);
    }
}

uint16_t receiveData() {

    uint16_t data = 0;
    for (int i = 0; i < 16; i++) {

        digitalWrite(CLK_OUT, HIGH);
        delay(1);
        int b = digitalRead(DAT_IN);
        data |= b << i;
        digitalWrite(CLK_OUT, LOW);
        delay(1);
    }

    return (data >> 1) & 0x3fff;
}

void sendData(uint16_t data) {

    data <<= 1;

    for (int i = 0; i < 16; i++) {
        digitalWrite(CLK_OUT, HIGH);
        digitalWrite(DAT_OUT, (data >> i) & 0x1);
        delay(1);
        digitalWrite(CLK_OUT, LOW);
        delay(1);
    }
}

void cmdIncrementPC() {
    sendCommand(CMD_INCREMENT_ADDRESS);
    delay(1);
}

void cmdBeginProgramming() {
    sendCommand(CMD_BEGIN_PROGRAM);
}

uint16_t cmdLoadConfiguration() {
    sendCommand(CMD_LOAD_CONFIG);
    delay(1);
    sendData(0);
}

uint16_t cmdReadProgramMemory() {
    sendCommand(CMD_READ_PROGRAM_MEMORY);
    delay(1);
    return receiveData();
}

uint8_t cmdReadDataMemory() {
    sendCommand(CMD_READ_DATA_MEMORY);
    delay(1);
    uint16_t data = receiveData();
    return data & 0xff;
}

void cmdLoadProgramMemory(uint16_t word) {
    sendCommand(CMD_LOAD_PROGRAM_MEMORY);
    delay(1);
    sendData(word);
    delay(1); // really necessary?
}

void cmdLoadDataMemory(uint8_t data) {
    sendCommand(CMD_LOAD_DATA_MEMORY);
    delay(1);
    sendData(data);
    delay(1); // really necessary?
}

void cmdBulkEraseProgramMemory() {
    sendCommand(CMD_BULK_ERASE_PROGRAM);
}

void cmdBulkEraseDataMemory() {
    sendCommand(CMD_BULK_ERASE_DATA);
}

void printWordBin(uint16_t word) {
    char str[] = "00 0000 0000 0000";
    if(word & (1 << 13)) str[0] = '1';
    if(word & (1 << 12)) str[1] = '1';

    if(word & (1 << 11)) str[3] = '1';
    if(word & (1 << 10)) str[4] = '1';
    if(word & (1 <<  9)) str[5] = '1';
    if(word & (1 <<  8)) str[6] = '1';

    if(word & (1 << 7)) str[8] = '1';
    if(word & (1 << 6)) str[9] = '1';
    if(word & (1 << 5)) str[10] = '1';
    if(word & (1 << 4)) str[11] = '1';

    if(word & (1 << 3)) str[13] = '1';
    if(word & (1 << 2)) str[14] = '1';
    if(word & (1 << 1)) str[15] = '1';
    if(word & (1 << 0)) str[16] = '1';
    Serial.print(str);
}

void printWordHex(uint16_t word) {
    const char hexmap[] = "0123456789abcdef";

    char str[] = "0000";
    str[3] = hexmap[word & 0xF];
    str[2] = hexmap[(word >> 4) & 0xF];
    str[1] = hexmap[(word >> 8) & 0xF];
    str[0] = hexmap[(word >> 12) & 0x3];

    Serial.print(str);
}

void printByteHex(uint16_t word) {
    const char hexmap[] = "0123456789abcdef";

    char str[] = "00";
    str[1] = hexmap[word & 0xF];
    str[0] = hexmap[(word >> 4) & 0xF];

    Serial.print(str);
}

uint16_t readDeviceID() {
    enterHVP();
    cmdLoadConfiguration();
    for(int i=0; i<6; i++) cmdIncrementPC();
    uint16_t devid = cmdReadProgramMemory();
    exitHVP();
    return devid;
}

uint16_t readConfigurationWord() {
    enterHVP();
    cmdLoadConfiguration();
    for(int i=0; i<7; i++) cmdIncrementPC();
    uint16_t conf = cmdReadProgramMemory();
    exitHVP();
    return conf;
}


void doProgram() {

    Serial.println("Bulk erasing program memory...");
    enterHVP();
    cmdLoadProgramMemory(0x3fff);
    cmdBulkEraseProgramMemory();
    delay(T_ERA_MS);
    exitHVP();

    Serial.println("Bulk erasing data memory...");
    enterHVP();
    cmdBulkEraseDataMemory();
    delay(T_ERA_MS);
    exitHVP();

    Serial.println("Writing configuration bits...");
    enterHVP();
    cmdLoadConfiguration();
    for(int i=0; i<7; i++) cmdIncrementPC();
    cmdLoadProgramMemory(config_word);
    cmdBeginProgramming();
    delay(T_PROG_MS);
    exitHVP();

    Serial.println("Writing program segment...");
    enterHVP();
    for(int i=0; i<sizeof(program_segment)/sizeof(program_segment[0]); i++) {
        printWordHex(pgm_read_word_near(program_segment +i));
        if(i%8 == 7) {
            Serial.println();
        } else {
            Serial.print(" ");
        }
        cmdLoadProgramMemory(pgm_read_word_near(program_segment +i));
        cmdBeginProgramming();
        delay(T_PROG_MS);
        cmdIncrementPC();
    }
    exitHVP();

    Serial.println("Writing data segment...");
    enterHVP();
    // only 8 LSB of address are used.
    for(int i=0; i<sizeof(data_segment)/sizeof(data_segment[0]); i++) {
        printByteHex(data_segment[i]);
        if(i%8 == 7) {
            Serial.println();
        } else {
            Serial.print(" ");
        }
        cmdLoadDataMemory(data_segment[i]);
        cmdBeginProgramming();
        delay(T_DPROG_MS);
        cmdIncrementPC();
    }
    exitHVP();

}


void readStuff() {

    Serial.println("Verification.");
    // read configuration
    uint16_t conf = readConfigurationWord();
    Serial.print("Configuration bits: ");
    printWordBin(conf);
    Serial.println();

    uint16_t devid = readDeviceID();
    Serial.print("Device ID:          ");
    printWordBin(devid);
    Serial.println();

    // read program
    Serial.println("Program:");
    enterHVP();
    const int nbytes = 256;
    for(int i=0; i<nbytes; i++) {
        uint16_t d = cmdReadProgramMemory();
        cmdIncrementPC();
        printWordHex(d);
        if(i%8 == 7) {
            Serial.println();
        } else {
            Serial.print(" ");
        }
    }
    exitHVP();

    // read data
    Serial.println("Data:");
    enterHVP();
    for(int i=0; i<128; i++) {
        uint8_t d = cmdReadDataMemory();
        cmdIncrementPC();
        printByteHex(d);
        if(i%8 == 7) {
            Serial.println();
        } else {
            Serial.print(" ");
        }
    }
    exitHVP();

}

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    digitalWrite(CLK_OUT, LOW);
    pinMode(CLK_OUT, OUTPUT);
    digitalWrite(DAT_OUT, LOW);
    pinMode(DAT_OUT, OUTPUT);
    pinMode(DAT_IN, INPUT);
    digitalWrite(VDD_OUT, LOW);
    pinMode(VDD_OUT, OUTPUT);
    digitalWrite(VPP_OUT, VPP_OFF);
    pinMode(VPP_OUT, OUTPUT);
    digitalWrite(PGM_OUT, LOW);
    pinMode(PGM_OUT, OUTPUT);
    digitalWrite(STATUS_LED, LOW);
    pinMode(STATUS_LED, OUTPUT);


    doProgram();
    readStuff();

    Serial.println("\nExit");
}

void loop() {}
