#ifndef __MAIN_C
#define __MAIN_C


//here should be declarations for code that will run on both the CH32 and the ESP32
//THUS it can only be C-code, NO C++ here!!

struct Node_t {
    uint8_t Online;
    uint8_t ConfigChanged;
    uint8_t EVMeter;
    uint8_t EVAddress;
    uint8_t MinCurrent;     // 0.1A
    uint8_t Phases;
    uint32_t Timer;         // 1s
    uint32_t IntTimer;      // 1s
    uint16_t SolarTimer;    // 1s
    uint8_t Mode;
};

typedef enum { NOT_PRESENT, ALWAYS_OFF, SOLAR_OFF, ALWAYS_ON, AUTO } EnableC2_t;

typedef enum mb_datatype {
    MB_DATATYPE_INT32 = 0,
    MB_DATATYPE_FLOAT32 = 1,
    MB_DATATYPE_INT16 = 2,
    MB_DATATYPE_MAX,
} MBDataType;

struct EMstruct {
    uint8_t Desc[10];
    uint8_t Endianness;     // 0: low byte first, low word first, 1: low byte first, high word first, 2: high byte first, low word first, 3: high byte first, high word first
    uint8_t Function;       // 3: holding registers, 4: input registers
    MBDataType DataType;    // How data is represented on this Modbus meter
    uint16_t URegister;     // Single phase voltage (V)
    int8_t UDivisor;        // 10^x
    uint16_t IRegister;     // Single phase current (A)
    int8_t IDivisor;        // 10^x
    uint16_t PRegister;     // Total power (W) -- only used for EV/PV meter momentary power
    int8_t PDivisor;        // 10^x
    uint16_t ERegister;     // Total imported energy (kWh); equals total energy if meter doesnt support exported energy
    int8_t EDivisor;        // 10^x
    uint16_t ERegister_Exp; // Total exported energy (kWh)
    int8_t EDivisor_Exp;    // 10^x
};
#define SB2_WIFI_MODE 0 

#define NUM_ADC_SAMPLES 32


#define EM_SENSORBOX 1                                                          // Mains meter types
#define EM_PHOENIX_CONTACT 2
#define EM_FINDER_7E 3
#define EM_EASTRON3P 4
#define EM_EASTRON3P_INV 5
#define EM_ABB 6
#define EM_SOLAREDGE 7
#define EM_WAGO 8
#define EM_API 9
#define EM_EASTRON1P 10
#define EM_FINDER_7M 11
#define EM_SINOTIMER 12
#define EM_UNUSED_SLOT1 13
#define EM_UNUSED_SLOT2 14
#define EM_UNUSED_SLOT3 15
#define EM_UNUSED_SLOT4 16
#define EM_CUSTOM 17

#define STATE_A 0                                                               // A Vehicle not connected
#define STATE_B 1                                                               // B Vehicle connected / not ready to accept energy
#define STATE_C 2                                                               // C Vehicle connected / ready to accept energy / ventilation not required
#define STATE_D 3                                                               // D Vehicle connected / ready to accept energy / ventilation required (not implemented)
#define STATE_COMM_B 4                                                          // E State change request A->B (set by node)
#define STATE_COMM_B_OK 5                                                       // F State change A->B OK (set by master)
#define STATE_COMM_C 6                                                          // G State change request B->C (set by node)
#define STATE_COMM_C_OK 7                                                       // H State change B->C OK (set by master)
#define STATE_ACTSTART 8                                                        // I Activation mode in progress
#define STATE_B1 9                                                              // J Vehicle connected / EVSE not ready to deliver energy: no PWM signal
#define STATE_C1 10                                                             // K Vehicle charging / EVSE not ready to deliver energy: no PWM signal (temp state when stopping charge from EVSE)
//#if SMARTEVSE_VERSION == 3 TODO
#define STATE_MODEM_REQUEST 11                                                          // L Vehicle connected / requesting ISO15118 communication, 0% duty
#define STATE_MODEM_WAIT 12                                                          // M Vehicle connected / requesting ISO15118 communication, 5% duty
#define STATE_MODEM_DONE 13                                                // Modem communication succesful, SoCs extracted. Here, re-plug vehicle
#define STATE_MODEM_DENIED 14                                                // Modem access denied based on EVCCID, re-plug vehicle and try again
//#else
//#define STATE_E 11                  // disconnected pilot / powered down
//#define STATE_F 12                  // -12V Fault condition

#define NOSTATE 255

#define NR_EVSES 8
#define BROADCAST_ADR 0x09

extern uint8_t Force_Single_Phase_Charging(void);
extern uint16_t getItemValue(uint8_t nav);
extern EnableC2_t EnableC2;
extern uint8_t Pilot();
extern uint8_t pilot;
extern int16_t Isum;
extern uint8_t LoadBl;
extern uint8_t setItemValue(uint8_t nav, uint16_t val); //this somehow prevents undefined reference in CH32 compile
extern char IsCurrentAvailable(void);
extern void receiveNodeStatus(uint8_t *buf, uint8_t NodeNr);
extern void receiveNodeConfig(uint8_t *buf, uint8_t NodeNr);
extern void requestMeasurement(uint8_t Meter, uint8_t Address, uint16_t Register, uint8_t Count);
extern void requestCurrentMeasurement(uint8_t Meter, uint8_t Address);
extern void ModbusWriteSingleRequest(uint8_t address, uint16_t reg, uint16_t value);
extern void ModbusReadInputRequest(uint8_t address, uint8_t function, uint16_t reg, uint16_t quantity);
extern void ModbusWriteMultipleRequest(uint8_t address, uint16_t reg, uint16_t *values, uint8_t count);
extern void SetCurrent(uint16_t current);

extern uint8_t ModbusRx[256];
extern void ReadItemValueResponse(void);
extern void WriteItemValueResponse(void);
extern void WriteMultipleItemValueResponse(void);
extern void ModbusDecode(uint8_t * buf, uint8_t len);
extern void SetCPDuty(uint32_t DutyCycle);

extern uint8_t Initialized;
extern uint8_t PwrPanic;
extern uint8_t ModemPwr;
extern volatile uint8_t RxRdy1;
extern volatile uint8_t ModbusRxLen;

#endif
