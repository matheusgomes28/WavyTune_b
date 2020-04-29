#ifndef DATA_STRUCTURES_BYTE_ARRAY_H
#define DATA_STRUCTURES_BYTE_ARRAY_H
#include <vector>

using Byte = std::size_t;
class ByteArray
{
public:
	ByteArray(std::size_t size);

	Byte* get_data();
	void set_data(Byte* data, std::size_t data_size, std::size_t offset = 0);


private:
	std::vector<Byte> data_;
};

#endif // DATA_STRUCTURES_BYTE_ARRAY_H
