package com.mgg.base.trackers.battery

import android.content.Context
import android.content.Intent
import android.content.IntentFilter
import android.os.BatteryManager
import android.os.BatteryManager.BATTERY_STATUS_CHARGING
import android.os.BatteryManager.BATTERY_STATUS_FULL
import android.os.Build
import android.util.Log
import com.mgg.base.executor.TaskExecutor
import com.mgg.base.trackers.BroadcastReceiverConstraintTracker

/**
 * Tracks whether or not the device's battery is charging.
 */
class BatteryChargingTracker(context: Context, taskExecutor: TaskExecutor) :
    BroadcastReceiverConstraintTracker<Boolean>(context, taskExecutor) {

    override val initialState: Boolean
        get() {
            // {@link ACTION_CHARGING} and {@link ACTION_DISCHARGING} are not sticky broadcasts, so
            // we use {@link ACTION_BATTERY_CHANGED} on all APIs to get the initial state.
            val intentFilter = IntentFilter(Intent.ACTION_BATTERY_CHANGED)
            val intent = appContext.registerReceiver(null, intentFilter)
            if (intent == null) {
                Log.e(TAG, "getInitialState - null intent received")
                return false
            }
            return isBatteryChangedIntentCharging(intent)
        }

    override val intentFilter: IntentFilter
        get() {
            val intentFilter = IntentFilter()
            if (Build.VERSION.SDK_INT >= 23) {
                intentFilter.addAction(BatteryManager.ACTION_CHARGING)
                intentFilter.addAction(BatteryManager.ACTION_DISCHARGING)
            } else {
                intentFilter.addAction(Intent.ACTION_POWER_CONNECTED)
                intentFilter.addAction(Intent.ACTION_POWER_DISCONNECTED)
            }
            return intentFilter
        }

    override fun onBroadcastReceive(intent: Intent) {
        val action = intent.action ?: return
        Log.d(TAG, "Received $action")
        when (action) {
            BatteryManager.ACTION_CHARGING -> state = true
            BatteryManager.ACTION_DISCHARGING -> state = false
            Intent.ACTION_POWER_CONNECTED -> state = true
            Intent.ACTION_POWER_DISCONNECTED -> state = false
        }
    }

    private fun isBatteryChangedIntentCharging(intent: Intent): Boolean {
        return if (Build.VERSION.SDK_INT >= 23) {
            val status = intent.getIntExtra(BatteryManager.EXTRA_STATUS, -1)
            (status == BATTERY_STATUS_CHARGING || status == BATTERY_STATUS_FULL)
        } else {
            intent.getIntExtra(BatteryManager.EXTRA_PLUGGED, 0) != 0
        }
    }
}

private val TAG = ("BatteryChargingTracker")