@file:RequiresApi(24)
@file:JvmName("NetworkApi24")

package com.mgg.base.trackers.network

import android.annotation.SuppressLint
import android.net.ConnectivityManager
import android.os.Build
import androidx.annotation.DoNotInline
import androidx.annotation.RequiresApi

@SuppressLint("MissingPermission")
@DoNotInline
@RequiresApi(Build.VERSION_CODES.N)
fun ConnectivityManager.registerDefaultNetworkCallbackCompat(
    networkCallback: ConnectivityManager.NetworkCallback
) = registerDefaultNetworkCallback(networkCallback)