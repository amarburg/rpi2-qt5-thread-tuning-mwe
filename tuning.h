#pragma once

#include <QDebug>

#include <sched.h>
#include <sys/time.h>
#include <sys/resource.h>

#include <unistd.h>
#include <sys/syscall.h>

#include <sys/mman.h>

#include <errno.h>
#include <string.h>


inline bool setScheduler( bool doFIFOScheduler, bool doMaxPriority )
{
	int origSchedPolicy = sched_getscheduler( 0 );
	struct sched_param sp;

	if( sched_getparam( 0, &sp ) != 0 ) {
		qDebug() << "Error getting scheduling parameters (" << errno << "): " << strerror(errno);
		return false;
	}

	struct rlimit rtprio;
	if( getrlimit( RLIMIT_RTPRIO, &rtprio) != 0 ) {
		qDebug() << "Error getting scheduling parameters (" << errno << "): " << strerror(errno);
		return false;
	}

	qDebug() << "rtprio cur " << rtprio.rlim_cur << " max " << rtprio.rlim_max;

	int origPrio = sp.sched_priority;

	int schedPolicy = origSchedPolicy;
	if( doFIFOScheduler ) { schedPolicy = SCHED_FIFO;

		sp.sched_priority = sched_get_priority_min( schedPolicy );
		if( doMaxPriority)
			sp.sched_priority++;
	}

// = sched_get_priority_max(schedPolicy);
// 	else
//

	qDebug() << "Changing sched policy of thread " << syscall(SYS_gettid)
					<< " from policy " << origSchedPolicy << " priority " << origPrio
					<< " to policy " << schedPolicy << " priority " << sp.sched_priority;

	if( sched_setscheduler(0, schedPolicy, &sp) != 0 ) {
		qDebug() << "Error setting scheduling parameters (" << errno << "): " << strerror(errno);
		return false;
	}

	return true;
}
