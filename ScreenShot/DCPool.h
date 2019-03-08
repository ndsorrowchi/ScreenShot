#pragma once
#include <vector>
#include <fstream>

BOOL CALLBACK MonitorEnumProc(
	HMONITOR hMonitor,  // handle to display monitor
	HDC hdcMonitor,     // handle to monitor DC
	LPRECT lprcMonitor, // monitor intersection rectangle
	LPARAM dwData       // data
	);

typedef std::vector<std::pair<HDC, RECT>> DCPoolType;

struct HDCPool
{
	DCPoolType m_pool;
	HDCPool();
	~HDCPool();
	void AddHDCToPool(HDC hdc, RECT rect);
	std::vector<RECT> GetRectangles();
	//int x_shift;
	//int y_shift;
	std::fstream logger;
};

