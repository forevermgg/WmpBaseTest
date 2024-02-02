package com.mgg.base.trackers.network

import android.content.Context
import android.net.ConnectivityManager
import android.net.Network
import android.net.NetworkCapabilities
import android.os.Build
import android.util.Log
import androidx.annotation.RequiresApi
import androidx.core.net.ConnectivityManagerCompat
import com.mgg.base.executor.TaskExecutor
import com.mgg.base.trackers.ConstraintTracker

@RequiresApi(Build.VERSION_CODES.N)
internal class NetworkStateTracker24(context: Context, taskExecutor: TaskExecutor) :
    ConstraintTracker<NetworkState>(context, taskExecutor) {

    private val connectivityManager: ConnectivityManager =
        appContext.getSystemService(Context.CONNECTIVITY_SERVICE) as ConnectivityManager
    override val initialState: NetworkState
        get() = connectivityManager.activeNetworkState


    private val ConnectivityManager.isActiveNetworkValidated: Boolean
        get() = try {
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
    internal val ConnectivityManager.activeNetworkState: NetworkState
        get() {
            // Use getActiveNetworkInfo() instead of getNetworkInfo(network) because it can detect VPNs.
            val info = activeNetworkInfo
            val isConnected = info != null && info.isConnected
            val isValidated = isActiveNetworkValidated
            val isMetered = ConnectivityManagerCompat.isActiveNetworkMetered(this)
            val isNotRoaming = info != null && !info.isRoaming
            return NetworkState(isConnected, isValidated, isMetered, isNotRoaming)
        } // b/163342798

    private val networkCallback = object : ConnectivityManager.NetworkCallback() {
        override fun onCapabilitiesChanged(network: Network, capabilities: NetworkCapabilities) {
            // The Network parameter is unreliable when a VPN app is running - use active network.
            Log.d(TAG, "Network capabilities changed: $capabilities")
            connectivityManager.activeNetworkState.also {
                state = it
            }
        }

        override fun onLost(network: Network) {
            Log.d(TAG, "Network connection lost")
            connectivityManager.activeNetworkState.also {
                state = it
            }
        }
    }

    override fun startTracking() {
        try {
            Log.d(TAG, "Registering network callback")
            connectivityManager.registerDefaultNetworkCallbackCompat(networkCallback)
        } catch (e: IllegalArgumentException) {
            // Catching the exceptions since and moving on - this tracker is only used for
            // GreedyScheduler and there is nothing to be done about device-specific bugs.
            // IllegalStateException: Happening on NVIDIA Shield K1 Tablets.  See b/136569342.
            // SecurityException: Happening on Solone W1450.  See b/153246136.
            Log.e(TAG, "Received exception while registering network callback", e)
        } catch (e: SecurityException) {
            Log.e(TAG, "Received exception while registering network callback", e)
        }
    }

    override fun stopTracking() {
        try {
            Log.d(TAG, "Unregistering network callback")
            connectivityManager.unregisterNetworkCallbackCompat(networkCallback)
        } catch (e: IllegalArgumentException) {
            // Catching the exceptions since and moving on - this tracker is only used for
            // GreedyScheduler and there is nothing to be done about device-specific bugs.
            // IllegalStateException: Happening on NVIDIA Shield K1 Tablets.  See b/136569342.
            // SecurityException: Happening on Solone W1450.  See b/153246136.
            Log.e(TAG, "Received exception while unregistering network callback", e)
        } catch (e: SecurityException) {
            Log.e(TAG, "Received exception while unregistering network callback", e)
        }
    }
}

private val TAG = ("NetworkStateTracker24")