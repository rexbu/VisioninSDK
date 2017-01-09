package com.rex.utils;

import java.util.Timer;
import java.util.TimerTask;

/**
 * Created by Rex on 14-5-20.
 */
public abstract  class TimerIOCallback extends IOCallback{
    private Timer       timer;
    private TimerTask   task;

    public TimerIOCallback(int length, int split){
        super(length, split);
        timer = new Timer();
        task = new TimerTask() {
            @Override
            public void run() {
                onRunning();
            }
        };
        // 立即开始，每隔split执行一次
        timer.schedule(task, 0, split);
    }
    public void stop(){
        timer.cancel();
        timer = null;
        task = null;
    }

}
