#include <QCoreApplication>

#include "main_thread.h"
#include "tuning.h"


Q_DECLARE_METATYPE( struct timespec );


int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);
	qRegisterMetaType< struct timespec >();


	if( Tuning::DoMemLock ) {
		// Prevent this process from being swapped out (technically this is
		// moot since we don't use a swapfile on the Pi)
		mlockall(MCL_CURRENT | MCL_FUTURE);
	}

	if( Tuning::DoRTLimit ){
		struct rlimit rlimits;
		getrlimit(RLIMIT_RTTIME, &rlimits );
		qDebug() << "Current RLIMIT_RTTIME = " << rlimits.rlim_cur;
		rlimits.rlim_cur = Tuning::RTLimit;
		if( setrlimit(RLIMIT_RTTIME, &rlimits ) ) {
			qDebug() << "Error setting RLIMIT_RTTIME (" << errno << "): " << strerror(errno);
		}
	}

// Mask out CPU affinity for all processes
if( Tuning::DoCpuAffinity ) {
	cpu_set_t cpu_set;
	if( sched_getaffinity(0, sizeof(cpu_set_t), &cpu_set ) != 0 ) {
		qDebug() << "Unable to get CPU affinity main thread (" << errno << "): " << strerror(errno);
	}

	CPU_CLR( Tuning::CriticalCpu, &cpu_set );

	if( sched_setaffinity(0, sizeof(cpu_set_t), &cpu_set ) != 0 ) {
		qDebug() << "Unable to set CPU affinity for main thread (" << errno << "): " << strerror(errno);
	}

	sched_getaffinity(0, sizeof(cpu_set_t), &cpu_set );
	qDebug() << "Main thread runs on " << (CPU_COUNT( &cpu_set )) << " cpus";
}

	setScheduler( Tuning::DoFIFOScheduler, false );

	MainThread *mt = new MainThread( &app );

	return app.exec();
}
