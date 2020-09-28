#include "RenderThread.h"


RenderThread::RenderThread(QObject *parent) : QThread(parent)
{
	stop_ = true;
}

void RenderThread::startRender()
{
	try
	{
		stop_ = false;
		start(HighestPriority);
	}
	catch (...)
	{

	}
}

void RenderThread::stopRender()
{
	try
	{
		mutex.lock();
		stop_ = true;
		mutex.unlock();
	}
	catch (...)
	{

	}
}

void RenderThread::run()
{
	try
	{
		forever
		{
			mutex.lock();

			if(stop_) 
			{
				stop_ = false;
				mutex.unlock();                        
				break;
			}


			emit notify();
			msleep(20);
			mutex.unlock();
		}
	}

	catch (...)
	{

	}
}
