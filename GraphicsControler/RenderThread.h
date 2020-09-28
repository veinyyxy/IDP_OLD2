#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include <QThread>
#include <QMutex>

class RenderThread : public QThread
{
	Q_OBJECT
public:
	RenderThread(QObject *parent = 0);

public:
	void startRender();
	void stopRender();

protected:
	virtual void run();

signals:
	void notify();

private:
	bool stop_;
	QMutex mutex;
};

#endif // RENDERTHREAD_H
