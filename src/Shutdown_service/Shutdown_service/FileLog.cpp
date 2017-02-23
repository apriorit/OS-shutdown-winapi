#include "FileLog.h"
#include <thread>
#include <ctime>
#include <stdexcept>

FileLog::~FileLog()
{
	Close();
}

bool FileLog::Open(std::string path)
{
	m_output.open(path, std::fstream::app);
	return m_output.is_open();
}

void FileLog::Close()
{
	if (m_output.is_open())
		m_output.close();
}

void FileLog::WriteToLog(const std::string& message)
{
	if (!m_output.is_open())
		throw std::runtime_error("Log file is closed. Cannot write to log");

	time_t t = time(0);
	struct tm now;
	localtime_s(&now, & t );
	m_output<<now.tm_hour<<":"<<now.tm_min<<":"<<now.tm_sec<<">> "<<"current thread id = "<<std::this_thread::get_id() <<" - "<<message.c_str()<<std::endl;
}