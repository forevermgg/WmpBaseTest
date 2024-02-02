package com.mgg.base.trackers.network

import android.content.Context
import android.content.Intent
import android.content.IntentFilter
import android.net.ConnectivityManager
import android.net.NetworkCapabilities
import android.os.Build
import android.util.Log
import androidx.core.net.ConnectivityManagerCompat
import com.mgg.base.executor.TaskExecutor
import com.mgg.base.trackers.BroadcastReceiverConstraintTracker

class NetworkStateTrackerPre24(context: Context, taskExecutor: TaskExecutor) :
    BroadcastReceiverConstraintTracker<NetworkState>(context, taskExecutor) {

    private val connectivityManager: ConnectivityManager =
        appContext.getSystemService(Context.CONNECTIVITY_SERVICE) as ConnectivityManager

    private val ConnectivityManager.isActiveNetworkValidated: Boolean
        get() = if (Build.VERSION.SDK_INT < 23) {
            false // NET_CAPABILITY_VALIDATED not available until API 23. Used on API 26+.
        } else try {
            val network = getActiveNetworkCompat()
            val capabilities = getNetworkCapabilitiesCompat(network)
            (capabilities?.hasCapabilityCompat(NetworkCapabilities.NET_CAPABILITY_VALIDATED))
                ?: false
        } catch (exception: SecurityException) {
            // b/163342798
            Log.e(TAG, "Unable to validate active network", exception)
            false
        }

    @Suppress("DEPRECATION")
    private val ConnectivityManager.activeNetworkState: NetworkState
        get() {
            // Use getActiveNetworkInfo() instead of getNetworkInfo(network) because it can detect VPNs.
            val info = activeNetworkInfo
            val isConnected = info != null && info.isConnected
            val isValidated = isActiveNetworkValidated
            val isMetered = ConnectivityManagerCompat.isActiveNetworkMetered(this)
            val isNotRoaming = info != null && !info.isRoaming
            return NetworkState(isConnected, isValidated, isMetered, isNotRoaming)
        } // b/163342798

    override fun onBroadcastReceive(intent: Intent) {
        @Suppress("DEPRECATION")
        if (intent.action == ConnectivityManager.CONNECTIVITY_ACTION) {
            Log.d(TAG, "Network broadcast received")
            state = connectivityManager.activeNetworkState
        }
    }

    @Suppress("DEPRECATION")
    override val intentFilter: IntentFilter
        get() = IntentFilter(ConnectivityManager.CONNECTIVITY_ACTION)
    override val initialState: NetworkState
        get() = connectivityManager.activeNetworkState
}

private val TAG = ("NetworkStateTrackerPre24")