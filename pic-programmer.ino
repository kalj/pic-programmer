
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

uint16_t program[] = {0x280D,
                      0x1683,
                      0x1281,
                      0x1181,
                      0x1501,
                      0x1481,
                      0x1401,
                      0x1283,
                      0x0008,
                      0x1D0B,
                      0x2809,
                      0x110B,
                      0x0008,
                      0x2001,
                      0x3000,
                      0x1683,
                      0x0086,
                      0x1283,
                      0x1406,
                      0x2009,
                      /* 0x2009, */
                      /* 0x2009, */
                      /* 0x2009, */
                      /* 0x2009, */
                      /* 0x2009, */
                      /* 0x2009, */
                      /* 0x2009, */
                      /* 0x2009, */
                      /* 0x2009, */
                      0x1006,
                      0x2009,
                      /* 0x2009, */
                      /* 0x2009, */
                      /* 0x2009, */
                      /* 0x2009, */
                      /* 0x2009, */
                      /* 0x2009, */
                      /* 0x2009, */
                      /* 0x2009, */
                      /* 0x2009, */
                      0x2811};

uint16_t config_word = 0x3FB8;
/* uint16_t config_word = 0b11111110111000; */
/* uint16_t config_word = 0b11111110111001; */

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

    return (data >> 1) & 0x3FFF;
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

void cmdLoadProgramData(uint16_t word) {
    sendCommand(CMD_LOAD_PROGRAM_MEMORY);
    delay(1);
    sendData(word);
    delay(1); // really necessary?
}

void cmdBulkEraseProgramMemory() {
    sendCommand(CMD_BULK_ERASE_PROGRAM);
}

void printWord(uint16_t word) {
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

    Serial.println("Bulk Erasing...");
    enterHVP();
    cmdLoadProgramData(0x3FFF);
    cmdBulkEraseProgramMemory();
    delay(T_ERA_MS);
    exitHVP();


    Serial.println("Writing configuration bits...");
    enterHVP();
    cmdLoadConfiguration();
    for(int i=0; i<7; i++) cmdIncrementPC();
    cmdLoadProgramData(config_word);
    cmdBeginProgramming();
    delay(T_PROG_MS);
    exitHVP();

    Serial.println("Writing program...");
    enterHVP();

    for(int i=0; i<sizeof(program)/sizeof(program[0]); i++) {

        Serial.print("Programming ");
        Serial.print(i, HEX);
        Serial.print("\t");
        Serial.println(program[i],HEX);
        cmdLoadProgramData(program[i]);
        cmdBeginProgramming();
        delay(T_PROG_MS);
        cmdIncrementPC();
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

    Serial.println("Verification.");
    // read configuration
    uint16_t conf = readConfigurationWord();
    Serial.print("Configuration bits: ");
    printWord(conf);
    Serial.println();

    // read program
    Serial.println("Program:");
    enterHVP();
    for(int i=0; i<41; i++) {
        uint16_t d = cmdReadProgramMemory();
        cmdIncrementPC();
        Serial.print(i, HEX);
        Serial.print("\t");
        Serial.println(d,HEX);
    }
    exitHVP();

    /* uint16_t devid = readDeviceID(); */
    /* Serial.print("Device ID:          "); */
    /* printWord(devid); */
    /* Serial.println(); */

    Serial.println("\nExit");
}

void loop() {}
