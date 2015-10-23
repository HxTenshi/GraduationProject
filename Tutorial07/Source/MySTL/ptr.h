#pragma once
#ifndef NULL
#define NULL 0
#endif

template<class T >
class enable_shared_from_this;
template<class T >
class sp;

template< class X, class Y, class T >
void sp_enable_shared_from_this(
	sp<X> const * ppx,
	Y const * py,
	enable_shared_from_this< T > const * pe);

void sp_enable_shared_from_this(...);

template < typename T >
struct shared_ptr_traits
{
	typedef T& reference;
};
template<>
struct shared_ptr_traits< void >
{
	typedef void reference;
};
class sp_counter_base{
public:
	sp_counter_base()
		: sp_count(1)
		, wp_count(1)
	{ }
	virtual ~sp_counter_base()
	{ }

	bool addref()
	{
		if (sp_count == 0)return false;

		sp_count++;
		return true;
	}
	void release()
	{
		if (--sp_count == 0)
		{
			dispose();
			weak_release();
		}
	}

	void weak_addref()
	{
		wp_count++;
	}

	void weak_release()
	{
		if (--wp_count == 0)
		{
			destroy();
		}
	}

	void destroy()
	{
		delete this;
	}

	bool enable_sp_count(){
		return sp_count>0;
	}

	virtual void dispose() = 0;

private:
	sp_counter_base(sp_counter_base const&);
	sp_counter_base& operator=(sp_counter_base const&);

private:
	int sp_count;
	int wp_count;
};

template<typename T>
class sp_counter_impl : public sp_counter_base{
public:
	sp_counter_impl(T* ptr)
		: ptr(ptr)
	{ }
	~sp_counter_impl()
	{ }

	void dispose() override
	{
		delete ptr;
	}
private:
	T* ptr;
};

class wp_count;
template<typename U>
class wp;
class sp_count{
public:
	sp_count();
	template<typename T>
	sp_count(T* ptr)
		: pimpl(new sp_counter_impl<T>(ptr))
	{ }
	sp_count(sp_count const& sc);
	sp_count& operator=(sp_count const& sc);
	sp_count(wp_count const& c);
	~sp_count();
private:
	sp_counter_base* pimpl;

	friend class wp_count;
};

template<typename T>
class sp{
public:

	sp()
		: ptr(NULL)
		, count()
	{ }

	template<typename U>
	sp(U* a)
		: ptr((T*)a)
		, count(a)
	{
		sp_enable_shared_from_this(this, a, a);
	}

	template <typename U>
	sp(const sp<U>& sp)
		: ptr((T*)sp.ptr)
		, count(sp.count)
	{ }

	template <typename U>
	sp(const wp<U>& wp)
		: ptr((T*)wp.ptr)
		, count(wp.count)
	{ }

	~sp()
	{ }

	T* Get(){
		return (T*)ptr;
	}
	T* Get()const{
		return (T*)ptr;
	}

	inline
	operator bool()const{
		return ptr != NULL;
	}


	typedef typename shared_ptr_traits< T >::reference reference;
	inline
	reference operator* ()const{
		return (reference)*ptr;
	}

	inline
	T* operator-> ()const{
		return (T*)ptr;
	}
private:
	T* ptr;
	sp_count count;

	template<typename U>
	friend class wp;

	template<typename U>
	friend class sp;

};

class wp_count{
public:
	wp_count()
		: pimpl(NULL)
	{}

	wp_count(sp_count const& c)
		: pimpl(c.pimpl)
	{
		if (pimpl)
			pimpl->weak_addref();
	}

	wp_count(wp_count const& c)
		: pimpl(c.pimpl)
	{
		if (pimpl)
			pimpl->weak_addref();
	}

	~wp_count()
	{
		if (pimpl)
			pimpl->weak_release();
	}

	bool expired(){
		return pimpl == NULL || !pimpl->enable_sp_count();
	}

	wp_count& operator=(sp_count const& c)
	{
		if (c.pimpl)
		{
			if (c.pimpl)c.pimpl->weak_addref();
			if (pimpl)pimpl->weak_release();
			pimpl = c.pimpl;
		}
		return *this;
	}

	wp_count& operator=(wp_count const& c)
	{
		if (c.pimpl)
		{
			if (c.pimpl)c.pimpl->weak_addref();
			if (pimpl)pimpl->weak_release();
			pimpl = c.pimpl;
		}
		return *this;
	}

private:
	sp_counter_base* pimpl;

	friend class sp_count;
};

template<class T>
class enable_shared_from_this;
template< typename T >
class wp
{
public:
	typedef typename shared_ptr_traits<T>::reference reference;

public:
	wp()
		: ptr(NULL)
		, count()
	{}

	template<typename U>
	wp(wp<U> const& p)
		: ptr(p.lock().get())
		, count(p.count)
	{
	}

	template<typename U>
	wp(sp<U> const& p)
		: ptr(p.ptr)
		, count(p.count)
	{
	}

	template<typename U>
	wp& operator=(wp<U> const& p)
	{
		ptr = p.lock().get();
		count = p.count;
		return *this;
	}

	template<typename U>
	wp& operator=(sp<U> const& p)
	{
		ptr = p.ptr;
		count = p.count;
		return *this;
	}

	sp<T> lock()const
	{
		//if (count.expired()) return sp<T>();
		return sp<T>(*this);
	}

	operator bool(){
		return !count.expired();
	}

	reference operator*()const
	{
		return *ptr;
	}

	T* operator->()const
	{
		return ptr;
	}


private:
	T* ptr;
	wp_count count;

	template<typename U>
	friend class sp;
	template<class T>
	friend class enable_shared_from_this;
};

template <typename T>
using shared_ptr = sp < T > ;

template <typename T>
using weak_ptr = wp < T > ;



template<class T>
class enable_shared_from_this{
protected:

	enable_shared_from_this()
	{
	}
	enable_shared_from_this(enable_shared_from_this const &)
	{
	}
	enable_shared_from_this & operator=(enable_shared_from_this const &)
	{
		return *this;
	}
	~enable_shared_from_this()
	{
	}

public:
	shared_ptr<T> shared_from_this()
	{
		shared_ptr<T> p(weak_this_);
		return p;
	}
	shared_ptr<T const> shared_from_this() const
	{
		shared_ptr<T const> p(weak_this_);
		return p;
	}

public: // actually private, but avoids compiler template friendship issues
	// Note: invoked automatically by shared_ptr; do not call
	template<class X, class Y> void
		_internal_accept_owner(shared_ptr<X> const * ppx, Y * py) const
	{
		if (weak_this_.count.expired())
		{
			weak_this_ = shared_ptr<T>(*ppx);
		}
	}

private:
	mutable weak_ptr<T> weak_this_;
};


template< class X, class Y, class T >
inline void sp_enable_shared_from_this(
	shared_ptr<X> const * ppx,
	Y const * py,
	enable_shared_from_this< T > const * pe)
{
	if (pe != 0)
	{
		pe->_internal_accept_owner(ppx, const_cast< Y* >(py));
	}
}

inline void sp_enable_shared_from_this(...)
{
}