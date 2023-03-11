#include "modbus.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>

int TestModbusReadCoilsReqInit(void) {
    printf("Test ModbusReadCoilsReqInit: ");

    ModbusReadCoilsReq req;
    ModbusReadCoilsReqInit(&req);
    if (req.FunctionCode != MDBS_FC_READ_COILS) {
        printf("Wrong function code!\r\n");
        return -1;
    }

    printf("OK\r\n");
    return 0;
}

int TestModbusRtuReadCoilsReqPduEncode(void) {
    printf("Test ModbusRtuReadCoilsReqPduEncode: ");

    ModbusReadCoilsReq req;
    ModbusReadCoilsReqInit(&req);

    uint8_t buf[256] = {0};
    uint8_t bufLen = 255;
    req.StartingAddress = 20;
    req.QuantityOfCoils = 37;
    int rc = ModbusRtuReadCoilsReqPduEncode(&req, buf, &bufLen);
    if (rc != 0) {
        printf("Error code: %d\r\n", rc);
    }
    if (bufLen != 5) {
        printf("Wrong buffer length!\r\n");
        return -1;
    }
    if (buf[0] != MDBS_FC_READ_COILS) {
        printf("Wrong function code!\r\n");
        return -1;
    }
    if (buf[1] != 0x00 || buf[2] != 0x13) {
        printf("Wrong starting address!\r\n");
        return -1;
    }
    if (buf[3] != 0x00 || buf[4] != 0x25) {
        printf("Wrong quantity of coils!\r\n");
        return -1;
    }

    printf("OK\r\n");
    return 0;
}

int TestModbusRtuReadCoilsReqPduDecode(void) {
    printf("Test ModbusRtuReadCoilsReqPduDecode: ");
    uint8_t input[5] = {0x01, 0x00, 0x13, 0x00, 0x25};
    ModbusReadCoilsReq req;
    ModbusReadCoilsReqInit(&req);
    int rc = ModbusRtuReadCoilsReqPduDecode(&req, input);
    if (rc != 0) {
        printf("Error code: %d\r\n", rc);
    }
    if (req.FunctionCode != MDBS_FC_READ_COILS) {
        printf("Wrong function code!\r\n");
        return -1;
    }
    if (req.StartingAddress != 20) {
        printf("Wrong starting address!\r\n");
        return -1;
    }
    if (req.QuantityOfCoils != 37) {
        printf("Wrong quantity of coils!\r\n");
        return -1;
    }
    printf("OK\r\n");
    return 0;
}

int TestModbusReadCoilsRspInit(void) {
    printf("Test ModbusReadCoilsRspInit: ");

    ModbusReadCoilsRsp rsp;
    ModbusReadCoilsRspInit(&rsp);
    if (rsp.FunctionCode != MDBS_FC_READ_COILS) {
        printf("Wrong function code!\r\n");
        return -1;
    }
    if (rsp.ExceptionCode != MDBS_EC_NONE) {
        printf("Wrong exception code!\r\n");
        return -1;
    }

    printf("OK\r\n");
    return 0;
}

int TestModbusRtuReadCoilsRspPduEncode(void) {
    printf("Test ModbusRtuReadCoilsRspPduEncode: ");

    ModbusReadCoilsRsp rsp;
    ModbusReadCoilsRspInit(&rsp);

    uint8_t buf[256] = {0};
    uint8_t bufLen = 255;
    rsp.ExceptionCode = MDBS_EC_FUNCTION_NOT_SUPPORTED;
    ModbusRtuReadCoilsRspPduEncode(&rsp, buf, &bufLen);
    if (bufLen != 2) {
        printf("Wrong buffer length!\r\n");
        return -1;
    }
    if (buf[0] != MDBS_FC_READ_COILS + 0x80) {
        printf("Wrong function code!\r\n");
        return -1;
    }
    if (buf[1] != MDBS_EC_FUNCTION_NOT_SUPPORTED) {
        printf("Wrong exception code!\r\n");
    }

    uint8_t status[3] = {0xCD, 0x6B, 0x05};
    memset(buf, 0, 256);
    bufLen = 255;
    ModbusReadCoilsRspInit(&rsp);
    rsp.ByteCount = 3;
    rsp.CoilStatus = status;
    ModbusRtuReadCoilsRspPduEncode(&rsp, buf, &bufLen);
    if (bufLen != 5) {
        printf("Wrong buffer length!\r\n");
        return -1;
    }
    if (buf[0] != MDBS_FC_READ_COILS) {
        printf("Wrong function code!\r\n");
        return -1;
    }
    if (buf[1] != 3) {
        printf("Wrong byte count!\r\n");
        return -1;
    }
    if (memcmp(&buf[2], status, 3) != 0) {
        printf("Wrong coil status!\r\n");
    }

    printf("OK\r\n");
    return 0;
}

int TestModbusRtuReadCoilsRspPduDecode(void) {
    printf("Test ModbusRtuReadCoilsRspPduDecode: ");

    uint8_t buf[2] = {0};
    buf[0] = 0x80 + MDBS_FC_READ_COILS;
    buf[1] = MDBS_EC_INVALID_ADDRESS;
    ModbusReadCoilsRsp rsp;
    ModbusReadCoilsRspInit(&rsp);
    int rc = ModbusRtuReadCoilsRspPduDecode(&rsp, buf);
    if(rc != MDBS_EC_NONE){
        printf("Wrong return code!\r\n");
    }
    if (rsp.FunctionCode != MDBS_FC_READ_COILS) {
        printf("Wrong function code!\r\n");
        return -1;
    }
    if (rsp.ExceptionCode != MDBS_EC_INVALID_ADDRESS) {
        printf("Wrong exception code!\r\n");
        return -1;
    }

    uint8_t buf1[7] = {0x01, 0x05, 0xCD, 0x6B, 0xB2, 0x0E, 0x1B};
    ModbusReadCoilsRspInit(&rsp);
    rc = ModbusRtuReadCoilsRspPduDecode(&rsp, buf1);
    if(rc != MDBS_EC_NONE){
        printf("Wrong return code!\r\n");
    }
    if (rsp.FunctionCode != MDBS_FC_READ_COILS) {
        printf("Wrong function code!\r\n");
        return -1;
    }
    if (rsp.ExceptionCode != MDBS_EC_NONE) {
        printf("Wrong exception code!\r\n");
        return -1;
    }
    if(rsp.ByteCount != 0x05){
        printf("Wrong byte count!\r\n");
        return -1;
    }
    if(memcmp(&buf1[2], rsp.CoilStatus, 5) != 0){
        printf("Wrong coil status!\r\n");
        return -1;
    }

    printf("OK\r\n");
    return 0;
}

int main() {
    printf("============ Test start! ============\r\n");
    if (TestModbusReadCoilsReqInit() != 0) {
        return -1;
    }
    if (TestModbusRtuReadCoilsReqPduEncode() != 0) {
        return -1;
    }
    if (TestModbusRtuReadCoilsReqPduDecode() != 0) {
        return -1;
    }
    if (TestModbusReadCoilsRspInit() != 0) {
        return -1;
    }
    if (TestModbusRtuReadCoilsRspPduEncode() != 0) {
        return -1;
    }
    if (TestModbusRtuReadCoilsRspPduDecode() != 0) {
        return -1;
    }
    printf("============ Test end! ============\r\n");
    return 0;
}
