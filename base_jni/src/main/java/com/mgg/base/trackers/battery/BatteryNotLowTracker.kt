package com.mgg.base.trackers.battery


import android.content.Context
import android.content.Intent
import android.content.IntentFilter
import android.os.BatteryManager
import android.util.Log
import com.mgg.base.executor.TaskExecutor
import com.mgg.base.trackers.BroadcastReceiverConstraintTracker

/**
 * Tracks whether or not the device's battery level is low.
 */
class BatteryNotLowTracker(context: Context, taskExecutor: TaskExecutor) :
    BroadcastReceiverConstraintTracker<Boolean>(context, taskExecutor) {
    /**
     * Based on BatteryService#shouldSendBatteryLowLocked(), but this ignores the previous plugged
     * state - cannot guarantee the last plugged state because this isn't always tracking.
     *
     * {@see https://android.googlesource.com/platform/frameworks/base/+/oreo-release/services/core/java/com/android/server/BatteryService.java#268}
     */
    override val initialState: Boolean
        get() {
            val intentFilter = IntentFilter(Intent.ACTION_BATTERY_CHANGED)
            val intent = appContext.registerReceiver(null, intentFilter)
            if (intent == null) {
                Log.e(TAG, "getInitialState - null intent received")
                return false
            }
            val status = intent.getIntExtra(BatteryManager.EXTRA_STATUS, -1)
            val level = intent.getIntExtra(BatteryManager.EXTRA_LEVEL, -1)
            val scale = intent.getIntExtra(BatteryManager.EXTRA_SCALE, -1)
            val batteryPercentage = level / scale.toFloat()

            // BATTERY_STATUS_UNKNOWN typically refers to devices without a battery.
            // So those kinds of devices must be allowed.
            return status == BatteryManager.BATTERY_STATUS_UNKNOWN ||
                    batteryPercentage > BATTERY_LOW_THRESHOLD
        }

    override val intentFilter: IntentFilter
        get() {
            val intentFilter = IntentFilter()
            intentFilter.addAction(Intent.ACTION_BATTERY_OKAY)
            intentFilter.addAction(Intent.ACTION_BATTERY_LOW)
            return intentFilter
        }

    override fun onBroadcastReceive(intent: Intent) {
        if (intent.action == null) {
            return
        }
        Log.d(TAG, "Received ${intent.action}")
        when (intent.action) {
            Intent.ACTION_BATTERY_OKAY -> state = true
            Intent.ACTION_BATTERY_LOW -> state = false
        }
    }
}

private val TAG = ("BatteryNotLowTracker")

/**
 * {@see https://android.googlesource.com/platform/frameworks/base/+/oreo-release/core/res/res/values/config.xml#986}
 */
internal const val BATTERY_LOW_THRESHOLD = 0.15f