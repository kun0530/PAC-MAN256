#pragma once

// TO-DO: ΩÃ±€≈œ ∞¥√º ∫∏¿Â

template <typename T>
class Singleton
{
protected:
	Singleton() = default;
	virtual ~Singleton() = default;

public:
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;

	static T& Instance()
	{
		static T instance;
		return instance;
	}
};