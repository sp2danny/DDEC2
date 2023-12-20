
#pragma once

#include <utility>
#include <cassert>
#include <type_traits>
#include <concepts>

template<typename T, typename TT>
concept DestructCompatible = 
	std::is_same_v<T, TT> ||
	(std::is_base_of_v<T, TT> && std::has_virtual_destructor_v<T>);

template<typename T>
class pop
{
public:
	pop() = default;

	template<typename TT>
		requires(DestructCompatible<T, TT>)
	void borrow(TT* p)
	{
		release();
		m_ptr = p;
		m_own = false;
	}

	template<typename TT>
		requires(DestructCompatible<T, TT>)
	void own(TT* p)
	{
		release();
		m_ptr = p;
		m_own = true;
	}

	bool have() const { return m_ptr != nullptr; }

	explicit operator bool() const { return have(); }

	void release()
	{
		if (m_ptr && m_own)
			delete m_ptr;
		m_ptr = nullptr;
		m_own = false;
	}

	template<typename TT=T, typename... Args>
		requires(DestructCompatible<T, TT>)
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

