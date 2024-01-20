package com.mgg.flatbuffers

class NativeFlatBuffersLib {

    companion object {
        // Used to load the 'flatbuffers' library on application startup.
        init {
            System.loadLibrary("flatbuffers")
        }
    }
}