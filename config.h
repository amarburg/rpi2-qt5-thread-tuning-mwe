#pragma once

#include <Qt>

const static unsigned int CriticalTaskPeriod = 10;  // ms
const static Qt::TimerType CriticalTimerType = Qt::PreciseTimer;

const static unsigned int NumNonCriticalThreads = 8;
const static unsigned int NonCriticalTaskPeriod = 100;  // ms
const static Qt::TimerType NonCriticalTimerType = Qt::CoarseTimer;

const static unsigned int RunFor = 1000; // ms

namespace Tuning {

	const static bool DoFIFOScheduler = true;
	const static bool DoMaxPriority = true;

	const static bool DoMemLock = true;

	const static bool DoRTLimit = true;
	const static unsigned int RTLimit = 4294967295;

	const static bool DoCpuAffinity = true;
	const static unsigned int CriticalCpu = 3;

}
