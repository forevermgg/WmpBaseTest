package com.mgg.base

class NativeBaseLib {

    external fun getVersion(): String

    companion object {
        // Used to load the 'base' library on application startup.
        init {
            System.loadLibrary("base_jni")
        }
    }
}