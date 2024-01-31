#include <jni.h>
#include <string>
#include "TestData.h"

std::unique_ptr<TestData> test_data_;

extern "C"
JNIEXPORT void JNICALL
Java_com_mgg_base_NativeBaseLib_register(JNIEnv *env, jobject thiz) {
  test_data_ = std::make_unique<TestData>();
  test_data_->TestExec();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_mgg_base_NativeBaseLib_unregister(JNIEnv *env, jobject thiz) {
  test_data_.reset();
}
