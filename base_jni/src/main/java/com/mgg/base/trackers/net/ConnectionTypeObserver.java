package com.mgg.base.trackers.net;

/**
 * Alerted when the connection type of the network changes.
 * The alert is fired on the UI thread.
 */
public interface ConnectionTypeObserver {
    public void onConnectionTypeChanged(int connectionType);
}