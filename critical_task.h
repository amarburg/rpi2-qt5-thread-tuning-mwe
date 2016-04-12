#pragma once

#include <QObject>

#include <time.h>

#include "config.h"
#include "tuning.h"

class CriticalTask : public QObject {
	Q_OBJECT

public:
	CriticalTask()
	{;}

	~CriticalTask()
	{;}

public slots:
	void initInThread( void )
	{

		// Attempt to set CPU affinity
	  if( Tuning::DoCpuAffinity ) {
	    qDebug() << "Setting CPU affinity for critical task...";

	    cpu_set_t cpu_set;
	    CPU_ZERO( &cpu_set );
	    CPU_SET( Tuning::CriticalCpu, &cpu_set );
	    if( sched_setaffinity(0, sizeof(cpu_set_t), &cpu_set ) != 0 ) {
	      qDebug() << "Unable to set CPU affinity for critical task (" << errno << "): " << strerror(errno);
	    }

	    sched_getaffinity(0, sizeof(cpu_set_t), &cpu_set );
	    qDebug() << "Critical thread runs on " << CPU_COUNT( &cpu_set ) << " cpus";

	  }

		// Change the scheduler in this thread...
		setScheduler( Tuning::DoFIFOScheduler, Tuning::DoMaxPriority );

		startTimer( CriticalTaskPeriod, CriticalTimerType );
	}

signals:

	void timestamp( const struct timespec &ts );

protected:

	virtual void timerEvent(QTimerEvent *event)
	{
		struct timespec ts;
		clock_gettime( CLOCK_MONOTONIC, &ts );

		// Do something to time critical.
		for( unsigned int i = 0; i < 1e4; ++i ) {
			struct timespec ts;
			clock_gettime( CLOCK_MONOTONIC, &ts );
		}

		emit timestamp( ts );
	}

};
