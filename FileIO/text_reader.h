#pragma once
#include "abstract_fileio.h"

class TextReader : public AbstractFileIO
{
public:
	TextReader(const std::string& path);
	~TextReader();

	void openFile() override;
	void closeFile() override;

	// Read functions based on the readBytes counterparts

	template <typename T>
	T readData() {

		// Move the data from heap to stack
		T* dataPtr = reinterpret_cast<T*>(readBytes(sizeof(T)));
		T data = std::move(*dataPtr);
		delete dataPtr;

		return data;
	}

	template <typename T>
	T readData(const unsigned int& offset) {

		// Move the data from heap to stack then return
		T* dataPtr = reinterpret_cast<T*>(readBytes(sizeof(T), offset));
		T data = std::move(*dataPtr);
		delete dataPtr;

		return data;
	}

	std::string readFile();

	void seekTo(const unsigned int& offset) override;
	unsigned int tellPos() const override;

	/// Function to return the size of the file. Note that this
	/// uses the seekg functions to go to the end and then 
	/// return the tellp.
	/// @return Size in bytes of the file (supports up to 4GB files).
	unsigned int getFileSize();

	// Maybe override the copy / move stuff

private:
	int getStreamFlags() const override;

	/// Reads *size* bytes from the stream.
	/// @param size The size in bytes to read from stream.
	/// @return Pointer to the bytes allocated in the heap.
	void* readBytes(const unsigned int& size);

	/// Reads *size* bytes from the stream, starting at position
	/// *offset*.
	/// @param size The number of bytes to read form the stream.
	/// @param offset The offsets form the beginning of the file to start
	/// reading form.
	/// @return Pointer to the bytes allocated in the heap.
	void* readBytes(const unsigned int& size, const unsigned int& offset);
};
