package com.mgg.base.trackers.storage

import android.content.Context
import android.content.Intent
import android.content.IntentFilter
import android.util.Log
import androidx.annotation.RestrictTo
import com.mgg.base.executor.TaskExecutor
import com.mgg.base.trackers.BroadcastReceiverConstraintTracker

/**
 * Tracks whether or not the device's storage is low.
 */
@Suppress("DEPRECATION")
@RestrictTo(RestrictTo.Scope.LIBRARY_GROUP)
class StorageNotLowTracker(context: Context, taskExecutor: TaskExecutor) :
    BroadcastReceiverConstraintTracker<Boolean>(context, taskExecutor) {

    override val initialState: Boolean
        get() {
            val intent = appContext.registerReceiver(null, intentFilter)
            return if (intent == null || intent.action == null) {
                // ACTION_DEVICE_STORAGE_LOW is a sticky broadcast that is removed when sufficient
                // storage is available again. ACTION_DEVICE_STORAGE_OK is not sticky. So if we
                // don't receive anything here, we can assume that the storage state is okay.
                true
            } else {
                when (intent.action) {
                    Intent.ACTION_DEVICE_STORAGE_OK -> true
                    Intent.ACTION_DEVICE_STORAGE_LOW -> false
                    else ->
                        // This should never happen because the intent filter is configured
                        // correctly.
                        false
                }
            }
        }

    override val intentFilter: IntentFilter
        get() {
            // In API 26+, DEVICE_STORAGE_OK/LOW are deprecated and are no longer sent to
            // manifest-defined BroadcastReceivers. Since we are registering our receiver manually, this
            // is currently okay. This may change in future versions, so this will need to be monitored.
            val intentFilter = IntentFilter()
            intentFilter.addAction(Intent.ACTION_DEVICE_STORAGE_OK)
            intentFilter.addAction(Intent.ACTION_DEVICE_STORAGE_LOW)
            return intentFilter
        }

    override fun onBroadcastReceive(intent: Intent) {
        if (intent.action == null) {
            return // Should never happen since the IntentFilter was configured.
        }
        Log.d(TAG, "Received " + intent.action)
        when (intent.action) {
            Intent.ACTION_DEVICE_STORAGE_OK -> state = true
            Intent.ACTION_DEVICE_STORAGE_LOW -> state = false
        }
    }
}

private val TAG = ("StorageNotLowTracker")