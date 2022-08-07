#ifndef FISHERYATESSHUFFLE_H
#define FISHERYATESSHUFFLE_H

#include <jni.h>
#include <cstddef>

extern void FisherYatesShuffle(jbyte*,jint,jint,jint,jint,const jint*,jint,jint,jboolean,jbyte*,jint);

#ifdef __cplusplus
extern "C" {
#endif
    JNIEXPORT void JNICALL
    Java_hideo_miyauchi_fisheryatesshuffle_MainActivity_jniFisherYatesShuffle(
            JNIEnv *env, __attribute__((unused)) jobject thiz,
            jbyteArray jbyteArray_rawBitmapByteArray, jint width, jint height, jint rowBytes,
            jint bytesPerPixel, jintArray jintArray_randomSequence, jint divV, jint divH,
            jboolean restore) {

        // pre-processing
        jint* randomSequence = env->GetIntArrayElements(jintArray_randomSequence, 0);
        jbyte* rawBitmapByteArray = env->GetByteArrayElements(jbyteArray_rawBitmapByteArray, 0);
        int nbytes = (width / divH) * bytesPerPixel;
        jbyteArray jcharArray_temp_buffer = env->NewByteArray(nbytes);
        jbyte* temp_buffer = env->GetByteArrayElements(jcharArray_temp_buffer, 0);

        // call implementation
        FisherYatesShuffle(rawBitmapByteArray, width, height, rowBytes, bytesPerPixel,
                           randomSequence, divV, divH, restore, temp_buffer, nbytes);

        // post-processing
        env->ReleaseByteArrayElements(jcharArray_temp_buffer, temp_buffer, 0);
        env->DeleteLocalRef(jcharArray_temp_buffer);
        env->ReleaseByteArrayElements(jbyteArray_rawBitmapByteArray, rawBitmapByteArray, 0);
        env->ReleaseIntArrayElements(jintArray_randomSequence, randomSequence, 0);
    }
#ifdef __cplusplus
}
#endif

#endif // FISHERYATESSHUFFLE_H
