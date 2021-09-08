#ifndef __WD_NONCOPYABLE_HPP__
#define __WD_NONCOPYABLE_HPP__

class Noncopyable{
protected:
	Noncopyable() {}
	~Noncopyable() {}
	Noncopyable(const Noncopyable&) = delete;
	Noncopyable & operator=(const Noncopyable&) = delete;

};
#endif
