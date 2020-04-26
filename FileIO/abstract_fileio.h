#pragma once
#include "pch.h"


class AbstractFileIO
{
public:

	AbstractFileIO(const std::string& path);
	AbstractFileIO(const AbstractFileIO& fromCopy);
	AbstractFileIO(AbstractFileIO&& fromMove);
	~AbstractFileIO();

	virtual void openFile() = 0;
	virtual void closeFile() = 0;

	virtual void seekTo(const unsigned int& offset) = 0;
	virtual unsigned int tellPos() const = 0;

	std::string getFilePath() const;

	// Copy / Move constructors
	AbstractFileIO& operator=(const AbstractFileIO& fromCopy);
	AbstractFileIO&& operator=(AbstractFileIO&& fromMove);

protected:
	std::fstream* getStream() const;

private:
	std::string filePath;
	std::fstream* stream;

	virtual int getStreamFlags() const = 0;
};

