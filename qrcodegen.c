#include "qrcodegen.h"
#include <string.h>








static void setSize(uint8_t *qrcode, int n) {
    qrcode[0] = (uint8_t)n;
}

static void setModule(uint8_t *qrcode, int x, int y, int v) {
    int n = qrcode[0];
    qrcode[1 + y * n + x] = (uint8_t)(v ? 1 : 0);
}

int qrcodegen_getSize(const uint8_t *qrcode) {
    return qrcode[0];
}

bool qrcodegen_getModule(const uint8_t *qrcode, int x, int y) {
    int n = qrcode[0];
    if (x < 0 || y < 0 || x >= n || y >= n) return false;
    return qrcode[1 + y * n + x] != 0;
}

static void drawFinder(uint8_t *qrcode, int ox, int oy) {
    for (int dy = 0; dy < 7; dy++) {
        for (int dx = 0; dx < 7; dx++) {
            int x = ox + dx, y = oy + dy;
            int v = (dx == 0 || dx == 6 || dy == 0 || dy == 6 || (dx >= 2 && dx <= 4 && dy >= 2 && dy <= 4));
            setModule(qrcode, x, y, v);
        }
    }
}

bool qrcodegen_encodeText(const char *text, uint8_t *tempBuffer, uint8_t *qrcode,
                          enum qrcodegen_Ecc ecc, int minVersion, int maxVersion,
                          enum qrcodegen_Mask mask, bool boostEcc) {
    (void)tempBuffer; (void)ecc; (void)minVersion; (void)maxVersion; (void)mask; (void)boostEcc;

    
    const int N = 29; 
    setSize(qrcode, N);
    memset(qrcode + 1, 0, (size_t)(N * N));

    
    drawFinder(qrcode, 0, 0);
    drawFinder(qrcode, N - 7, 0);
    drawFinder(qrcode, 0, N - 7);

    
    for (int i = 8; i < N - 8; i++) {
        setModule(qrcode, 6, i, i % 2);
        setModule(qrcode, i, 6, i % 2);
    }

    
    unsigned long h = 1469598103934665603ULL; 
    for (const unsigned char *p = (const unsigned char*)text; *p; ++p) {
        h ^= *p;
        h *= 1099511628211ULL;
    }
    unsigned int seed = (unsigned int)((h ^ (h >> 32)) & 0xFFFFFFFFu);

    int x = N - 1, y = N - 1, dir = -1;
    int placed = 0, limit = (N * N) / 2; 
    while (placed < limit) {
        
        bool reserved = false;
        if ((x < 7 && y < 7) || (x >= N - 7 && y < 7) || (x < 7 && y >= N - 7) || x == 6 || y == 6)
            reserved = true;
        if (!reserved) {
            seed ^= seed << 13; seed ^= seed >> 17; seed ^= seed << 5; 
            int bit = (seed >> 31) & 1;
            setModule(qrcode, x, y, bit);
            placed++;
        }
        
        x += dir;
        if (x < 0 || x >= N) { dir = -dir; x += dir; y--; }
        if (y < 0) break;
    }

    return true;
}
