
#pragma once

#include <mutex>
#include <chrono>
#include <thread>

template<typename T>
class ReadWriteLock
{

	struct ReadLock {
		T read();
		ReadLock(ReadLock&&) = default;
		~ReadLock();
	private:
		ReadLock() = delete;
		ReadLock(const ReadLock&) = delete;
		ReadLock(ReadWriteLock&);
		ReadWriteLock& owner;
	friend
		class ReadWriteLock;
	};

	struct WriteLock {
		void write(const T&);
		WriteLock(WriteLock&&) = default;
		~WriteLock();
	private:
		WriteLock() = delete;
		WriteLock(const WriteLock&) = delete;
		WriteLock(ReadWriteLock&);
		ReadWriteLock& owner;
	friend
		class ReadWriteLock;
	};


	void readerDown();
	void writerDown();

public:
	ReadWriteLock();
	ReadWriteLock(const T&);

	ReadLock getReader();
	WriteLock getWriter();

private:

	std::mutex mx;
	int readers = 0;
	bool writer = false;
	int writers_waiting = 0;

	T value;
};


template<typename T>
T ReadWriteLock<T>::ReadLock::read()
{
	return owner.value;
}

template<typename T>
ReadWriteLock<T>::ReadLock::~ReadLock()
{
	owner.readerDown();
}

template<typename T>
ReadWriteLock<T>::ReadLock::ReadLock(ReadWriteLock& owner)
	: owner(owner)
{}

template<typename T>
void ReadWriteLock<T>::readerDown()
{
	std::lock_guard lock(mx);
	--readers;
}



template<typename T>
void ReadWriteLock<T>::WriteLock::write(const T& val)
{
	owner.value = val;
}

template<typename T>
ReadWriteLock<T>::WriteLock::~WriteLock()
{
	owner.writerDown();
}

template<typename T>
ReadWriteLock<T>::WriteLock::WriteLock(ReadWriteLock& owner)
	: owner(owner)
{}

template<typename T>
void ReadWriteLock<T>::writerDown()
{
	std::lock_guard lock(mx);
	writer = false;
}





template<typename T>
auto ReadWriteLock<T>::getReader() -> ReadLock
{
	while (true)
	{
		{
			std::lock_guard lock(mx);
			// a reader can get a lock if there are no write locks or waiting writers
			if (!(writer || writers_waiting))
			{
				++readers;
				return {*this};
			}
		}

		std::this_thread::sleep_for(1ms);
	}
}

template<typename T>
auto ReadWriteLock<T>::getWriter() -> WriteLock
{
	{
		std::lock_guard lock(mx);
		++writers_waiting;
	}

	while (true)
	{
		{
			std::lock_guard lock(mx);
			// a writer can get a lock if there are no locks at all
			if (!(writer || readers))
			{
				writer = true;
				--writers_waiting;
				return {*this};
			}
		}

		std::this_thread::sleep_for(1ms);
	}
}

template<typename T>
ReadWriteLock<T>::ReadWriteLock()
{
}

template<typename T>
ReadWriteLock<T>::ReadWriteLock(const T& initial)
	: value(initial)
{
}

