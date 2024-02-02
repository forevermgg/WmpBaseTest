package com.mgg.base.trackers

import android.annotation.SuppressLint
import android.content.Context
import android.util.Log
import com.mgg.base.executor.TaskExecutor

/**
 * A base for tracking constraints and notifying listeners of changes.
 *
 * @param T the constraint data type observed by this tracker
 */
abstract class ConstraintTracker<T> protected constructor(
    context: Context,
    private val taskExecutor: TaskExecutor
) {
    protected val appContext: Context = context.applicationContext
    private val lock = Any()
    private val listeners = LinkedHashSet<ConstraintListener<T>>()

    private var currentState: T? = null

    /**
     * Add the given listener for tracking.
     * This may cause [.getInitialState] and [.startTracking] to be invoked.
     * If a state is set, this will immediately notify the given listener.
     *
     * @param listener The target listener to start notifying
     */
    fun addListener(listener: ConstraintListener<T>) {
        synchronized(lock) {
            if (listeners.add(listener)) {
                if (listeners.size == 1) {
                    currentState = initialState
                    Log.d(TAG, "${javaClass.simpleName}: initial state = $currentState")
                    startTracking()
                }
                @Suppress("UNCHECKED_CAST")
                listener.onConstraintChanged(currentState as T)
            }
        }
    }

    /**
     * Remove the given listener from tracking.
     *
     * @param listener The listener to stop notifying.
     */
    fun removeListener(listener: ConstraintListener<T>) {
        synchronized(lock) {
            if (listeners.remove(listener) && listeners.isEmpty()) {
                stopTracking()
            }
        }
    }

    var state: T
        get() {
            return currentState ?: initialState
        }
        @SuppressLint("RestrictedApi")
        set(newState) {
            synchronized(lock) {
                if (currentState != null && (currentState == newState)) {
                    return
                }

                currentState = newState

                // onConstraintChanged may lead to calls to addListener or removeListener.
                // This can potentially result in a modification to the set while it is being
                // iterated over, so we handle this by creating a copy and using that for
                // iteration.
                val listenersList = listeners.toList()
                taskExecutor.executeOnMainThread {
                    listenersList.forEach { listener ->
                        // currentState was initialized by now
                        @Suppress("UNCHECKED_CAST")
                        listener.onConstraintChanged(currentState as T)
                    }
                }
            }
        }

    /**
     * Determines the initial state of the constraint being tracked.
     */
    abstract val initialState: T

    /**
     * Start tracking for constraint state changes.
     */
    abstract fun startTracking()

    /**
     * Stop tracking for constraint state changes.
     */
    abstract fun stopTracking()
}

private val TAG = ("ConstraintTracker")