#ifndef MUTEX_H
#define MUTEX_H

#include <atomic>

// ���ֱ��� mutex
class NullMutex {
public:
	inline void lock() {}
	inline void unlock() {}
};

// ���ɶ�
class SpinLock {
	std::atomic_flag locked = ATOMIC_FLAG_INIT;
public:
	void lock() {
		while (locked.test_and_set(std::memory_order_acquire)) { ; }
	}
	void unlock() {
		locked.clear(std::memory_order_release);
	}
};


#endif

