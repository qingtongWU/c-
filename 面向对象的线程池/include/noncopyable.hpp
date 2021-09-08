#ifndef __NONCOPYABLE_HPP__
#define __NONCOPYABLE_HPP__

class Noncopyable
{
protected:
    Noncopyable() {}
    ~Noncopyable() {}

    Noncopyable(const Noncopyable&) = delete;
    Noncopyable & operator=(const Noncopyable&) = delete;

};

#endif