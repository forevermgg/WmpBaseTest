package com.mgg.varinat

public class NativeVariantLib {

    public companion object {
        // Used to load the 'varinat' library on application startup.
        init {
            System.loadLibrary("variant")
        }
    }
}