#define keepAlivePinNr 10
int incomingByte = 0; // for incoming serial data

const int MAX_FRAME_LEN = 64;
char frameBuf[MAX_FRAME_LEN];
int detectOff = 0;
int frameLen = MAX_FRAME_LEN;
bool inFrame = false;
char printbuf[256];
const bool DEBUG = false;

uint16_t calcChecksum = 0;

struct PMS7003_framestruct {
    uint8_t  frameHeader[2];
    uint16_t frameLen = MAX_FRAME_LEN;
    uint16_t concPM1_0_CF1;
    uint16_t concPM2_5_CF1;
    uint16_t concPM10_0_CF1;
    uint16_t checksum;
} thisFrame;


void setup() {
    pinMode(keepAlivePinNr,OUTPUT);
    Serial.begin(57600);
    delay(1000);
}


bool pms7003_read() {
    digitalWrite(keepAlivePinNr,HIGH);
    Serial1.begin(9600);
    bool packetReceived = false;
    while (!packetReceived) {
        if (Serial1.available() > 32) {
            int drain = Serial1.available();           
            for (int i = drain; i > 0; i--) {
                Serial1.read();
            }
        }
        if (Serial1.available() > 0) {           
            incomingByte = Serial1.read();         
            if (!inFrame) {
                if (incomingByte == 0x42 && detectOff == 0) {
                    frameBuf[detectOff] = incomingByte;
                    thisFrame.frameHeader[0] = incomingByte;
                    calcChecksum = incomingByte; // Checksum init!
                    detectOff++;
                }
                else if (incomingByte == 0x4D && detectOff == 1) {
                    frameBuf[detectOff] = incomingByte;
                    thisFrame.frameHeader[1] = incomingByte;
                    calcChecksum += incomingByte;
                    inFrame = true;
                    detectOff++;
                }
            }
            else {
                frameBuf[detectOff] = incomingByte;
                calcChecksum += incomingByte;
                detectOff++;
                uint16_t val = frameBuf[detectOff-1]+(frameBuf[detectOff-2]<<8);
                switch (detectOff) {
                    case 4:
                        thisFrame.frameLen = val;
                        frameLen = val + detectOff;
                        break;
                    case 6:
                        thisFrame.concPM1_0_CF1 = val;
                        break;
                    case 8:
                        thisFrame.concPM2_5_CF1 = val;
                        break;
                    case 10:
                        thisFrame.concPM10_0_CF1 = val;
                        break;
                    case 32:
                        thisFrame.checksum = val;
                        calcChecksum -= ((val>>8)+(val&0xFF));
                        break;
                    default:
                        break;
                }
    
                if (detectOff >= frameLen) {          
                    if(calcChecksum == thisFrame.checksum){   
                    Serial.print(thisFrame.concPM1_0_CF1);
                    Serial.print(';');
                    Serial.print(thisFrame.concPM2_5_CF1);
                    Serial.print(';');
                    Serial.print(thisFrame.concPM10_0_CF1);
                    Serial.print(';');
                    Serial.print(millis());
                    Serial.print(";\n");                    
                    }
                    packetReceived = true;
                    detectOff = 0;
                    calcChecksum = 0;
                    inFrame = false;
                }
            }
        }
    }
    Serial1.end();
    return (calcChecksum == thisFrame.checksum);
}
void loop () {
    if (!pms7003_read()) {
        delay(1000);
    }
}
