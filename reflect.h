#include <typeinfo>

using std::type_info;

template< typename... Args >
std::string string_sprintf(const char* format, Args... args)
{
	int length = std::snprintf(nullptr, 0, format, args...);
	assert(length >= 0);

	char* buf = new char[length + 1];
	std::snprintf(buf, length + 1, format, args...);

	std::string str(buf);
	delete[] buf;
	return std::move(str);
}

template<typename base_t>
static auto GetFunctionBaseType(void(base_t::*)()) -> base_t;

#define GENERATE_BODY() \
public: \
	static Class* StaticClass() { static ClassOf<this_t> _class; return &_class; } \
	virtual Class* GetClass() { static ClassOf<this_t> _class; return &_class; } \
private: \
	template<typename T> void GetBaseType() {} \
	typedef decltype(GetFunctionBaseType(&GetBaseType<void>)) this_t; \

class Class
{
	template <class T>
	friend class ClassOf;

private:
	virtual const type_info& get_type_info() const = 0;

public:
	virtual size_t size() const = 0;
	virtual const char* name() const = 0;
	virtual size_t hash() const = 0;

	bool operator==(const Class& _class) const
	{
		return get_type_info() == _class.get_type_info();
	}
};

template <class T>
class ClassOf : public Class
{
private:
	virtual const type_info& get_type_info() const override
	{
		return typeid(T);
	}

public:
	ClassOf() {}
	ClassOf(const Class& _class)
	{
		if (typeid(T) != _class.get_type_info())
		{
			auto const& message = string_sprintf("'%s' is not base of '%s'", typeid(T).name(), _class.get_type_info().name());

			throw std::bad_cast::__construct_from_string_literal(message.c_str());
		}
	}

	virtual size_t size() const override
	{
		return sizeof(T);
	}

	virtual const char* name() const override
	{
		return typeid(T).name();
	}

	virtual size_t hash() const override
	{
		return typeid(T).hash_code();
	}
};

class Object
{
	GENERATE_BODY();
};

class Field : public Object
{
	GENERATE_BODY();

private:
	size_t offset;

public:
	Field(const size_t& offset) : offset(offset) {}

	template <typename T>
	T get(void* object)
	{
		return *reinterpret_cast<T*>(reinterpret_cast<char*>(object) + offset);
	}

	template <typename T>
	void set(void* object, const T& value)
	{
		*reinterpret_cast<T*>(reinterpret_cast<char*>(object) + offset) = value;
	}
};
