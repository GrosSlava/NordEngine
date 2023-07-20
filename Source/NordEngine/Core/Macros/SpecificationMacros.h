// Copyright Nord Engine. All Rights Reserved.
#pragma once




/**
	This class can't be copied.
*/
#define NONCOPYABLE(TypeName)                      \
public:                                            \
                                                   \
	TypeName(TypeName&&) = delete;                 \
	TypeName(const TypeName&) = delete;            \
	TypeName& operator=(const TypeName&) = delete; \
	TypeName& operator=(TypeName&&) = delete;      \
                                                   \
private:

/**
	This struct can't be copied.
*/
#define NONCOPYABLE_STRUCT(TypeName)               \
public:                                            \
                                                   \
	TypeName(TypeName&&) = delete;                 \
	TypeName(const TypeName&) = delete;            \
	TypeName& operator=(const TypeName&) = delete; \
	TypeName& operator=(TypeName&&) = delete;

/**
	Myers Singleton.
	NOTE: Implement Default Constructor.
*/
#define SINGLETON(TypeName)                \
	NONCOPYABLE(TypeName)                  \
private:                                   \
                                           \
	TypeName();                            \
                                           \
public:                                    \
                                           \
	inline static TypeName* Get() noexcept \
	{                                      \
		static TypeName S;                 \
		return &S;                         \
	}                                      \
                                           \
private:

/* 
	Myers Singleton.
	NOTE: Implement Default Constructor.
*/
#define SINGLETON_STRUCT(TypeName)         \
	NONCOPYABLE_STRUCT(TypeName)           \
private:                                   \
                                           \
	TypeName();                            \
                                           \
public:                                    \
                                           \
	inline static TypeName* Get() noexcept \
	{                                      \
		static TypeName S;                 \
		return &S;                         \
	}
