#include "modbus.h"

int ModbusExceptionRspInit(ModbusExceptionRsp *pRsp) {
    pRsp->FunctionCode = 0;
    pRsp->ExceptionCode = 0;
    return MDBS_ERR_NONE;
}

int ModbusRtuExceptionRspPduEncode(ModbusExceptionRsp *pRsp, uint8_t *pBuf, uint8_t *pLen) {
    if (*pLen < 2) {
        return MDBS_ERR_OVERFLOW;
    }
    pBuf[0] = pRsp->FunctionCode + 0x80;
    pBuf[1] = pRsp->ExceptionCode;
    *pLen = 2;
    return MDBS_ERR_NONE;
}

int ModbusRtuExceptionRspPduDecode(ModbusExceptionRsp *pRsp, const uint8_t *pBuf) {
    pRsp->FunctionCode = pBuf[0] - 0x80;
    pRsp->ExceptionCode = pBuf[1];
    return MDBS_ERR_NONE;
}

int ModbusReadCoilsReqInit(ModbusReadCoilsReq *pReq) {
    pReq->FunctionCode = MDBS_FC_READ_COILS;
    return MDBS_ERR_NONE;
}

int ModbusRtuReadCoilsReqPduEncode(ModbusReadCoilsReq *pReq, uint8_t *pBuf, uint8_t *pLen) {
    if (pReq->FunctionCode != MDBS_FC_READ_COILS) {
        return MDBS_ERR_NOT_INIT;
    }
    if (*pLen < 5) {
        return MDBS_ERR_OVERFLOW;
    }
    pBuf[0] = pReq->FunctionCode;
    pReq->StartingAddress--;
    pBuf[1] = ((uint8_t *) &pReq->StartingAddress)[1];
    pBuf[2] = ((uint8_t *) &pReq->StartingAddress)[0];
    pReq->StartingAddress++;
    pBuf[3] = ((uint8_t *) &pReq->QuantityOfCoils)[1];
    pBuf[4] = ((uint8_t *) &pReq->QuantityOfCoils)[0];
    *pLen = 5;
    return MDBS_ERR_NONE;
}

int ModbusRtuReadCoilsReqPduDecode(ModbusReadCoilsReq *pReq, const uint8_t *pBuf) {
    if (pReq->FunctionCode != MDBS_FC_READ_COILS) {
        return MDBS_ERR_NOT_INIT;
    }
    ((uint8_t *) &pReq->StartingAddress)[1] = pBuf[1];
    ((uint8_t *) &pReq->StartingAddress)[0] = pBuf[2];
    pReq->StartingAddress++;
    ((uint8_t *) &pReq->QuantityOfCoils)[1] = pBuf[3];
    ((uint8_t *) &pReq->QuantityOfCoils)[0] = pBuf[4];
    return MDBS_ERR_NONE;
}

int ModbusReadCoilsRspInit(ModbusReadCoilsRsp *pRsp) {
    pRsp->FunctionCode = MDBS_FC_READ_COILS;
    pRsp->ExceptionCode = MDBS_EC_NONE;
    return MDBS_ERR_NONE;
}

int ModbusRtuReadCoilsRspPduEncode(ModbusReadCoilsRsp *pRsp, uint8_t *pBuf, uint8_t *pLen) {
    int i;
    if (pRsp->FunctionCode != MDBS_FC_READ_COILS) {
        return MDBS_ERR_NOT_INIT;
    }
    if (*pLen < (2 + pRsp->ByteCount)) {
        return MDBS_ERR_OVERFLOW;
    }

    if (pRsp->ExceptionCode != MDBS_EC_NONE) {
        ModbusExceptionRsp eRsp;
        ModbusExceptionRspInit(&eRsp);
        eRsp.FunctionCode = pRsp->FunctionCode;
        eRsp.ExceptionCode = pRsp->ExceptionCode;
        return ModbusRtuExceptionRspPduEncode(&eRsp, pBuf, pLen);
    }

    pBuf[0] = pRsp->FunctionCode;
    pBuf[1] = pRsp->ByteCount;
    for (i = 0; i < pRsp->ByteCount; i++) {
        pBuf[i + 2] = pRsp->CoilStatus[i];
    }

    *pLen = 2 + pRsp->ByteCount;
    return MDBS_ERR_NONE;
}

int ModbusRtuReadCoilsRspPduDecode(ModbusReadCoilsRsp *pRsp, uint8_t *pBuf) {
    if (pRsp->FunctionCode != MDBS_FC_READ_COILS || pRsp->ExceptionCode != MDBS_EC_NONE) {
        return MDBS_ERR_NOT_INIT;
    }

    if (pBuf[0] == MDBS_FC_READ_COILS + 0x80) {
        ModbusExceptionRsp eRsp;
        ModbusExceptionRspInit(&eRsp);
        ModbusRtuExceptionRspPduDecode(&eRsp, pBuf);
        pRsp->ExceptionCode = eRsp.ExceptionCode;
        return MDBS_ERR_NONE;
    }

    pRsp->ByteCount = pBuf[1];
    pRsp->CoilStatus = &pBuf[2];

    return MDBS_ERR_NONE;
}
