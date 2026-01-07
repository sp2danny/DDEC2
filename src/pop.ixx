
export module pop;

#ifdef __cpp_contracts
  #define Assert(x) contract_assert(c)
#else
  import <cassert>;
  #define Assert(x) assert(x)
#endif

import std;

template<typename TT, typename T>
concept PointerAssignable = requires(TT * ttp, T * tp) {
	{ tp = ttp };
};

template<typename TT, typename T>
concept DestructCompatible =
PointerAssignable<TT, T> && (
	std::is_same_v<const T, const TT> ||
	(std::is_base_of_v<T, TT> && std::has_virtual_destructor_v<T>)
	);

template<typename TT, typename T>
concept PointerCompatible =
PointerAssignable<TT, T> && (
	std::is_same_v<const T, const TT> || std::is_base_of_v<T, TT>
	);

export struct to_own {};
export struct to_borrow {};
export struct to_create {};

export
template<typename T>
class pop // Potentially Owning Pointer
{
	constexpr static inline bool UnderlyingNoexceptCompare = requires(const T & t)
	{
		{ t <=> t } noexcept;
	};
	constexpr static inline bool UnderlyingNoexceptDestructor = requires(T & t)
	{
		{ t.~T() } noexcept;
	};

public:

	using deleter_f = void (*)(T*) noexcept;

	pop() noexcept = default;

	template<PointerCompatible<T> TT>
	pop(to_borrow, TT* p) noexcept
		: pop()
	{
		borrow(p);
	}

	template<DestructCompatible<T> TT>
	pop(to_own, TT* p) noexcept
		: pop()
	{
		own(p);
	}

	template<typename... Args>
	pop(to_create, Args&& ...args) noexcept
		: pop()
	{
		create<T>(std::forward<Args>(args)...);
	}

	pop(const pop& other)
		: pop()
	{
		(*this) = other;
	}

	pop(pop&& other) noexcept
		: pop()
	{
		swap(other);
	}

	pop& operator=(const pop& other)
	{
		clear();
		if (!other.m_ptr) return *this;
		m_ptr = other.m_ptr;
		if (!other.m_own) return *this;
		m_del = other.m_del;
		if (other.m_ref) {
			m_ref = other.m_ref;
			++*m_ref;
		}
		else {
			m_ref = new std::atomic<int>(2);
			other.m_ref = m_ref;
		}
		m_own = true;
		return *this;
	}

	pop& operator=(pop&& other) noexcept(UnderlyingNoexceptDestructor)
	{
		clear();
		swap(other);
		return *this;
	}

	void swap(pop& other) noexcept
	{
		using std::swap;
		swap(m_ptr, other.m_ptr);
		swap(m_own, other.m_own);
		swap(m_del, other.m_del);
		swap(m_ref, other.m_ref);
	}

	template<PointerCompatible<T> TT>
	void borrow(TT* p) noexcept(UnderlyingNoexceptDestructor)
	{
		clear();
		m_ptr = p;
		m_own = false;
	}

	template<PointerCompatible<T> TT>
	void borrow(TT& p) noexcept(UnderlyingNoexceptDestructor)
	{
		clear();
		m_ptr = &p;
		m_own = false;
	}

	template<DestructCompatible<T> TT>
	void own(TT* p) noexcept(UnderlyingNoexceptDestructor)
	{
		clear();
		m_ptr = p;
		m_own = true;
	}

	template<DestructCompatible<T> TT>
	void own(TT* p, deleter_f del) noexcept(UnderlyingNoexceptDestructor)
	{
		clear();
		m_ptr = p;
		m_own = true;
		m_del = del;
	}

	bool have() const noexcept { return m_ptr != nullptr; }

	explicit operator bool() const noexcept { return have(); }

	void clear() noexcept(UnderlyingNoexceptDestructor)
	{
		//#pragma clang diagnostic push
		//#pragma clang diagnostic ignored "-Wdelete-abstract-non-virtual-dtor"
		if (m_ptr && m_own) {
			if (m_ref) {
				auto val = -- * m_ref;
				if (!val) {
					delete m_ref;
					if (m_del)
						m_del(m_ptr);
					else
						delete m_ptr;
				}
			}
			else {
				if (m_del)
					m_del(m_ptr);
				else
					delete m_ptr;
			}
		}
		//#pragma clang diagnostic pop
		m_ptr = nullptr;
		m_own = false;
		m_del = nullptr;
		m_ref = nullptr;
	}

	template<typename TT = T, typename... Args>
		requires(DestructCompatible<TT, T>)
	void create(Args&&... args)
	{
		clear();
		m_ptr = new TT(std::forward<Args>(args)...);
		if (!m_ptr)
			throw std::bad_alloc{};
		m_own = true;
	}

	T* get() const noexcept
	{
		return m_ptr;
	}

	T& operator*() const
	{
		Assert(have());
		return *m_ptr;
	}

	T* operator->() const
	{
		Assert(have());
		return m_ptr;
	}

	~pop() noexcept(UnderlyingNoexceptDestructor)
	{
		clear();
	}

	auto operator<=>(const pop& other) const noexcept(UnderlyingNoexceptCompare)
		requires requires(const T& a, const T& b) {
			{ std::compare_strong_order_fallback(a, b) };
	}
	{
		bool h1 = have();
		bool h2 = other.have();
		if (h1 && h2)
			return std::compare_strong_order_fallback(**this, *other);
		else
			return h1 <=> h2;
	}
	bool operator==(const pop& other) const noexcept(UnderlyingNoexceptCompare)
		requires requires(const T& a, const T& b) {
			{ std::compare_strong_order_fallback(a, b) };
	}
	{
		return std::is_eq(*this <=> other);
	}

private:
	T* m_ptr = nullptr;
	bool m_own = false;
	deleter_f m_del = nullptr;
	mutable std::atomic<int>* m_ref = nullptr;
};


