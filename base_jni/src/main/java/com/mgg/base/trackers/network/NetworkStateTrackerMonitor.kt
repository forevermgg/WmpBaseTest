package com.mgg.base.trackers.network

import android.app.Application
import android.content.Context
import androidx.annotation.Keep
import com.mgg.base.trackers.ConstraintListener
import com.mgg.base.trackers.ConstraintTracker
@Keep
class NetworkStateTrackerMonitor constructor(application: Application) : ConstraintListener<NetworkState> {

    /**
     * The tracker used to track network state changes.
     */
    val networkStateTracker: ConstraintTracker<NetworkState> =
        NetworkStateTracker(application.applicationContext, ArchTaskExecutor.getInstance())

    fun currentState(): NetworkState {
        return networkStateTracker.state
    }

    fun addListener(): Boolean {
        networkStateTracker.addListener(this)
        return true
    }

    fun removeListener(): Boolean {
        networkStateTracker.removeListener(this)
        return true
    }

    override fun onConstraintChanged(newValue: NetworkState) {
    }

    external fun onConstraintChanged(isConnected: Boolean)
}