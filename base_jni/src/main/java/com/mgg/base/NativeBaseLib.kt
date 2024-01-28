package com.mgg.base

class NativeBaseLib {

    external fun getVersion(): String

    external fun register()

    external fun unregister()

    companion object {
        // Used to load the 'base' library on application startup.
        init {
            System.loadLibrary("base_jni")
        }
    }
}