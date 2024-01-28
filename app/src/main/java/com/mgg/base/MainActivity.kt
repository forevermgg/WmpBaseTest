package com.mgg.base

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.TextView
import com.mgg.base.databinding.ActivityMainBinding
import com.mgg.varinat.NativeVariantLib

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    private var testNativeBaseLib = NativeBaseLib()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        // Example of a call to a native method
        binding.sampleText.text = stringFromJNI()

        NativeVariantLib.Companion.toString()

        testNativeBaseLib.register()
    }

    override fun onDestroy() {
        super.onDestroy()
        testNativeBaseLib.unregister()
    }

    /**
     * A native method that is implemented by the 'base' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String

    companion object {
        // Used to load the 'base' library on application startup.
        init {
            System.loadLibrary("app")
        }
    }
}