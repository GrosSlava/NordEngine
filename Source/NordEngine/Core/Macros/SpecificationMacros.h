
#pragma once

#include <string>




/*
	Macro for specific structs or classes where very important to have correct data
*/






/*
	@param ClassName.
*/
#define GENERATED_BODY(ClassName)																\
	private:																					\
		using Self = ClassName;																	\
	public:																						\
		virtual std::string GetSemanticClassName() const noexcept { return #ClassName ; }		\
		virtual std::string GetParentSemanticClassName() const noexcept { return "None"; }		\
	private:
/*
	@param StructName.
*/
#define GENERATED_STRUCT_BODY(StructName)														\
	private:																					\
		using Self = StructName;																\
	public:																						\
		virtual std::string GetSemanticStructName() const noexcept { return #StructName; }

/*
	@param ClassName, ParentClass.
*/
#define GENERATED_BODY_CHILD(ClassName, ParentClass)															\
	private:																									\
		using Super = ParentClass;																				\
		using Self = ClassName;																					\
	public:																										\
		virtual std::string GetSemanticClassName() const noexcept override { return #ClassName; }				\
		virtual std::string GetParentSemanticClassName() const noexcept override { return #ParentClass; }		\
	private:




/*
	This class can't be copied.
*/
#define NONCOPYABLE(TypeName)							\
	public:												\
		TypeName(TypeName&&) = delete;					\
		TypeName(const TypeName&) = delete;				\
		TypeName& operator=(const TypeName&) = delete;	\
		TypeName& operator=(TypeName&&) = delete;		\
	private:
/*
	This struct can't be copied.
*/
#define NONCOPYABLE_STRUCT(TypeName)					\
	public:												\
		TypeName(TypeName&&) = delete;					\
		TypeName(const TypeName&) = delete;				\
		TypeName& operator=(const TypeName&) = delete;	\
		TypeName& operator=(TypeName&&) = delete;		\




/* 
	Base Singleton.
	NOTE: Implement Default Constructor and Get() method.
*/
#define SINGLETON(TypeName)					\
	NONCOPYABLE(TypeName)					\
	private:								\
		TypeName();							\
	public:									\
		static TypeName* Get() noexcept;	\
	private:
/* 
	Base Singleton.
	NOTE: Implement Default Constructor and Get() method.
*/
#define SINGLETON_STRUCT(TypeName)			\
	NONCOPYABLE_STRUCT(TypeName)			\
	private:								\
		TypeName();							\
	public:									\
		static TypeName* Get() noexcept;	