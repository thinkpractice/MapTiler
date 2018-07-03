#ifndef BUFFER_CREATOR_H
#define BUFFER_CREATOR_H

#include <vector>

using namespace std;

class BufferFactory
{
public:
	BufferFactory();
	virtual ~BufferFactory();
	
	void* CreateBuffer(size_t bufferSize);
	
private:
	vector<void*> _buffers;
};

#endif