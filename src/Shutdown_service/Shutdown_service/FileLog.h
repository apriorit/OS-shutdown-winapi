#pragma once
#include <fstream>

class FileLog
{
private:
	std::ofstream m_output;

public:
	FileLog() {}
	~FileLog();
	bool Open(std::string path);
	void Close();
	void WriteToLog(const std::string& message);
};