#include "modbus.h"

#include <stdio.h>
#include <stdint.h>

int TestModbusReadCoilsReqInit() {
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

int TestModbusRtuReadCoilsReqPduEncode() {
    printf("Test ModbusRtuReadCoilsReqPduEncode: ");

    ModbusReadCoilsReq req;
    ModbusReadCoilsReqInit(&req);

    uint8_t buf[256] = {0};
    uint8_t buf_len = 255;
    req.StartingAddress = 20;
    req.QuantityOfCoils = 37;
    int rc = ModbusRtuReadCoilsReqPduEncode(&req, buf, &buf_len);
    if (rc != 0) {
        printf("Error code: %d\r\n", rc);
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

int TestModbusRtuReadCoilsReqPduDecode() {
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
    printf("============ Test end! ============\r\n");
    return 0;
}
