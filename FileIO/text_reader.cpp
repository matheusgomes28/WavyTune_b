#include "pch.h"
#include "text_reader.h"

TextReader::TextReader(const std::string& path)
	: AbstractFileIO(path)
{}

TextReader::~TextReader() // Nothing was allocated in this derived class
{}

// Returns the correct flags for reading the file (as binary)
int TextReader::getStreamFlags() const
{
	return std::ios_base::in | std::ios_base::binary;
}


// I/O related stuff
void TextReader::openFile()
{
	// Closethe file first
	closeFile();
	getStream()->open(getFilePath(), getStreamFlags());
}

void* TextReader::readBytes(const unsigned int& offset)
{
	char* data = new char[offset];
	getStream()->read(data, offset);
	return data;
}

void* TextReader::readBytes(const unsigned int& size, const unsigned int& offset)
{
	char* data = new char[size];
	seekTo(offset); // go to that offset to read
	getStream()->read(data, size);
	return data;
}

std::string TextReader::readFile()
{
	std::string result = "";
	unsigned int size = getFileSize();
	for (unsigned int i = 0; i < size; i++) {

		char nextChar = readData<char>();
		result += nextChar;
	}
	return result;
}

void TextReader::closeFile()
{
	// Only close if its actually open
	if (getStream()->is_open()) {
		getStream()->close();
	}
}

void TextReader::seekTo(const unsigned int& offset)
{
	getStream()->seekg(offset);
}

unsigned int TextReader::tellPos() const
{
	return getStream()->tellg();
}

unsigned int TextReader::getFileSize()
{
	unsigned int currentOffset = tellPos();
	getStream()->seekg(0, std::ios_base::end);
	unsigned int size = getStream()->tellg();
	seekTo(currentOffset);
	return size;
}