#include "stdafx.h"
#include "DCPool.h"


BOOL CALLBACK MonitorEnumProc(
	HMONITOR hMonitor,  // handle to display monitor
	HDC hdcMonitor,        // handle to monitor DC
	LPRECT lprcMonitor,   // monitor intersection rectangle
	LPARAM dwData         // data
	)
{
	RECT rect = *lprcMonitor;
	HDCPool * hdcPool = reinterpret_cast<HDCPool *>(dwData);
	hdcPool->AddHDCToPool(hdcMonitor, rect);
	/*if (rect.left < hdcPool->x_shift)
		hdcPool->x_shift = rect.left;
	if (rect.top < hdcPool->y_shift)
		hdcPool->y_shift = rect.top;*/
	return true;
}

HDCPool::HDCPool()
{
	//x_shift = y_shift = INT_MAX;
	

	HDC hDesktopDC = GetDC(NULL);
	if (!hDesktopDC)
	{
		throw std::runtime_error("CDisplayHandlesPool: GetDC failed");
	}
	if (!EnumDisplayMonitors(hDesktopDC, NULL, MonitorEnumProc, reinterpret_cast<LPARAM>(this)))
	{
		throw std::runtime_error("CDisplayHandlesPool: EnumDisplayMonitors failed");
	}
/*
	logger.open("debug_dc_log.txt",std::ios::app|std::ios::out);
	for(unsigned int i=0; i< m_pool.size(); i++)
	{
		RECT rec = m_pool[i].second;
		logger << "Monitor" << i+1 << ": (" << rec.left << ", " << rec.top << ") => (" << rec.right << ", " << rec.bottom << ")\n";
	}
	logger.close();
	*/
}


HDCPool::~HDCPool()
{
	for (DCPoolType::iterator it = m_pool.begin(); it != m_pool.end(); ++it)
	{
		if (it->first)
			DeleteDC(it->first);
	}
}

void HDCPool::AddHDCToPool(HDC hdc, RECT rect)
{
	m_pool.push_back(std::make_pair(hdc, rect));
	hdc = 0;
}

std::vector<RECT> HDCPool::GetRectangles()
{
	std::vector<RECT> result;
	result.reserve(m_pool.size());
	for(unsigned int i=0; i< m_pool.size(); i++)
	{
		result.push_back(m_pool[i].second);
	}
	return result;
}