#include "pch.h"
#include "abstract_fileio.h"

AbstractFileIO::AbstractFileIO(const std::string& path)
	: filePath(path),
	stream(new std::fstream(path))
{}

AbstractFileIO::AbstractFileIO(const AbstractFileIO& fromCopy)
	: filePath(fromCopy.filePath),
	stream(new std::fstream(fromCopy.filePath))
{}

AbstractFileIO::AbstractFileIO(AbstractFileIO&& fromMove)
	: filePath(fromMove.filePath),
	stream(fromMove.stream)
{
	fromMove.filePath = "";
	stream = nullptr;
}

AbstractFileIO::~AbstractFileIO()
{
	filePath = "";

	// Close stream and delete it
	if (stream) {
		if (stream->is_open()) {
			stream->close();
		}

		delete stream;
		stream = nullptr;
	}
}

std::string AbstractFileIO::getFilePath() const
{
	return filePath;
}

// IO related stuff
std::fstream* AbstractFileIO::getStream() const
{
	return stream;
}


// perator overloads
AbstractFileIO& AbstractFileIO::operator=(const AbstractFileIO& fromCopy)
{
	this->filePath = fromCopy.filePath;
	this->stream = new std::fstream(fromCopy.filePath);
	return *this;
}

AbstractFileIO&& AbstractFileIO::operator=(AbstractFileIO&& fromMove)
{
	this->filePath = fromMove.filePath;
	fromMove.filePath = "";
	this->stream = fromMove.stream;
	fromMove.stream = nullptr;
	return std::move(*this);
}
