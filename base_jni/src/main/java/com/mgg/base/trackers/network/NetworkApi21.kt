@file:RequiresApi(21)
@file:JvmName("NetworkApi21")

package com.mgg.base.trackers.network

import android.annotation.SuppressLint
import android.net.ConnectivityManager
import android.net.ConnectivityManager.NetworkCallback
import android.net.Network
import android.net.NetworkCapabilities
import android.os.Build
import androidx.annotation.DoNotInline
import androidx.annotation.RequiresApi

@DoNotInline
@RequiresApi(Build.VERSION_CODES.LOLLIPOP)
internal fun ConnectivityManager.unregisterNetworkCallbackCompat(
    networkCallback: NetworkCallback
) = unregisterNetworkCallback(networkCallback)

@SuppressLint("MissingPermission")
@DoNotInline
@RequiresApi(Build.VERSION_CODES.LOLLIPOP)
internal fun ConnectivityManager.getNetworkCapabilitiesCompat(
    network: Network?
) = getNetworkCapabilities(network)

@DoNotInline
@RequiresApi(Build.VERSION_CODES.LOLLIPOP)
internal fun NetworkCapabilities.hasCapabilityCompat(capability: Int) = hasCapability(capability)