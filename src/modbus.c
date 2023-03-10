#include "modbus.h"

int ModbusReadCoilsReqInit(ModbusReadCoilsReq *pReq) {
    pReq->FunctionCode = MDBS_FC_READ_COILS;
    return MDBS_ERR_NONE;
}

int ModbusRtuReadCoilsReqPduEncode(ModbusReadCoilsReq *pReq, uint8_t *pBuf, uint8_t *pLen) {
    if (pReq->FunctionCode != MDBS_FC_READ_COILS) {
        return MDBS_ERR_REQ_NOT_INIT;
    }
    if (*pLen < 5) {
        return MDBS_ERR_BUF_OVERFLOW;
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
        return MDBS_ERR_REQ_NOT_INIT;
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
    pRsp->ExceptionCode = MDBS_EC_OK;
    return MDBS_ERR_NONE;
}


int ModbusRtuReadCoilsRspPduEncode(ModbusReadCoilsRsp *pRsp, uint8_t *pBuf, uint8_t *pLen) {
    if(pRsp->ExceptionCode != MDBS_FC_READ_COILS) {
        return MDBS_ERR_RSP_NOT_INIT;
    }
    if(*pLen < 2 + pRsp->ByteCount)
}
