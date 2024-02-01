package com.mgg.base

import android.os.Trace

class NativeBaseLib {

    external fun getVersion(): String

    external fun register()

    external fun unregister()

    companion object {
        // Used to load the 'base' library on application startup.
        init {
            Trace.beginSection("System.loadLibrary(base_jni)")
            System.loadLibrary("base_jni")
            Trace.endSection()
        }
    }
}