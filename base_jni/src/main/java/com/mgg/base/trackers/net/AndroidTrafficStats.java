package com.mgg.base.trackers.net;

import android.net.TrafficStats;
import android.os.Process;

/** This class interacts with TrafficStats API provided by Android. */
public class AndroidTrafficStats {
    private AndroidTrafficStats() {}

    /**
     * @return Number of bytes transmitted since device boot. Counts packets across all network
     *         interfaces, and always increases monotonically since device boot. Statistics are
     *         measured at the network layer, so they include both TCP and UDP usage.
     */
    private static long getTotalTxBytes() {
        long bytes = TrafficStats.getTotalTxBytes();
        return bytes != TrafficStats.UNSUPPORTED ? bytes : TrafficStatsError.ERROR_NOT_SUPPORTED.ordinal();
    }

    /**
     * @return Number of bytes received since device boot. Counts packets across all network
     *         interfaces, and always increases monotonically since device boot. Statistics are
     *         measured at the network layer, so they include both TCP and UDP usage.
     */
    private static long getTotalRxBytes() {
        long bytes = TrafficStats.getTotalRxBytes();
        return bytes != TrafficStats.UNSUPPORTED ? bytes : TrafficStatsError.ERROR_NOT_SUPPORTED.ordinal();
    }

    /**
     * @return Number of bytes transmitted since device boot that were attributed to caller's UID.
     *         Counts packets across all network interfaces, and always increases monotonically
     *         since device boot. Statistics are measured at the network layer, so they include
     *         both TCP and UDP usage.
     */
    private static long getCurrentUidTxBytes() {
        long bytes = TrafficStats.getUidTxBytes(Process.myUid());
        return bytes != TrafficStats.UNSUPPORTED ? bytes : TrafficStatsError.ERROR_NOT_SUPPORTED.ordinal();
    }

    /**
     * @return Number of bytes received since device boot that were attributed to caller's UID.
     *         Counts packets across all network interfaces, and always increases monotonically
     *         since device boot. Statistics are measured at the network layer, so they include
     *         both TCP and UDP usage.
     */
    private static long getCurrentUidRxBytes() {
        long bytes = TrafficStats.getUidRxBytes(Process.myUid());
        return bytes != TrafficStats.UNSUPPORTED ? bytes : TrafficStatsError.ERROR_NOT_SUPPORTED.ordinal();
    }
}
