
#pragma once

#include <utility>
#include <cassert>
#include <type_traits>
#include <compare>

template<typename T, typename TT>
concept DestructCompatible = 
	std::is_same_v<T, TT> ||
	(std::is_base_of_v<T, TT> && std::has_virtual_destructor_v<T>);

template<typename T>
concept Clonable = requires(T* p1, T* p2) { p1 = clone(p2); };

template<typename T>
class pop
{
public:
	pop() = default;

	pop(const pop& other) requires Clonable<T>
		: pop()
	{
		(*this) = other;
	}

	pop(pop&& other)
		: pop()
	{
		swap(other);
	}
	
	pop& operator=(const pop& other) requires Clonable<T>
	{
		release();
		if (!other.m_ptr)
		{
			m_ptr = nullptr;
			m_own = false;
		}
		else if (!other.m_own)
		{
			m_ptr = other.m_ptr;
			m_own = false;
		}
		else
		{
			m_ptr = clone(other.m_ptr);
			m_own = true;
		}
		return *this;
	}
	
	pop& operator=(pop&& other)
	{
		release();
		swap(other);
		return *this;
	}

	void swap(pop& other)
	{
		using std::swap;
		swap(m_ptr, other.m_ptr);
		swap(m_own, other.m_own);	
	}

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
	
	T* get() const
	{
		return m_ptr;
	}

	T& operator*() const
	{
		assert(have());
		return *m_ptr;
	}

	T* operator->() const
	{
		assert(have());
		return m_ptr;
	}

	~pop() { release(); }
	
	auto operator<=>(const pop& other) const
		requires std::three_way_comparable<T>
	{
		bool h1 = have();
		bool h2 = other.have();
		if (h1&&h2)
			return (**this) <=> (*other);
		else
			return h1 <=> h2;
	}
	bool operator==(const pop& other) const
	{
		return std::is_eq(*this <=> other);
	}

private:
	T* m_ptr = nullptr;
	bool m_own = false;
};

