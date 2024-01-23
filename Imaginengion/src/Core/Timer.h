//////////////////////////////////////////////////////////////////////////////
// Timer.h
// =======
// High Resolution Timer.
// This timer is able to measure the elapsed time with 1 micro-second accuracy
// in both Windows, Linux and Unix system 
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2003-01-13
// UPDATED: 2017-03-30
//
// Copyright (c) 2003 Song Ho Ahn
//////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef TIMER_H_DEF
#define TIMER_H_DEF

#if defined(WIN32) || defined(_WIN32)   // Windows system specific
#include <windows.h>
#else          // Unix based system specific
#include <sys/time.h>
#endif

namespace IM {
    class Timer
    {
    public:
        Timer();                                    // default constructor
        ~Timer();                                   // default destructor

        void   start();                             // start timer
        void   stop();                              // stop the timer

        double getElapsedTimeInMicroSec();          // get elapsed time in micro-second

        // get elapsed time in second (same as getElapsedTime)
        // divide elapsedTimeInMicroSec by 1000000
        double getElapsedTimeInSec() {
            return this->getElapsedTimeInMicroSec() * 0.000001;
        }

        // get elapsed time in milli-second
        // divide elapsedTimeInMicroSec by 1000
        double getElapsedTimeInMilliSec() {
            return this->getElapsedTimeInMicroSec() * 0.001;
        }

        double GetStartTimeInMicroSec() { return startTimeInMicroSec; }
        double GetEndTimeInMicroSec() { return endTimeInMicroSec; }

    private:
        double startTimeInMicroSec;                 // starting time in micro-second
        double endTimeInMicroSec;                   // ending time in micro-second
        int    stopped;                             // stop flag 
    #if defined(WIN32) || defined(_WIN32)
        LARGE_INTEGER frequency;                    // ticks per second
        LARGE_INTEGER startCount;                   //
        LARGE_INTEGER endCount;                     //
    #else
        timeval startCount;                         //
        timeval endCount;                           //
    #endif
    };

    #endif // TIMER_H_DEF
}