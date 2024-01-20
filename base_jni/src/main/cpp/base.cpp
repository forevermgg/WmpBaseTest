#include <jni.h>
#include <string>

extern "C"
JNIEXPORT jstring JNICALL
Java_com_mgg_base_NativeBaseLib_getVersion(JNIEnv *env, jobject thiz) {
    std::string hello = "Base JNI from C++";
    return env->NewStringUTF(hello.c_str());
}