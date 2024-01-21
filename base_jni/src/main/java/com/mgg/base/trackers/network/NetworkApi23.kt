@file:RequiresApi(23)
@file:JvmName("NetworkApi23")

package com.mgg.base.trackers.network

import android.annotation.SuppressLint
import android.net.ConnectivityManager
import android.os.Build
import androidx.annotation.DoNotInline
import androidx.annotation.RequiresApi

@SuppressLint("MissingPermission")
@DoNotInline
@RequiresApi(Build.VERSION_CODES.M)
fun ConnectivityManager.getActiveNetworkCompat() = activeNetwork