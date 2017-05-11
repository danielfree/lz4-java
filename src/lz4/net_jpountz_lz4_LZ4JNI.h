/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class net_jpountz_lz4_LZ4JNI */

#ifndef _Included_net_jpountz_lz4_LZ4JNI
#define _Included_net_jpountz_lz4_LZ4JNI
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     net_jpountz_lz4_LZ4JNI
 * Method:    init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_net_jpountz_lz4_LZ4JNI_init
  (JNIEnv *, jclass);

/*
 * Class:     net_jpountz_lz4_LZ4JNI
 * Method:    LZ4F_createCompressionContext
 * Signature: (Lnet/jpountz/lz4/LZ4JNI/LZ4FError;)J
 */
JNIEXPORT jlong JNICALL Java_net_jpountz_lz4_LZ4JNI_LZ4F_1createCompressionContext
  (JNIEnv *, jclass, jobject);

/*
 * Class:     net_jpountz_lz4_LZ4JNI
 * Method:    LZ4F_freeCompressionContext
 * Signature: (JLnet/jpountz/lz4/LZ4JNI/LZ4FError;)V
 */
JNIEXPORT void JNICALL Java_net_jpountz_lz4_LZ4JNI_LZ4F_1freeCompressionContext
  (JNIEnv *, jclass, jlong, jobject);

/*
 * Class:     net_jpountz_lz4_LZ4JNI
 * Method:    LZ4F_compressBegin
 * Signature: (JI[BIILnet/jpountz/lz4/LZ4JNI/LZ4FError;)I
 */
JNIEXPORT jint JNICALL Java_net_jpountz_lz4_LZ4JNI_LZ4F_1compressBegin
  (JNIEnv *, jclass, jlong, jint, jbyteArray, jint, jint, jobject);

/*
 * Class:     net_jpountz_lz4_LZ4JNI
 * Method:    LZ4F_compressBound
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_jpountz_lz4_LZ4JNI_LZ4F_1compressBound
  (JNIEnv *, jclass, jint);

/*
 * Class:     net_jpountz_lz4_LZ4JNI
 * Method:    LZ4F_compressUpdate
 * Signature: (J[BII[BIILnet/jpountz/lz4/LZ4JNI/LZ4FError;)I
 */
JNIEXPORT jint JNICALL Java_net_jpountz_lz4_LZ4JNI_LZ4F_1compressUpdate
  (JNIEnv *, jclass, jlong, jbyteArray, jint, jint, jbyteArray, jint, jint, jobject);

/*
 * Class:     net_jpountz_lz4_LZ4JNI
 * Method:    LZ4F_compressEnd
 * Signature: (J[BIILnet/jpountz/lz4/LZ4JNI/LZ4FError;)I
 */
JNIEXPORT jint JNICALL Java_net_jpountz_lz4_LZ4JNI_LZ4F_1compressEnd
  (JNIEnv *, jclass, jlong, jbyteArray, jint, jint, jobject);

/*
 * Class:     net_jpountz_lz4_LZ4JNI
 * Method:    LZ4F_compressFlush
 * Signature: (J[BIILnet/jpountz/lz4/LZ4JNI/LZ4FError;)I
 */
JNIEXPORT jint JNICALL Java_net_jpountz_lz4_LZ4JNI_LZ4F_1compressFlush
  (JNIEnv *, jclass, jlong, jbyteArray, jint, jint, jobject);

/*
 * Class:     net_jpountz_lz4_LZ4JNI
 * Method:    LZ4F_createDecompressionContext
 * Signature: (Lnet/jpountz/lz4/LZ4JNI/LZ4FError;)J
 */
JNIEXPORT jlong JNICALL Java_net_jpountz_lz4_LZ4JNI_LZ4F_1createDecompressionContext
  (JNIEnv *, jclass, jobject);

/*
 * Class:     net_jpountz_lz4_LZ4JNI
 * Method:    LZ4F_freeDecompressionContext
 * Signature: (JLnet/jpountz/lz4/LZ4JNI/LZ4FError;)V
 */
JNIEXPORT void JNICALL Java_net_jpountz_lz4_LZ4JNI_LZ4F_1freeDecompressionContext
  (JNIEnv *, jclass, jlong, jobject);

/*
 * Class:     net_jpountz_lz4_LZ4JNI
 * Method:    LZ4F_decompress
 * Signature: (J[BII[BIILnet/jpountz/lz4/LZ4JNI/LZ4FError;)I
 */
JNIEXPORT jint JNICALL Java_net_jpountz_lz4_LZ4JNI_LZ4F_1decompress
  (JNIEnv *, jclass, jlong, jbyteArray, jint, jint, jbyteArray, jint, jint, jobject);

#ifdef __cplusplus
}
#endif
#endif
