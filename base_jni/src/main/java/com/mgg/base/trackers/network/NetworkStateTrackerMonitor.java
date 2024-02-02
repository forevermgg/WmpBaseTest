package com.mgg.base.trackers.network;

import static com.mgg.base.trackers.network.NetworkStateTrackerKt.NetworkStateTracker;

import android.app.Application;

import com.mgg.base.executor.ArchTaskExecutor;
import com.mgg.base.trackers.ConstraintListener;
import com.mgg.base.trackers.ConstraintTracker;

public class NetworkStateTrackerMonitor implements ConstraintListener<NetworkState> {
    private final Application mApplication;

    private final ConstraintTracker<NetworkState> networkStateTracker;

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

    public void addListener() {
        networkStateTracker.addListener(this);
    }

    public void removeListener() {
        networkStateTracker.removeListener(this);
    }

    public void onConstraintChanged(NetworkState newValue) {
        onConstraintChanged(1, newValue.isConnected());
    }

    public native void onConstraintChanged(int connectType, boolean isConnected);
}
