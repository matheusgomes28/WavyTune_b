#ifndef DATA_STRUCTURES_BYTE_ARRAY_H
#define DATA_STRUCTURES_BYTE_ARRAY_H
#include <vector>
#include <stdexcept>
#include <string>

using Byte = unsigned char;
class ByteArray
{
public:
	ByteArray(std::size_t size);
	ByteArray(Byte* data, std::size_t size);
	ByteArray(const ByteArray& from);
	ByteArray(ByteArray&& from);

	std::size_t get_size() const;
	Byte* get_data();
	void set_data(Byte* data, std::size_t data_size, std::size_t offset = 0);


	ByteArray& operator=(const ByteArray&);
	ByteArray& operator=(ByteArray&&);


private:
	std::vector<Byte> data_;
};

class ByteArrayException : public std::runtime_error
{
public: 
	ByteArrayException(std::string err_message);
	const char* what() const noexcept override;
private:
	std::string message_;
};

#endif // DATA_STRUCTURES_BYTE_ARRAY_H
