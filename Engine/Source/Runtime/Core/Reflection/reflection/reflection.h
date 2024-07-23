#pragma once
#include <any>
#include <magic_enum/magic_enum.hpp>
#include "Reflection/json.h"
#include <functional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "ContainerTypes.h"
#include "Misc/Platform.h"
#include "Object/ObjectPropertyTag.h"

#define NAME(name) #name

#define DefineDefaultConstructor(class_name) \
    friend class _ObjectDefaultConstructor<class_name, true>;\
    friend class _ObjectDefaultConstructor<class_name, false>;\
    public: explicit class_name(const MObjectInitiliazer&) {}; \

#if defined(__REFLECTION_PARSER__)
#define MPROPERTY(...) __attribute__((annotate("MPROPERTY," #__VA_ARGS__)))
#define MFUNCTION(...) __attribute__((annotate("MFUNCTION," #__VA_ARGS__)))
#else
#define MPROPERTY(...)
#define MFUNCTION(...)
#endif // __REFLECTION_PARSER__

#define REFLECTION_BODY_OBJECT(class_name) \
    friend class ::Reflection::TypeFieldReflectionOparator::Type##class_name##Operator; \
    friend class Serializer; \
    public: virtual std::string ClassName() { return #class_name; }\

#define REFLECTION_BODY(class_name) \
    friend class ::Reflection::TypeFieldReflectionOparator::Type##class_name##Operator; \
    friend class Serializer; \
    public: virtual std::string ClassName() override { return #class_name; }\

#define REFLECTION_STRUCT_BODY(class_name) \
	friend class ::Reflection::TypeFieldReflectionOparator::Type##class_name##Operator; \
	friend class Serializer; \
	public: virtual std::string ClassName() { return #class_name; }\

#define REFLECTION_TYPE(class_name, ...) \
    namespace Reflection \
    { \
        namespace TypeFieldReflectionOparator \
        { \
            class Type##class_name##Operator; \
        } \
    };

#define MCLASS(class_name, ...) \
    namespace Reflection \
    { \
        namespace TypeFieldReflectionOparator \
        { \
            class Type##class_name##Operator; \
        } \
    }; \
    inline int MTAG_MCLASS_##class_name=0;

#define MSTRUCT(class_name, ...) \
	namespace Reflection \
	{ \
		namespace TypeFieldReflectionOparator \
		{ \
			class Type##class_name##Operator; \
		} \
	}; \
	inline int MTAG_MSTRUCT_##class_name=0;

#define REGISTER_FIELD_TO_MAP(name, value) TypeMetaRegisterinterface::registerToFieldMap(name, value);
#define REGISTER_Method_TO_MAP(name, value) TypeMetaRegisterinterface::registerToMethodMap(name, value);
#define REGISTER_BASE_CLASS_TO_MAP(name, value) TypeMetaRegisterinterface::registerToClassMap(name, value);
#define REGISTER_ARRAY_TO_MAP(name, value) TypeMetaRegisterinterface::registerToArrayMap(name, value);
#define REGISTER_ENUM_TO_MAP(name, value) TypeMetaRegisterinterface::registerToEnumMap(name, value);
#define REGISTER_POINTER_TO_MAP(name, value) TypeMetaRegisterinterface::registerToPointerMap(name, value);
#define UNREGISTER_ALL TypeMetaRegisterinterface::unregisterAll();

#define PICCOLO_REFLECTION_NEW(name, ...) Reflection::ReflectionPtr(#name, new name(__VA_ARGS__));
#define PICCOLO_REFLECTION_DELETE(value) \
    if (value) \
    { \
        delete value.operator->(); \
        value.getPtrReference() = nullptr; \
    }
#define PICCOLO_REFLECTION_DEEP_COPY(type, dst_ptr, src_ptr) \
    *static_cast<type*>(dst_ptr) = *static_cast<type*>(src_ptr.getPtr());

#define TypeMetaDef(class_name, ptr) \
    Reflection::ReflectionInstance(Reflection::TypeMeta::newMetaFromName(#class_name), \
                                            ptr)

#define TypeMetaDefPtr(class_name, ptr) \
    new Reflection::ReflectionInstance(Reflection::TypeMeta::newMetaFromName(#class_name), \
                                                ptr)

template<typename T, typename U, typename = void>
struct is_safely_castable : std::false_type
{
};

template<typename T, typename U>
struct is_safely_castable<T, U, std::void_t<decltype(static_cast<U>(std::declval<T>()))>> : std::true_type
{
};

template<typename T>
concept IsNativePointer = std::is_pointer_v<T>;

template<typename T>
concept IsUniquePointer = requires(T a) {
	requires std::is_same_v<T, std::unique_ptr<typename T::element_type>>;
};

template<typename T>
concept IsSharedPointer = requires(T a) {
	requires std::is_same_v<T, std::shared_ptr<typename T::element_type>>;
};

template<typename T>
concept IsWeakPointer = requires(T a) {
	requires std::is_same_v<T, std::weak_ptr<typename T::element_type>>;
};

template<typename T>
concept IsSmartPointer = IsUniquePointer<T> || IsSharedPointer<T> || IsWeakPointer<T>;

template<typename T>
concept IsAnyPointer = IsNativePointer<T> || IsUniquePointer<T> || IsSharedPointer<T>;

template<IsUniquePointer T>
void* reflection_get_pointer_inner(const T& ptr) {
	return static_cast<void*>(ptr.get());
}

template<IsSharedPointer T>
void* reflection_get_pointer_inner(const T& ptr) {
	return static_cast<void*>(ptr.get());
}

template<IsWeakPointer T>
void* reflection_get_pointer_inner(const T& ptr) {
	return static_cast<void*>(ptr.lock().get());
}

template<IsNativePointer T>
void* reflection_get_pointer_inner(T ptr) {
	return static_cast<void*>(ptr);
}

namespace Reflection
{
    class TypeMeta;
    class FieldAccessor;
    class MethodAccessor;
    class ArrayAccessor;
    class ReflectionInstance;
} // namespace Reflection
typedef std::function<void(void*, void*)> SetFuncion;
typedef std::function<void*(void*)> GetFuncion;
typedef std::function<const char*()> GetNameFuncion;
typedef std::function<void(int, void*, void*)> SetArrayFunc;
typedef std::function<void*(int, void*)> GetArrayFunc;
typedef std::function<int(void*)> GetSizeFunc;
typedef std::function<bool()> GetBoolFunc;
typedef std::function<std::any(void *, std::any)> InvokeFunction;
typedef std::function<std::vector<std::string>()> GetStringArrayFunc;
typedef std::function<void(void*, std::string)> SetStringFunc;
typedef std::function<std::string(void*)> GetStringFunc;

typedef std::function<void*(const Json&)> ConstructorWithJson;
typedef std::function<Json(void*)> WriteJsonByName;
typedef std::function<int(Reflection::ReflectionInstance*&, void*)> GetBaseClassReflectionInstanceListFunc;

typedef std::tuple<SetFuncion, GetFuncion, GetNameFuncion, GetNameFuncion, GetNameFuncion, GetBoolFunc, GetBoolFunc, GetBoolFunc,
std::function<std::vector<void*>()>>
FieldFunctionTuple;
typedef std::tuple<GetNameFuncion, InvokeFunction> MethodFunctionTuple;
typedef std::tuple<GetBaseClassReflectionInstanceListFunc, ConstructorWithJson, WriteJsonByName> ClassFunctionTuple;
typedef std::tuple<SetArrayFunc, GetArrayFunc, GetSizeFunc, GetNameFuncion, GetNameFuncion> ArrayFunctionTuple;
typedef std::tuple<GetNameFuncion, GetStringArrayFunc, GetStringFunc, SetStringFunc> EnumFunctionTuple;
typedef std::tuple<GetFuncion> PointerFunctionTuple;

namespace Reflection
{
	class EnumAccessor;
	class ArrayAccessor;
	class PointerAccessor;

    class ENGINE_API TypeMetaRegisterinterface
    {
    public:
        static void registerToClassMap(const char* name, ClassFunctionTuple* value);

        static void registerToFieldMap(const char* name, FieldFunctionTuple* value);

        static void registerToMethodMap(const char* name, MethodFunctionTuple* value);

        static void registerToArrayMap(const char* name, ArrayFunctionTuple* value);

    	static void registerToEnumMap(const char* name, EnumFunctionTuple* value);

    	static void registerToPointerMap(const char* name, PointerFunctionTuple* value);

        static void unregisterAll();
    };

    class ENGINE_API TypeMeta
    {
        friend class FieldAccessor;
        friend class ArrayAccessor;
        friend class TypeMetaRegisterinterface;

    public:
        TypeMeta();

        // static void Register();

        static TypeMeta newMetaFromName(std::string type_name);

        static bool newArrayAccessorFromName(std::string array_type_name, ArrayAccessor& accessor);

        static ReflectionInstance newFromNameAndJson(std::string type_name, const Json& json_context);

        static Json writeByName(std::string type_name, void* instance);

        std::string getTypeName();

        std::vector<FieldAccessor> getFieldsList();

        std::vector<MethodAccessor> getMethodsList();

        int getBaseClassReflectionInstanceList(ReflectionInstance*& out_list, void* instance);

        FieldAccessor getFieldByName(const std::string& name);

        MethodAccessor getMethodByName(const std::string& name);

        bool isValid() { return m_is_valid; }

        TypeMeta& operator=(const TypeMeta& dest);

    private:
        TypeMeta(std::string type_name);

    private:
        std::vector<FieldAccessor, std::allocator<FieldAccessor>> m_fields;
        std::vector<MethodAccessor, std::allocator<MethodAccessor>> m_methods;
        std::string m_type_name;

        bool m_is_valid;
    };

    class ENGINE_API FieldAccessor
    {
        friend class TypeMeta;

    public:
        FieldAccessor();

        void* get(void* instance);

        template <typename T>
        T get(void* instance)
        {
            return *static_cast<T*>(get(instance));
        }

        void set(void* instance, void* value);

        TypeMeta getOwnerTypeMeta();

        bool getTypeMeta(TypeMeta& field_type) const;

        const char* getTypeName() const;

        const char* getFieldName() const;

    	FORCEINLINE String GetName() const{ return class_name; }

		std::string getFieldTypeName() const;

        // Remove ref and const and namespace
        std::string GetPureTypeName();

        bool isArrayType();

    	bool isEnumType();

    	bool isPointerType();

        /**
         * Get enum accessor of the instance in this field
         * @param instance the instance to the field
         * @return EnumAccessor of the instance
         */
        EnumAccessor GetEnumAccessor(void* instance);
    	PointerAccessor GetPointerAccessor(void* instance);
        FieldAccessor& operator=(const FieldAccessor& dest);

		/**
		 * Get all property tags of the field
		 * @return array of property tags
		 */
		std::vector<const ObjectPropertyTag *> GetPropertyTags() const;

    	/**
		 * Get property tag of the field
		 * @tparam T the property tag type, should be derived from PropertyTag::ObjectPropertyTag
		 * @return the property tag of the field, may be nullptr if the tag not exist
		 */
		template <class T>
    	requires std::derived_from<T, ObjectPropertyTag>
    	const T* GetPropertyTag()
		{
			for (auto tag : GetPropertyTags())
			{
				if (auto p = dynamic_cast<const T*>(tag))
				{
					return p;
				}
			}
			return nullptr;
		}

    	FORCEINLINE bool operator == (const std::string& FieldName) const
		{
			return GetName() == FieldName;
		}

	private:
        FieldAccessor(FieldFunctionTuple* functions);

    private:
        FieldFunctionTuple* m_functions;
        const char* class_name;
        const char* m_field_type_name;
    };

    class ENGINE_API MethodAccessor
    {
        friend class TypeMeta;

    public:
        MethodAccessor();

        template<typename... Args>
        std::any Invoke(void* instance, Args&& ...args)
        {
            auto P = std::make_tuple(args...);
            return std::get<1>(*m_functions)(instance, &P);
        }

        const char* getMethodName() const;

        MethodAccessor& operator=(const MethodAccessor& dest);

    private:
        MethodAccessor(MethodFunctionTuple* functions);

    private:
        MethodFunctionTuple* m_functions;
        const char* m_method_name;
    };

    /**
     *  Function reflection is not implemented, so use this as an std::vector accessor
     */
    class ENGINE_API ArrayAccessor
    {
        friend class TypeMeta;

    public:
        ArrayAccessor();

        const char* getArrayTypeName();

        const char* getElementTypeName();

        void set(int index, void* instance, void* element_value);

        void* get(int index, void* instance);

        int getSize(void* instance);

        ArrayAccessor& operator=(ArrayAccessor& dest);

    private:
        ArrayAccessor(ArrayFunctionTuple* array_func);

    private:
        ArrayFunctionTuple* m_func;
        const char* m_array_type_name;
        const char* m_element_type_name;
    };

    /**
     * Enum reflection accessor, should be generated by FieldAccessor
     */
    class ENGINE_API EnumAccessor
	{
	public:
		friend class TypeMeta;
    	friend class FieldAccessor;
		EnumAccessor() = default;

		/**
		 * Get all enum value string
		 * @return vector of string of enum values
		 */
		std::vector<std::string> GetEnumStringArray() const;

		/**
		 * Set enum value by string for instance
		 * @param value set string of enum value
		 */
		void                     SetEnumValue(std::string value) const;

		/**
		 * Get enum value string of instance
		 * @return string of enum value
		 */
		std::string              GetEnumValue() const;

		/**
		 * Get enum type name
		 * @return enum type name
		 */
		const char* 			GetEnumTypeName() const;

		/**
		 * Get the filed name of enum, as EnumType FiledName;
		 * @return filed name of enum
		 */
		const char* GetFiledName() const;

	private:
		EnumAccessor(EnumFunctionTuple* func, FieldAccessor* field_accessor, void* instance);
	private:
		EnumFunctionTuple* m_func;
		const char* m_enum_type_name;
		const char* m_filed_name;
    	FieldAccessor* m_field_accessor;
		void* m_instance;
    	void* m_filed_ptr;
	};

	class ENGINE_API PointerAccessor
	{
	public:
		friend class TypeMeta;
		friend class FieldAccessor;
		PointerAccessor() = default;

		PointerAccessor(PointerFunctionTuple* func, FieldAccessor* field_accessor, void* instance);

		/**
		 * Get the inner pointer of the ptr, beacuse the filed may not be a native pointer
		 * Basicly, for native pointer, it will return T*
		 * for smart pointer, it will return the T->get()
		 * @return pointer of instance
		 */
		void* GetPointer() const;

	protected:
		FieldAccessor* m_field_accessor;
		PointerFunctionTuple* m_func;
		void* m_filed_ptr;
		void* m_instance;
	};

    class ENGINE_API ReflectionInstance
    {
    public:
        ReflectionInstance(TypeMeta meta, void* instance) : m_meta(meta), m_instance(instance)
        {
        }

        ReflectionInstance() : m_meta(), m_instance(nullptr)
        {
        }

        ReflectionInstance& operator=(ReflectionInstance& dest);

        ReflectionInstance& operator=(ReflectionInstance&& dest);

    public:
        TypeMeta m_meta;
        void* m_instance;
    };

    template<typename T>
    class ReflectionPtr
    {
        template<typename U>
        friend class ReflectionPtr;

    public:
        ReflectionPtr(std::string type_name, T* instance) : m_type_name(type_name), m_instance(instance)
        {
        }

        ReflectionPtr() : m_type_name(), m_instance(nullptr)
        {
        }

        ReflectionPtr(const ReflectionPtr& dest) : m_type_name(dest.m_type_name), m_instance(dest.m_instance)
        {
        }

        template<typename U /*, typename = typename std::enable_if<std::is_safely_castable<T*, U*>::value>::type */>
        ReflectionPtr<T>& operator=(const ReflectionPtr<U>& dest)
        {
            if (this == static_cast<void *>(&dest)) {
                return *this;
            }
            m_type_name = dest.m_type_name;
            m_instance  = static_cast<T *>(dest.m_instance);
            return *this;
        }

        template<typename U /*, typename = typename std::enable_if<std::is_safely_castable<T*, U*>::value>::type*/>
        ReflectionPtr<T>& operator=(ReflectionPtr<U>&& dest)
        {
            if (this == static_cast<void *>(&dest)) {
                return *this;
            }
            m_type_name = dest.m_type_name;
            m_instance  = static_cast<T *>(dest.m_instance);
            return *this;
        }

        ReflectionPtr<T>& operator=(const ReflectionPtr<T>& dest)
        {
            if (this == &dest) {
                return *this;
            }
            m_type_name = dest.m_type_name;
            m_instance  = dest.m_instance;
            return *this;
        }

        ReflectionPtr<T>& operator=(ReflectionPtr<T>&& dest)
        {
            if (this == &dest) {
                return *this;
            }
            m_type_name = dest.m_type_name;
            m_instance  = dest.m_instance;
            return *this;
        }

        std::string getTypeName() const { return m_type_name; }

        void setTypeName(std::string name) { m_type_name = name; }

        bool operator==(const T* ptr) const { return (m_instance == ptr); }

        bool operator!=(const T* ptr) const { return (m_instance != ptr); }

        bool operator==(const ReflectionPtr<T>& rhs_ptr) const { return (m_instance == rhs_ptr.m_instance); }

        bool operator!=(const ReflectionPtr<T>& rhs_ptr) const { return (m_instance != rhs_ptr.m_instance); }

        template<
            typename T1 /*, typename = typename std::enable_if<std::is_safely_castable<T*, T1*>::value>::type*/>
        explicit operator T1 *()
        {
            return static_cast<T1 *>(m_instance);
        }

        template<
            typename T1 /*, typename = typename std::enable_if<std::is_safely_castable<T*, T1*>::value>::type*/>
        operator ReflectionPtr<T1>()
        {
            return ReflectionPtr<T1>(m_type_name, (T1 *) (m_instance));
        }

        template<
            typename T1 /*, typename = typename std::enable_if<std::is_safely_castable<T*, T1*>::value>::type*/>
        explicit operator const T1 *() const
        {
            return static_cast<T1 *>(m_instance);
        }

        template<
            typename T1 /*, typename = typename std::enable_if<std::is_safely_castable<T*, T1*>::value>::type*/>
        operator const ReflectionPtr<T1>() const
        {
            return ReflectionPtr<T1>(m_type_name, (T1 *) (m_instance));
        }

        T* operator->() { return m_instance; }

        T* operator->() const { return m_instance; }

        T& operator*() { return *(m_instance); }

        T* getPtr() { return m_instance; }

        T* getPtr() const { return m_instance; }

        const T& operator*() const { return *(static_cast<const T *>(m_instance)); }

        T*& getPtrReference() { return m_instance; }

        operator bool() const { return (m_instance != nullptr); }

    private:
        std::string m_type_name{""};
        typedef T m_type;
        T* m_instance{nullptr};
    };
} // namespace Reflection


namespace Reflection
{
    namespace TypeFieldReflectionOparator
    {
        template<typename T>
        struct function_traits : function_traits<decltype(&T::operator())>
        {
        };

        template<typename R, typename... Args>
        struct function_traits<R(*)(Args...)>
        {
            using args_tuple = std::tuple<Args...>;
            using return_type = R;
        };

        template<typename C, typename R, typename... Args>
        struct function_traits<R(C::*)(Args...)>
        {
            using args_tuple = std::tuple<Args...>;
            using return_type = R;
        };

        template<typename C, typename R, typename... Args>
        struct function_traits<R(C::*)(Args...) const>
        {
            using args_tuple = std::tuple<Args...>;
            using return_type = R;
        };


        template<typename Func, class ClassType, typename TupleType>
        static std::any InvokeFun(Func&& func, void* instance, const std::any& Parameter, std::true_type)
        {
            std::apply(std::forward<Func>(func),
                       std::tuple_cat(std::make_tuple(static_cast<ClassType *>(instance)),
                                      *std::any_cast<TupleType *>(Parameter)));
            return {};
        }

        template<typename Func, class ClassType, typename TupleType>
        static std::any InvokeFun(Func&& func, void* instance, const std::any& Parameter, std::false_type)
        {
            return std::apply(std::forward<Func>(func),
                              std::tuple_cat(std::make_tuple(static_cast<ClassType *>(instance)),
                                             *std::any_cast<TupleType *>(Parameter)));
        }
    }
}; // namespace Reflection::TypeFieldReflectionOparator