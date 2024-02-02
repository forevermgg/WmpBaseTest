package com.mgg.base.trackers.network

import android.content.Context
import android.os.Build
import com.mgg.base.executor.TaskExecutor
import com.mgg.base.trackers.ConstraintTracker

/**
 * A [ConstraintTracker] for monitoring network state.
 *
 *
 * For API 24 and up: Network state is tracked using a registered [NetworkCallback] with
 * [ConnectivityManager.registerDefaultNetworkCallback], added in API 24.
 *
 *
 * For API 23 and below: Network state is tracked using a [android.content.BroadcastReceiver].
 * Much less efficient than tracking with [NetworkCallback]s and [ConnectivityManager].
 *
 *
 * Based on [android.app.job.JobScheduler]'s ConnectivityController on API 26.
 * {@see https://android.googlesource.com/platform/frameworks/base/+/oreo-release/services/core/java/com/android/server/job/controllers/ConnectivityController.java}
 */
fun NetworkStateTracker(
    context: Context,
    taskExecutor: TaskExecutor
): ConstraintTracker<NetworkState> {
    // Based on requiring ConnectivityManager#registerDefaultNetworkCallback - added in API 24.
    return if (Build.VERSION.SDK_INT >= 24) {
        NetworkStateTracker24(context, taskExecutor)
    } else {
        NetworkStateTrackerPre24(context, taskExecutor)
    }
}