package com.mgg.base.trackers.network;

import static com.mgg.base.trackers.network.NetworkStateTrackerKt.NetworkStateTracker;

import android.app.Application;

import com.mgg.base.trackers.ConstraintListener;
import com.mgg.base.trackers.ConstraintTracker;

public class NetworkStateTrackerMonitor implements ConstraintListener<NetworkState> {
    private Application mApplication;

    private ConstraintTracker<NetworkState> networkStateTracker;
    public NetworkStateTrackerMonitor(Application application) {
        mApplication = application;
        networkStateTracker = NetworkStateTracker(application, ArchTaskExecutor.getInstance());
    }

    public NetworkState currentState() {
        return networkStateTracker.getState();
    }

    public Boolean isLanConnected() {
        return networkStateTracker.getState().isConnected();
    }

    public Boolean addListener()  {
        networkStateTracker.addListener(this);
        return true;
    }

    public Boolean removeListener() {
        networkStateTracker.removeListener(this);
        return true;
    }

    public void onConstraintChanged(NetworkState newValue) {
        onConstraintChanged(1, newValue.isConnected());
    }

    public native void onConstraintChanged(int connectType, boolean isConnected);
}
