


#ifndef QRCODEGEN_H
#define QRCODEGEN_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

enum qrcodegen_Ecc {
    qrcodegen_Ecc_LOW = 0,
    qrcodegen_Ecc_MEDIUM,
    qrcodegen_Ecc_QUARTILE,
    qrcodegen_Ecc_HIGH,
};

enum qrcodegen_Mask {
    qrcodegen_Mask_AUTO = -1,
    qrcodegen_Mask_0 = 0,
    qrcodegen_Mask_1,
    qrcodegen_Mask_2,
    qrcodegen_Mask_3,
    qrcodegen_Mask_4,
    qrcodegen_Mask_5,
    qrcodegen_Mask_6,
    qrcodegen_Mask_7,
};

#define qrcodegen_VERSION_MIN 1
#define qrcodegen_VERSION_MAX 40


#define qrcodegen_BUFFER_LEN_MAX (1 + 177 * 177)

bool qrcodegen_encodeText(const char *text, uint8_t *tempBuffer, uint8_t *qrcode,
                          enum qrcodegen_Ecc ecc, int minVersion, int maxVersion,
                          enum qrcodegen_Mask mask, bool boostEcc);

int qrcodegen_getSize(const uint8_t *qrcode);

bool qrcodegen_getModule(const uint8_t *qrcode, int x, int y);

#ifdef __cplusplus
}
#endif

#endif 
