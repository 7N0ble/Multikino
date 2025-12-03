#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "qrcodegen.h"

extern "C" void generateQRCode(const char *text) {
    uint8_t qrcode[qrcodegen_BUFFER_LEN_MAX];
    uint8_t tempBuffer[qrcodegen_BUFFER_LEN_MAX];

    bool ok = qrcodegen_encodeText(
            text, tempBuffer, qrcode,
            qrcodegen_Ecc_LOW,
            qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX,
            qrcodegen_Mask_AUTO, true);

    if (!ok) {
        printf("Nelze vygenerovat QR kod!\n");
        return;
    }

    int size = qrcodegen_getSize(qrcode);

    printf("\nQR KOD PRO VSTUPENKU:\n\n");

    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            printf(qrcodegen_getModule(qrcode, x, y) ? "##" : "  ");
        }
        printf("\n");
    }

    printf("\n");
}
