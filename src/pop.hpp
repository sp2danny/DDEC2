
#pragma once

#include <utility>
#include <cassert>
#include <type_traits>

template<typename T>
class pop
{
public:
	pop() = default;
	
	void borrow(T* p)
	{
		release();
		m_ptr = p;
		m_own = false;
	}
	
	void own(T* p)
	{
		release();
		m_ptr = p;
		m_own = false;
	}
	
	bool have() const { return m_ptr != nullptr; }
	void release()
	{
		if (m_ptr && m_own)
			delete m_ptr;
		m_ptr = nullptr;
		m_own = false;
	}
	
	template<typename TT=T, typename... Args> requires(std::is_base_of_v<T,TT>)
	void create(Args&&... args)
	{
		release();
		m_ptr = new TT(std::forward<Args>(args)...);
		m_own = true;
	}
	
	T& operator*()
	{
		assert(m_ptr);
		return *m_ptr;
	}
	
	T* operator->()
	{
		assert(m_ptr);
		return m_ptr;
	}
	
	~pop() { release(); }

private:
	T* m_ptr = nullptr;
	bool m_own = false;
};

