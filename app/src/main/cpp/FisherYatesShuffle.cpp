
#include <array>
#include "FisherYatesShuffle.h"

static void swap(jint i, jint j, jbyte *rawBitmapByteArray, jint width, jint height, jint rowBytes,
                 jint bytesPerPixel, jint divV, jint divH, jbyte *temp_buffer, jint nbytes) {

    int pi = (i % divH) * (width / divH) + (i / divH) * (height / divV) * width;
    int pj = (j % divH) * (width / divH) + (j / divH) * (height / divV) * width;

    for (int y = 0;y < (height / divV);y++) {

        int spi = pi * bytesPerPixel + y * rowBytes;
        int spj = pj * bytesPerPixel + y * rowBytes;

        memcpy(temp_buffer, &rawBitmapByteArray[spi], nbytes);
        memcpy(&rawBitmapByteArray[spi], &rawBitmapByteArray[spj], nbytes);
        memcpy(&rawBitmapByteArray[spj], temp_buffer, nbytes);
    }
}

void FisherYatesShuffle(jbyte *rawBitmapByteArray, jint width, jint height, jint rowBytes,
                        jint bytesPerPixel, const jint *randomSequence, jint divV, jint divH,
                        jboolean restore, jbyte *temp_buffer, jint nbytes) {

    int n = divH * divV;

    if (!restore) {
        for (int i = n-1;i >= 0;i--) {
            int j = randomSequence[i];
            swap(i, j, rawBitmapByteArray, width, height, rowBytes, bytesPerPixel, divV, divH, temp_buffer, nbytes);
        }
    } else {
        for (int i = 0;i < n;i++) {
            int j = randomSequence[i];
            swap(i, j, rawBitmapByteArray, width, height, rowBytes, bytesPerPixel, divV, divH, temp_buffer, nbytes);
        }
    }
}
