package ursine.api.timers;

import ursine.api.timers.Timer;

typedef TimerGroupContainer = {
    var timers : Array<Timer>;
    var paused : Bool;
}

class TimerManager {
    private var m_groups : Map<TimerGroup, TimerGroupContainer>;
    private var m_created : Array<Timer>;

    public function new() {
        m_groups = new Map<TimerGroup, TimerGroupContainer>( );
        m_created = new Array<Timer>( );
    }

    public function create(duration : Int, group : TimerGroup = Timer.DefaultGroup) : Timer {
        var timer = new Timer( duration );

        timer.m_group = group;

        var container : TimerGroupContainer = m_groups[ group ];

        if (container == null) {
            container = {
                timers: [ timer ],
                paused: false
            };

            m_groups[ group ] = container;
        } else {
            if (container.paused)
                timer.pause( );

            container.timers.push( timer );
        }

        m_created.push( timer );

        return timer;
    }

    public function isPaused(group : TimerGroup) {
        var container = m_groups[ group ];

        return container == null ? true : container.paused;
    }

    public function pause(group : TimerGroup, force : Bool = false) {
        var container = m_groups[ group ];

        if (container == null)
            return;

        container.paused = true;

        for (timer in container.timers) {
            timer.pause( !force );
        }
    }

    public function resume(group : TimerGroup, force : Bool = false) {
        var container = m_groups[ group ];

        if (container == null)
            return;

        container.paused = false;

        for (timer in container.timers) {
            if (force || timer.m_pausedFromGroup)
                timer.resume( );
        }
    }

    public function pauseAll(force : Bool = false) {
        for (group in m_groups.keys( ))
            pause( group, force );
    }

    public function resumeAll(force : Bool = false) {
        for (group in m_groups.keys( ))
            resume( group, force );
    }

    public function cancelAll() {
        for (timer in m_created)
            timer.cancel( );

        m_groups = new Map<TimerGroup, TimerGroupContainer>( );
        m_created = new Array<Timer>( );
    }

    public function cancel(timer : Timer) {
        var container = m_groups[ timer.m_group ];

        if (container == null)
            return;

        container.timers.remove( timer );

        timer.cancel( );
    }
}