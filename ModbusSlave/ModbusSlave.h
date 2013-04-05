/*  Modbus over serial line - RTU Slave for Arduino
 *  
 *  See Modbus_slave.cpp for more info and arduino sketch example code
 */ 

#ifndef MODBUS_SLAVE_h
#define MODBUS_SLAVE_h

/* Modbus RTU common parameters, the Master MUST use the same parameters */
enum {
        MB_SLAVE = 1,   /* modbus slave id */
};

/* Slave registers enumeration for referencing regs[] datamap array */
enum {        
        MB_ALARMS,
        MB_FUELSWITCHLEVEL,
        MB_P_RATIO_FILTER_STATE,
        MB_P_RATIO_STATE_REACTOR,
        MB_P_REACTORLEVEL,
        MB_T_BREDLEVEL,
        MB_T_TREDLEVEL,
        MB_LAMBDA_D,
        MB_LAMBDA_I,
        MB_LAMBDA_P,
        MB_LAMBDA_SETPOINT,
        MB_P_COMB,
        MB_P_FILTER,
        MB_P_Q_AIR_ENG,
        MB_P_Q_AIR_RCT,
        MB_P_Q_GAS_ENG,
        MB_P_REACTOR,
        MB_AUGER_STATE,
        MB_BLOWER_STATE,
        MB_ENGINE_STATE,
        MB_FLARE_STATE,
        MB_GRATE_STATE,
        MB_T_BRED,
        MB_T_COMB,
        MB_T_DRYING_GAS_OUT,
        MB_T_ENG_COOLANT,
        MB_T_LOW_FUEL,
        MB_T_PYRO_IN,
        MB_T_PYRO_OUT,
        MB_T_REACTOR_GAS_OUT,
        MB_T_TRED,
        MB_GRATE_VAL,
        MB_LAMBDA_IN,
        MB_LAMBDA_OUT,
        MB_P_RATIO_FILTER,
        MB_P_RATIO_REACTOR,
        MB_CONFIG1,
        MB_CONFIG2,
        MB_CONFIG3,
        MB_CONFIG4,
        MB_CONFIG5,
        MB_CONFIG6,
        MB_CONFIG7,
        MB_CONFIG8,
        MB_CONFIG9,
        MB_CONFIG10,
        MB_CONFIG11,
        MB_CONFIG12,
        MB_CONFIG13,
        MB_CONFIG14,
        MB_CONFIG15,
        MB_CONFIG16,
        MB_CONFIG17,
        MB_CONFIG18,
        MB_CONFIG19,
        MB_CONFIG20,
        MB_CONFIG21,
        MB_CONFIG22,
        MB_CONFIG23, 
        MB_REGS     /* total number of registers on slave */
};

/* 
 *This is the slave's modbus holding register data map. Use this array
 *to get or set data from/to the holding registers. This array has size
 *MB_REGS.
 */
extern int regs[];

/* 
 *When the master changes (a) register(s), use this struct to check
 *which one(s). Set written.num_regs back to zero after having checked.
 */
typedef struct lastwrite {  
        int start_addr;     
        int num_regs;
} lastwrite;
extern lastwrite written; 

/* 
 * init_mb_slave(baud, parity, tx_en_pin)
 *
 * sets the communication parameters for of the serial line.
 *
 * baud: baudrate in bps (typical values 9600, 19200... 115200)
 * parity: a single character sets the parity mode: 
 *         'n' for no parity; 'e' for even parity, 'o' for odd parity.
 * tx_en_pin: arduino pin number that controls transmision/reception
 *        of an external half-duplex device (e.g. a RS485 interface chip).
 *        0 or 1 disables this function (for a two-device network)
 *        >2 for point-to-multipoint topology (e.g. several arduinos)
 */
void init_mb_slave(long baud, char parity, char txenpin);

/*
 * start_mb_slave(slave_id, holding_regs_array, number_of_regs)
 * 
 * checks if there is any valid request from the modbus master. If there is,
 * performs the action requested
 * 
 * slave: slave id (1 to 127)
 * regs: an array with the holding registers. They start at address 1 (master point of view)
 * regs_size: total number of holding registers, i.e. the size of the array regs.
 * returns: 0 if no request from master,
 * NO_REPLY (-1) if no reply is sent to the master
 * an exception code (1 to 4) in case of a modbus exceptions
 * the number of bytes sent as reply ( > 4) if OK.
 */
int start_mb_slave(unsigned char slave, int *regs, unsigned int regs_size);

#endif
