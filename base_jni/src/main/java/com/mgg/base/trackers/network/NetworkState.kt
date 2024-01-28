package com.mgg.base.trackers.network

/**
 * Stores information about network state.
 */
data class NetworkState(
    /**
     * Determines if the network is connected.
     */
    val isConnected: Boolean,

    /**
     * Determines if the network is validated - has a working Internet connection.
     */
    val isValidated: Boolean,

    /**
     * Determines if the network is metered.
     */
    val isMetered: Boolean,

    /**
     * Determines if the network is not roaming.
     */
    val isNotRoaming: Boolean
)
