package ursine.api.timers;

typedef TimerGroup = Int;

@:enum
abstract TimerPauseSource(UInt) {
    public inline var Manager = 0;
    public inline var Client = 1;
}

@:allow(ursine.api.timers.TimerManager)
class Timer {
    public static inline var RepeatInfinite = -1;
    public static inline var DefaultGroup : TimerGroup = 0;

    private var m_group : Int;

    private var m_duration : Int;
    private var m_paused : Bool;
    private var m_cancelled : Bool;

    private var m_pausedFromGroup : Bool;

    private var m_repeat : Int;

    private var m_lastResumed : Float;
    private var m_lastPaused : Float;
    private var m_remainingDuration : Float;

    private var m_handle : haxe.Timer;

    private var m_repeatCallback : Dynamic;
    private var m_completeCallback : Dynamic;

    private function new(duration : Int) {
        m_duration = duration;

        m_paused = false;
        m_cancelled = false;

        m_pausedFromGroup = false;

        m_lastResumed = m_lastPaused = haxe.Timer.stamp( );

        m_repeat = 0;
        m_repeatCallback = null;
        m_completeCallback = null;

        m_handle = new haxe.Timer( duration );
        m_handle.run = tickFromRepeat;
    }

    public function isPaused() {
        return m_paused;
    }

    public function pause(fromGroup : Bool = false) {
        if (m_paused)
            return;

        m_handle.stop( );
        m_lastPaused = haxe.Timer.stamp( );
        m_remainingDuration = m_duration - (m_lastPaused - m_lastResumed);

        m_paused = true;
        m_pausedFromGroup = fromGroup;
    }

    public function resume() {
        if (!m_paused)
            return;

        m_handle = new haxe.Timer( cast m_remainingDuration );
        m_handle.run = tickFromPause;

        m_paused = false;
    }

    public function isCanceled() {
        return m_cancelled;
    }

    public function cancel() {
        m_handle.stop( );

        m_cancelled = true;
    }

    public function repeat(count : Int) {
        m_repeat = count;

        return this;
    }

    public function onRepeat(callback : Dynamic) {
        m_repeatCallback = callback;

        return this;
    }

    public function onComplete(callback : Dynamic) {
        m_completeCallback = callback;

        return this;
    }

    public function getElapsed() : Float {
        if (m_paused) {
            return m_lastPaused - m_lastResumed;
        } else {
            return haxe.Timer.stamp( ) - m_lastResumed;
        }
    }

    private function tickFromPause() {
        onTimerComplete( );

        m_handle.stop( );

        m_handle = new haxe.Timer( m_duration );
        m_handle.run = tickFromRepeat;
    }

    private function tickFromRepeat() {
       onTimerComplete( );
    }

    private function onTimerComplete() {
        if (m_repeatCallback)
            m_repeatCallback( );

        // we're repeating forever
        if (m_repeat == RepeatInfinite)
            return;

        if (--m_repeat <= 0) {
            if (m_completeCallback)
                m_completeCallback( );

            cancel( );
        }
    }
}