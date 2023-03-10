# MODBUS

Modbus protocol encode and decode library for mcu.

## Features

- high reliability.
- zero memory allocation.
- straight forward.
- high performance.
- high compatibility.

## Server working flow

```mermaid
graph TB
    entry([Entry])
    
    parse_adu[Parse adu]
    parse_func_code[Parse function code]
    check_func_code{{Function code supported?}}
    parse_pdu[Parse pdu]
    
    ignore_adu[Ignore adu]
    err_1[Function code not supported ec=1]
    send_err_rsp[Send error responce]
    
    exit([Exit])
    
    entry --> parse_adu
    parse_adu -- Checksum error --> ignore_adu
    parse_adu -- Not for this device  --> ignore_adu
    parse_adu -- Not error --> parse_func_code
    parse_func_code --> check_func_code
    check_func_code -- NO --> err_1
    check_func_code -- YES --> parse_pdu
    parse_pdu --> exit
    
    ignore_adu --> exit
    send_err_rsp --> exit
    err_1 --> send_err_rsp
```