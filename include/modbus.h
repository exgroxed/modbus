#ifndef MODBUS_H
#define MODBUS_H

#include "stdint.h"

#define MDBS_VERSION_MAJOR 0
#define MDBS_VERSION_MINOR 0
#define MDBS_VERSION_REVISION 0

#define MDBS_FC_READ_COILS 0x01
#define MDBS_FC_READ_DISCRETE_INPUTS 0x02
#define MDBS_FC_READ_HOLDING_REGISTERS 0x03
#define MDBS_FC_READ_INPUT_REGISTERS 0x04
#define MDBS_FC_WRITE_SINGLE_COILS 0x05
#define MDBS_FC_WRITE_SINGLE_REGISTER 0x06
#define MDBS_FC_WRITE_MULTIPLE_COILS 0x1F
#define MDBS_FC_WRITE_MULTIPLE_REGISTERS 0x10

#define MDBS_EC_NONE 0
#define MDBS_EC_FUNCTION_NOT_SUPPORTED 1
#define MDBS_EC_INVALID_ADDRESS  2
#define MDBS_EC_INVALID_QUANTITY 3
#define MDBS_EC_INTERNAL_ERROR 4

#define MDBS_RTU_MAX_DATA_LENGTH 252

#define MDBS_ERR_NONE (0)
#define MDBS_ERR_NOT_INIT (-1)
#define MDBS_ERR_OVERFLOW (-2)
#define MDBS_ERR_RSP (-3)

typedef struct {
    uint8_t DeviceAddress;
    uint8_t *Pdu;
    uint16_t Crc16;
} ModbusRtuAdu;

int ModbusRtuAduInit(ModbusRtuAdu *pAdu);

int ModbusRtuAduEncode(ModbusRtuAdu *pAdu, uint8_t *pBuf, uint8_t *pLen);

int ModbusRtuAduDecode(ModbusRtuAdu *pAdu, uint8_t *pBuf);

typedef struct {
    uint8_t FunctionCode;
    uint8_t ExceptionCode;
} ModbusExceptionRsp;

int ModbusExceptionRspInit(ModbusExceptionRsp *pRsp);

int ModbusRtuExceptionRspPduEncode(ModbusExceptionRsp *pRsp, uint8_t *pBuf, uint8_t *pLen);

int ModbusRtuExceptionRspPduDecode(ModbusExceptionRsp *pRsp, const uint8_t *pBuf);

typedef struct {
    uint8_t FunctionCode;
    uint16_t StartingAddress;
    uint16_t QuantityOfCoils;
} ModbusReadCoilsReq;

int ModbusReadCoilsReqInit(ModbusReadCoilsReq *pReq);

int ModbusRtuReadCoilsReqPduEncode(ModbusReadCoilsReq *pReq, uint8_t *pBuf, uint8_t *pLen);

int ModbusRtuReadCoilsReqPduDecode(ModbusReadCoilsReq *pReq, const uint8_t *pBuf);

typedef struct {
    uint8_t FunctionCode;
    uint8_t ExceptionCode;
    uint8_t ByteCount;
    uint8_t *CoilStatus;
} ModbusReadCoilsRsp;

int ModbusReadCoilsRspInit(ModbusReadCoilsRsp *pRsp);

int ModbusRtuReadCoilsRspPduEncode(ModbusReadCoilsRsp *pRsp, uint8_t *pBuf, uint8_t *pLen);

int ModbusRtuReadCoilsRspPduDecode(ModbusReadCoilsRsp *pRsp, uint8_t *pBuf);

#endif // MODBUS_H
