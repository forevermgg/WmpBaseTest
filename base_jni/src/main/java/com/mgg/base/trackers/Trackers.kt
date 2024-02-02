package com.mgg.base.trackers

import android.content.Context
import com.mgg.base.executor.TaskExecutor
import com.mgg.base.trackers.battery.BatteryChargingTracker
import com.mgg.base.trackers.battery.BatteryNotLowTracker
import com.mgg.base.trackers.network.NetworkState
import com.mgg.base.trackers.network.NetworkStateTracker
import com.mgg.base.trackers.storage.StorageNotLowTracker

class Trackers @JvmOverloads constructor(
    context: Context,
    taskExecutor: TaskExecutor,
    /**
     * The tracker used to track the battery charging status.
     */
    val batteryChargingTracker: ConstraintTracker<Boolean> =
        BatteryChargingTracker(context.applicationContext, taskExecutor),

    /**
     * The tracker used to track if the battery is okay or low.
     */
    val batteryNotLowTracker: BatteryNotLowTracker =
        BatteryNotLowTracker(context.applicationContext, taskExecutor),

    /**
     * The tracker used to track network state changes.
     */
    val networkStateTracker: ConstraintTracker<NetworkState> =
        NetworkStateTracker(context.applicationContext, taskExecutor),

    /**
     * The tracker used to track if device storage is okay or low.
     */
    val storageNotLowTracker: ConstraintTracker<Boolean> =
        StorageNotLowTracker(context.applicationContext, taskExecutor),
)