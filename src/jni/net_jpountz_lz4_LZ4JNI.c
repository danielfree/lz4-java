/*
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdint.h>

#include "lz4.h"
#include "lz4hc.h"
#include "lz4frame.h"
#include "net_jpountz_lz4_LZ4JNI.h"

static const LZ4F_preferences_t lz4_preferences = {
	{ LZ4F_max256KB, LZ4F_blockLinked, LZ4F_contentChecksumEnabled, LZ4F_frame, 0, { 0, 0 } },
	0,   /* compression level */
	0,   /* autoflush */
	{ 0, 0, 0, 0 },  /* reserved, must be set to 0 */
};

static jclass OutOfMemoryError;

JNIEXPORT void JNICALL Java_net_jpountz_lz4_LZ4JNI_init(JNIEnv *env, jclass cls) {
  OutOfMemoryError = (*env)->FindClass(env, "java/lang/OutOfMemoryError");
}

static void throw_OOM(JNIEnv *env) {
  (*env)->ThrowNew(env, OutOfMemoryError, "Out of memory");
}


void returnError(JNIEnv* env, jobject errorResult, LZ4F_errorCode_t errorCode) {
  // Get the ID for the `message` field in the error result
  jclass errorClass = (*env)->GetObjectClass(env, errorResult);
  jfieldID fid = (*env)->GetFieldID(env, errorClass, "message", "Ljava/lang/String;");

  // The string message in java string format
  jstring message = (*env)->NewStringUTF(env, LZ4F_getErrorName(errorCode));

  // And set it into the error result
  (*env)->SetObjectField(env, errorResult, fid, message);
}

jlong dContextToLong(LZ4F_decompressionContext_t context) {
  intptr_t readable_context = *(intptr_t*)&context;
  return (long) readable_context;
}

jlong cContextToLong(LZ4F_compressionContext_t context) {
  intptr_t readable_context = *(intptr_t*)&context;
  return (long) readable_context;
}

LZ4F_decompressionContext_t longToDContext(jlong readable_context) {
  intptr_t context = readable_context;
  return *(LZ4F_decompressionContext_t*)&context;
}

LZ4F_compressionContext_t longToCContext(jlong readable_context) {
  intptr_t context = readable_context;
  return *(LZ4F_compressionContext_t*)&context;
}

JNIEXPORT jlong JNICALL Java_net_jpountz_lz4_LZ4JNI_LZ4F_1createDecompressionContext(JNIEnv *env, jclass cls, jobject errorResult) {
  LZ4F_decompressionContext_t context;
  LZ4F_errorCode_t error = LZ4F_createDecompressionContext(&context, LZ4F_VERSION);
  if (LZ4F_isError(error)) {
    returnError(env, errorResult, error);
    return 0;
  }
  return dContextToLong(context);
}

JNIEXPORT void JNICALL Java_net_jpountz_lz4_LZ4JNI_LZ4F_1freeDecompressionContext(JNIEnv *env, jclass cls, jlong readable_context, jobject errorResult) {
  LZ4F_errorCode_t error = LZ4F_freeDecompressionContext(longToDContext(readable_context));
  if (LZ4F_isError(error))
    returnError(env, errorResult, error);
}

JNIEXPORT jint JNICALL Java_net_jpountz_lz4_LZ4JNI_LZ4F_1decompress(JNIEnv* env, jclass cls, jlong readable_context, jbyteArray srcBuffer, jint srcOffset, jint srcSize, jbyteArray dstBuffer, jint dstOffset, jint dstSize, jobject errorResult) {
  void* availableDstBuffer = (void*) (*env)->GetPrimitiveArrayCritical(env, dstBuffer, 0) + dstOffset;
  void* availableSrcBuffer = (void*) (*env)->GetPrimitiveArrayCritical(env, srcBuffer, 0) + srcOffset;
  LZ4F_decompressOptions_t options;
  size_t total_written = 0;
  size_t total_read = 0;
  LZ4F_decompressionContext_t context = longToDContext(readable_context);
  while (total_read < srcSize) {
    size_t finalDstSize = dstSize - total_written;
    size_t finalSrcSize = srcSize - total_read;
    if (finalDstSize == 0) {
      return -total_read-1;
    }
    size_t result = LZ4F_decompress(context, availableDstBuffer + total_written, &finalDstSize, availableSrcBuffer + total_read, &finalSrcSize, &options);
    if (LZ4F_isError(result)) {
      returnError(env, errorResult, result);
      return 0;
    }
    total_written += finalDstSize;
    total_read += finalSrcSize;
  }
  return total_written;
}

JNIEXPORT jlong JNICALL Java_net_jpountz_lz4_LZ4JNI_LZ4F_1createCompressionContext(JNIEnv* env, jclass cls, jobject errorResult) {
  LZ4F_compressionContext_t context;
  LZ4F_errorCode_t error = LZ4F_createCompressionContext(&context, LZ4F_VERSION);
  if (LZ4F_isError(error)) {
    returnError(env, errorResult, error);
    return 0;
  }
  return cContextToLong(context);
}

JNIEXPORT void JNICALL Java_net_jpountz_lz4_LZ4JNI_LZ4F_1freeCompressionContext(JNIEnv *env, jclass cls, jlong readable_context, jobject errorResult) {
  LZ4F_errorCode_t error = LZ4F_freeCompressionContext(longToCContext(readable_context));
  if (LZ4F_isError(error))
    returnError(env, errorResult, error);
}

JNIEXPORT jint JNICALL Java_net_jpountz_lz4_LZ4JNI_LZ4F_1compressBegin(JNIEnv *env, jclass cls, jlong readable_context, jint compressionLevel, jbyteArray dstBuffer, jint dstOffset, jint dstSize, jobject errorResult) {
  void* availableDstBuffer = (void*) (*env)->GetPrimitiveArrayCritical(env, dstBuffer, 0) + dstOffset;
  LZ4F_preferences_t prefs = lz4_preferences;
  prefs.compressionLevel = compressionLevel;
  size_t result = LZ4F_compressBegin(longToCContext(readable_context), availableDstBuffer, dstSize, &prefs);
  if (LZ4F_isError(result)) {
    returnError(env, errorResult, result);
    return 0;
  }
  return result;
}

JNIEXPORT jint JNICALL Java_net_jpountz_lz4_LZ4JNI_LZ4F_1compressBound(JNIEnv* env, jclass cls, jint srcSize) {
  return LZ4F_compressBound(srcSize, &lz4_preferences);
}

JNIEXPORT jint JNICALL Java_net_jpountz_lz4_LZ4JNI_LZ4F_1compressUpdate(JNIEnv *env, jclass cls, jlong readable_context, jbyteArray srcBuffer, jint srcOffset, jint srcSize, jbyteArray dstBuffer, jint dstOffset, jint dstSize, jobject errorResult) {
  void* availableDstBuffer = (void*) (*env)->GetPrimitiveArrayCritical(env, dstBuffer, 0) + dstOffset;
  void* availableSrcBuffer = (void*) (*env)->GetPrimitiveArrayCritical(env, srcBuffer, 0) + srcOffset;
  size_t result = LZ4F_compressUpdate(longToCContext(readable_context), availableDstBuffer, dstSize, availableSrcBuffer, srcSize, NULL);
  if (LZ4F_isError(result)) {
    returnError(env, errorResult, result);
    return 0;
  }
  return result;
}

JNIEXPORT jint JNICALL Java_net_jpountz_lz4_LZ4JNI_LZ4F_1compressEnd(JNIEnv *env, jclass cls, jlong readable_context, jbyteArray dstBuffer, jint dstOffset, jint dstSize, jobject errorResult) {
  void* availableDstBuffer = (void*) (*env)->GetPrimitiveArrayCritical(env, dstBuffer, 0) + dstOffset;
  size_t result = LZ4F_compressEnd(longToCContext(readable_context), availableDstBuffer, dstSize, NULL);
  if (LZ4F_isError(result)) {
    returnError(env, errorResult, result);
    return 0;
  }
  return result;
}

JNIEXPORT jint JNICALL Java_net_jpountz_lz4_LZ4JNI_LZ4F_1compressFlush(JNIEnv *env, jclass cls, jlong readable_context, jbyteArray dstBuffer, jint dstOffset, jint dstSize, jobject errorResult) {
  void* availableDstBuffer = (void*) (*env)->GetPrimitiveArrayCritical(env, dstBuffer, 0) + dstOffset;
  size_t result = LZ4F_flush(longToCContext(readable_context), availableDstBuffer, dstSize, NULL);
  if (LZ4F_isError(result)) {
    returnError(env, errorResult, result);
    return 0;
  }
  return result;
}
