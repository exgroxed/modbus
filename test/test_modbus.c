#include "modbus.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef int (*TestFunctionPrototype)(void);

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

int TestModbusRtuReadCoilsReqAduEncode(void) {
    printf("Test ModbusRtuReadCoilsReqAduEncode: ");

    uint8_t input[] = {0x11, 0x01, 0x00, 0x13, 0x00, 0x25, 0x0E, 0x84};
    uint8_t aduBuf[256] = {0};
    uint8_t aduBufLen = 255;

    ModbusReadCoilsReq req;
    ModbusReadCoilsReqInit(&req);
    req.StartingAddress = 20;
    req.QuantityOfCoils = 37;

    ModbusRtuAdu adu;
    ModbusRtuAduInit(&adu);
    adu.DeviceAddress = 17;
    adu.Pdu = &req;
    ModbusRtuReadCoilsReqAduEncode(&adu, aduBuf, &aduBufLen);

    if (memcmp(input, aduBuf, aduBufLen) != 0) {
        printf("Wrong result!\r\n");
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
    int rc = ModbusRtuReadCoilsReqPduDecode(&req, input, 5);
    if (rc != MDBS_ERR_NONE) {
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

int TestModbusRtuReadCoilsReqAduDecode(void) {
    printf("Test ModbusRtuReadCoilsReqAduDecode: ");

    uint8_t input[8] = {0x11, 0x01, 0x00, 0x13, 0x00, 0x25, 0x0E, 0x84};

    ModbusReadCoilsReq req;
    ModbusReadCoilsReqInit(&req);
    ModbusRtuAdu adu;
    ModbusRtuAduInit(&adu);
    adu.Pdu = &req;
    int err = ModbusRtuReadCoilsReqAduDecode(&adu, input, 8);
    if (err != MDBS_ERR_NONE) {
        printf("Wrong return code!\r\n");
        return -1;
    }
    if (req.FunctionCode != MDBS_FC_READ_COILS
        || req.StartingAddress != 20
        || req.QuantityOfCoils != 37) {
        printf("Wrong result!\r\n");
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

int TestModbusRtuReadCoilsRspAduEncode(void) {
    printf("Test ModbusRtuReadCoilsRspPduEncode: ");

    uint8_t input[10] = {0x11, 0x01, 0x05, 0xCD, 0x6B, 0xB2, 0x0E, 0x1B, 0x45, 0xE6};

    uint8_t status[5] = {0xCD, 0x6B, 0xB2, 0x0E, 0x1B};
    uint8_t output[10] = {0};
    uint8_t outputLen = 10;
    ModbusReadCoilsRsp rsp;
    ModbusReadCoilsRspInit(&rsp);
    rsp.ByteCount = 5;
    rsp.CoilStatus = status;

    ModbusRtuAdu adu;
    ModbusRtuAduInit(&adu);
    adu.DeviceAddress = 17;
    adu.Pdu = &rsp;
    int err = ModbusRtuReadCoilsRspAduEncode(&adu, output, &outputLen);
    if (err != MDBS_ERR_NONE) {
        printf("Wrong return code: %d\r\n", err);
        return -1;
    }
    if (memcmp(input, output, 10) != 0) {
        printf("Wrong result!\r\n");
        return -1;
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
    int rc = ModbusRtuReadCoilsRspPduDecode(&rsp, buf, 2);
    if (rc != MDBS_EC_NONE) {
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
    rc = ModbusRtuReadCoilsRspPduDecode(&rsp, buf1, 7);
    if (rc != MDBS_EC_NONE) {
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
    if (rsp.ByteCount != 0x05) {
        printf("Wrong byte count!\r\n");
        return -1;
    }
    if (memcmp(&buf1[2], rsp.CoilStatus, 5) != 0) {
        printf("Wrong coil status!\r\n");
        return -1;
    }

    printf("OK\r\n");
    return 0;
}

int TestModbusRtuReadCoilsRspAduDecode(void) {
    printf("Test ModbusRtuReadCoilsRspAduDecode: ");

    uint8_t buf[10] = {0x11, 0x01, 0x05, 0xCD, 0x6B, 0xB2, 0x0E, 0x1B, 0x45, 0xE6};
    uint8_t bufLen = 10;
    ModbusReadCoilsRsp rsp;
    ModbusReadCoilsRspInit(&rsp);
    ModbusRtuAdu adu;
    ModbusRtuAduInit(&adu);
    adu.Pdu = &rsp;
    ModbusRtuReadCoilsRspAduDecode(&adu, buf, bufLen);
    if (rsp.FunctionCode != MDBS_FC_READ_COILS
        || rsp.ExceptionCode != MDBS_EC_NONE
        || rsp.ByteCount != 0x05
        || memcmp(&buf[3], rsp.CoilStatus, 5) != 0) {
        printf("Wrong result!\r\n");
        return -1;
    }

    printf("OK\r\n");
    return 0;
}

int main() {
    printf("============ Test start! ============\r\n");
    TestFunctionPrototype Tests[] = {
            TestModbusRtuReadCoilsReqPduEncode,
            TestModbusRtuReadCoilsReqAduEncode,
            TestModbusRtuReadCoilsReqPduDecode,
            TestModbusRtuReadCoilsReqAduDecode,
            TestModbusRtuReadCoilsRspPduEncode,
            TestModbusRtuReadCoilsRspAduEncode,
            TestModbusRtuReadCoilsRspPduDecode,
            TestModbusRtuReadCoilsRspAduDecode,
    };

    int i;
    for (i = 0; i < sizeof(Tests) / sizeof(TestFunctionPrototype); i++) {
        if (Tests[i]() != 0) {
            return -1;
        }
    }
    printf("============ Test end! ============\r\n");
    return 0;
}
