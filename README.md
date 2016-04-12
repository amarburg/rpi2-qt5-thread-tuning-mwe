## Minimum working example for Qt5 thread performance

We have been developing a data acquisition application on the Raspberry Pi 2 --- heavily biased towards serial comms.  The timing is generally best-effort quasi-realtime.   However, there is one thread (which services some A/D converters) where it would be nice if sample-to-sample timing is reasonably consistent.   

This led me down a path towards getting the best performance for this thread (and the whole application) within the constraints of non-realtime, userland Raspbian Linux.

The application included here is a demonstration and minimum working example.   It uses Qt5 for many of the basic services because that's what we were using in our app, but the specific performance tuning syscalls are reasonably generic.  The various performance strategies can be turned on/off with flags in ``config.h``.

As with all things on the internet, please take with a grain of salt and YMMV.   On the other hand, please feel free to call me out when I've made a terrible mistake.  We're all learning together here, after all.

We were developing on a Raspberry Pi 2, so that's my focus.  No reason this wouldn't work on any generic Linux installation, though some of the configuration details may be different.

## Install

	sudo apt-get install qt5-default g++
	qmake rpi2-qt5-thread-tuning-mwe.pro
	make -j4
	./rpi2-qt5-thread-tuning-mwe

Individual tuning options can be turned on and off in ``config.h``

#### Setting RT_FIFO priorities

In newer OS (in my case, Raspbian 2016-03-18 Jessie Lite) normal users do not have privileges to change their scheduler and priority.   To give a user those permissions, put a file in ``/etc/security/limits.d``:

	pi	soft	rtprio 2

This allows the user ``pi`` to change to the RT_FIFO scheduler and to set scheduler priorities up to 2 (which is one more than the minimum).    Based on my reading of the [man page](http://linux.die.net/man/2/sched_setscheduler):

	If the RLIMIT_RTPRIO soft limit is 0, then the only permitted changes are to lower the priority, or to switch to a non-real-time policy.

it's only really the soft limit which matters.  It must be non-zero to allow changes to the priority, and it sets the maximum priority allowed to the user.

## Analysis

I've written a very short [Jupyter](http://jupyter.org/) / iPython [notebook](https://github.com/amarburg/rpi2-qt5-thread-tuning-mwe/blob/master/timestamp_analysis.ipynb) which does some basic analysis on the data.  It produces some nice plots:

[[https://raw.githubusercontent.com/amarburg/rpi2-qt5-thread-tuning-mwe/master/histogram.png|alt=histograms]]
