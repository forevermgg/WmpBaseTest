package com.mgg.flatbuffers

public class NativeFlatBuffersLib {

    public companion object {
        // Used to load the 'flatbuffers' library on application startup.
        init {
            System.loadLibrary("flatbuffers")
        }
    }
}