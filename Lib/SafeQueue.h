#ifndef SAFE_QUEUE
#define SAFE_QUEUE

#include <queue>
#include <mutex>
#include <condition_variable>

// A threadsafe-queue.
template <class T>
class SafeQueue
{
    public:
        SafeQueue(void)
            : _queue(),
              _mutex(),
              _condition()
        {
        }

        ~SafeQueue(void)
        {
        }

        bool Empty() const
        {
            return _queue.empty();
        }


        // Add an element to the queue.
        void enqueue(T t)
        {
            std::lock_guard<std::mutex> lock(_mutex);
            _queue.push(std::move(t));
            _condition.notify_one();
        }

        // Get the "front"-element.
        // If the queue is empty, wait till a element is avaiable.
        T dequeue(void)
        {
            std::unique_lock<std::mutex> lock(_mutex);
            while(_queue.empty())
            {
                // release lock as long as the wait and reaquire it afterwards.
                _condition.wait(lock);
            }
            T val = std::move(_queue.front());
            _queue.pop();
            return val;
        }

    private:
        std::queue<T> _queue;
        mutable std::mutex _mutex;
        std::condition_variable _condition;
};

#endif
