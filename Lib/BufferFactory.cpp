#include "BufferFactory.h"
#include <stdlib.h>

BufferFactory::BufferFactory()
{
}

BufferFactory::~BufferFactory()
{
	for (void* buffer : _buffers)
		free(buffer);
}

void* BufferFactory::CreateBuffer(size_t bufferSize)
{
	void* buffer = malloc(bufferSize);
	_buffers.push_back(buffer);
	return buffer;
}

