package com.mgg.base.trackers.network

import android.app.Application
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

    fun isLanConnected(): Boolean {
        return networkStateTracker.state.isConnected
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
        onConstraintChanged(1, newValue.isConnected)
    }

    private external fun onConstraintChanged(connectType: Int, isConnected: Boolean)
}