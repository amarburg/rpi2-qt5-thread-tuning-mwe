#pragma once

#include <QObject>

#include <unistd.h>

#include "config.h"

class NonCriticalTask : public QObject {
	Q_OBJECT

public:
	NonCriticalTask()
	{;}

public slots:

	void initInThread( void )
	{
		startTimer( NonCriticalTaskPeriod, NonCriticalTimerType );
	}

protected:

	virtual void timerEvent(QTimerEvent *event)
	{
		// Do some non-critical busy work
		for( float j = 0.0; j < 1e5; ++j )  float k = sqrt( j );
	}


};
