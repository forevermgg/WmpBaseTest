package com.mgg.base.trackers.network

import android.annotation.SuppressLint
import android.content.Context
import android.content.Context.CONNECTIVITY_SERVICE
import android.net.ConnectivityManager

/**
 * Checks if a network connection exists.
 */
open class NetworkUtils(private val context: Context) {

    @SuppressLint("MissingPermission")
    @Suppress("DEPRECATION")
    open fun hasNetworkConnection(): Boolean {
        val connectivityManager =
            context.getSystemService(CONNECTIVITY_SERVICE) as ConnectivityManager
        val activeNetworkInfo = connectivityManager.activeNetworkInfo
        return activeNetworkInfo != null && activeNetworkInfo.isConnected
    }

    @Suppress("DEPRECATION")
    fun hasNetwork(): Boolean {
        val cm = context.getSystemService(CONNECTIVITY_SERVICE) as ConnectivityManager

        val activeNetwork = cm.activeNetworkInfo
        return activeNetwork != null && activeNetwork.isConnectedOrConnecting
    }
}