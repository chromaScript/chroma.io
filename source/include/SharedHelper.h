#ifndef SHAREDHELPER_H
#define SHAREDHELPER_H

template <class Base, class Derived>
struct enable_shared : public Base
{
	std::shared_ptr<Derived> shared_from_this()
	{
		return std::static_pointer_cast<Derived>(
			Base::shared_from_this());
	};
};

#endif