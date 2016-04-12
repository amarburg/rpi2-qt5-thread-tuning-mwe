#pragma once
#include <QCoreApplication>
#include <QFile>
#include <QThread>
#include <QTextStream>
#include <QDebug>

#include "critical_task.h"
#include "non_critical_task.h"

#include "config.h"

class MainThread : public QObject {
	Q_OBJECT
public:
	MainThread( QObject *parent = Q_NULLPTR )
		: QObject( parent ),
			_logFile( "timestamps.txt" ),
			_logStream( &_logFile ),
			_logCount(0)
	{
		if (!_logFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
			qDebug() << "Unable to open timestamp file";
			QCoreApplication::quit();
		}

		_crit = new CriticalTask( );
		_critThread = new QThread( this );
		_crit->moveToThread( _critThread );
		QObject::connect( _critThread, SIGNAL(started()), _crit, SLOT(initInThread()));
		QObject::connect( _critThread, SIGNAL(finished()), _crit, SLOT(deleteLater()));

		QObject::connect( _crit, SIGNAL(timestamp(const struct timespec &)),
											this, SLOT(logTimestamp(const struct timespec &)), Qt::QueuedConnection );

		for( unsigned int i = 0; i < NumNonCriticalThreads; ++i ) {
			_noncrit[i] = new NonCriticalTask( );
			_noncritThreads[i] = new QThread( this );
			_noncrit[i]->moveToThread( _noncritThreads[i] );
			QObject::connect( _noncritThreads[i], SIGNAL(started()), _noncrit[i], SLOT(initInThread()));
			QObject::connect( _noncritThreads[i], SIGNAL(finished()), _noncrit[i], SLOT(deleteLater()));

			_noncritThreads[i]->start();
		}

		_critThread->start();

		startTimer( RunFor, NonCriticalTimerType );

		_logFile.write("sec,nsec\n");
	}

	~MainThread()
	{
		_critThread->quit();
		_critThread->wait();

		for( auto &thread : _noncritThreads ) {
			thread->quit();
			thread->wait();
		}

		delete _crit;
		for( auto &task : _noncrit ) {
			delete task;
		}

		_logFile.close();
	}


public slots:

	void logTimestamp( const struct timespec ts )
	{
		_logStream << ts.tv_sec << ',';
		_logStream.setPadChar('0');
		_logStream.setFieldWidth(9);
		_logStream << ts.tv_nsec;
		_logStream.setFieldWidth(0);
		_logStream << endl;

		_logCount++;
		if( _logCount % 100 == 0 )
			qDebug() << "Logging " << _logCount << " lines";
	}

protected:

	virtual void timerEvent(QTimerEvent *event)
	{
		QCoreApplication::quit();
	}

	QFile _logFile;
	QTextStream _logStream;
	unsigned int _logCount;

	CriticalTask *_crit;
	std::array< NonCriticalTask *, NumNonCriticalThreads > _noncrit;

	QThread *_critThread;
	std::array< QThread *, NumNonCriticalThreads > _noncritThreads;



};
