package com.mgg.varinat

class NativeLib {

    /**
     * A native method that is implemented by the 'varinat' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String

    companion object {
        // Used to load the 'varinat' library on application startup.
        init {
            System.loadLibrary("variant")
        }
    }
}