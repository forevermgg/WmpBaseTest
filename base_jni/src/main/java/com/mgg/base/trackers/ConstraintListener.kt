package com.mgg.base.trackers

import androidx.annotation.MainThread

/**
 * The listener for constraint changes.
 *
 * @param T the constraint data type for this listener
 */
interface ConstraintListener<T> {
    /**
     * Called when the value of a constraint has changed.
     * @param newValue the new value of the constraint
     */
    @MainThread
    fun onConstraintChanged(newValue: T)
}