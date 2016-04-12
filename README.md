## Minimum working example for Qt5 thread performance

We have been developing a data acquisition application on the Raspberry Pi 2 --- heavily biased towards serial comms.  The timing is generally best-effort quasi-realtime.   However, there is one thread (which services some A/D converters) where it would be nice if sample-to-sample timing is reasonably consistent.   

This led me down a path towards getting the best performance for this thread (and the whole application) within the constraints of non-realtime, userland Raspbian Linux.

The application included here is a demonstration and minimum working example.   It uses Qt5 for many of the basic services because that's what we were using in our app, but the specific performance tuning syscalls are reasonably generic.  The various performance strategies can be turned on/off with flags in ``config.h``.

As with all things on the internet, please take with a grain of salt and YMMV.   On the other hand, please feel free to call me out when I've made a terrible mistake.  We're all learning together here, after all.

We were developing on a Raspberry Pi 2, so that's my focus.  No reason this wouldn't work on any generic Linux installation, though some of the configuration details may be different.

## Install

	sudo apt-get install qt5-default g++
	qmake pi2-qt5-thread-tuning-mwe.pro
	make -j4
