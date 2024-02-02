package com.mgg.base.trackers

import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.content.IntentFilter
import android.util.Log
import com.mgg.base.executor.TaskExecutor

abstract class BroadcastReceiverConstraintTracker<T>(
    context: Context,
    taskExecutor: TaskExecutor
) : ConstraintTracker<T>(context, taskExecutor) {
    private val broadcastReceiver: BroadcastReceiver = object : BroadcastReceiver() {
        override fun onReceive(context: Context, intent: Intent) {
            onBroadcastReceive(intent)
        }
    }

    /**
     * Called when the [BroadcastReceiver] is receiving an [Intent] broadcast and should
     * handle the received [Intent].
     *
     * @param intent  The [Intent] being received.
     */
    abstract fun onBroadcastReceive(intent: Intent)

    /**
     * @return The [IntentFilter] associated with this tracker.
     */
    abstract val intentFilter: IntentFilter

    override fun startTracking() {
        Log.d(TAG, "${javaClass.simpleName}: registering receiver")
        appContext.registerReceiver(broadcastReceiver, intentFilter)
    }

    override fun stopTracking() {
        Log.d(TAG, "${javaClass.simpleName}: unregistering receiver")
        appContext.unregisterReceiver(broadcastReceiver)
    }
}

private val TAG = ("BroadcastReceiverConstraintTracker")