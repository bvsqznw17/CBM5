#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <QVector>

template <typename T>
class RingBufferFull
{
public:
    inline RingBufferFull() {}

    inline explicit RingBufferFull(int size) : m_v(size)
    { m_data = m_v.data(); m_i = 0; }

    inline RingBufferFull(const RingBufferFull<T> &v) : m_v(v.m_v)
    { m_data = m_v.data(); m_i = v.m_i; }

    inline RingBufferFull<T> &operator=(const RingBufferFull<T> &v)
    { m_v = v.m_v; m_data = m_v.data(); m_i = v.m_i; return *this; }

    inline T &operator[](int i)
    { return m_data[(i + m_i) % m_v.size()]; }

    inline const T &operator[](int i) const
    { return m_data[(i + m_i) % m_v.size()]; }

    inline int size() const { return m_v.size(); }
    inline int ssize() const { return m_v.size(); }
    inline void clear() { *this = RingBufferFull(m_v.size()); }

    inline void append(const T &t)
    {
        m_data[m_i] = t;
        m_i = (m_i + 1) % m_v.size();
    }

    inline void prepend(const T &t)
    {
        m_i = (m_i - 1 + m_v.size()) % m_v.size();
        m_data[m_i] = t;
    }

    inline void appends(const T *t, int n)
    {
        for (const T *e = t + n; t != e; t++)
            append(*t);
    }

    inline void prepends(const T *t, int n)
    {
        for (const T *e = t + n; t != e;)
            prepend(*(--e));
    }

    inline void appendm(const T *t, int n)
    {
        if (QTypeInfo<T>::isComplex)
            appends(t, n);
        else {
            memcpy(m_data + m_i, t, n * sizeof(T));
            m_i = (m_i + n) % m_v.size();
        }
    }

    inline void prependm(const T *t, int n)
    {
        if (QTypeInfo<T>::isComplex)
            prepends(t, n);
        else {
            m_i = (m_i - n + m_v.size()) % m_v.size();
            memcpy(m_data + m_i, t, n * sizeof(T));
        }
    }

    inline void append(const T *t, int n)
    {
        if (QTypeInfo<T>::isComplex)
            appends(t, n);
        else
            m_append(t, n);
    }

    inline void prepend(const T *t, int n)
    {
        if (QTypeInfo<T>::isComplex)
            prepends(t, n);
        else
            m_prepend(t, n);
    }

private:
    void m_append(const T *t, int n)
    {
        while (n > 0) {
            int m = qMin(n, m_v.size() - m_i);
            appendm(t, m);
            n -= m;
            t += m;
        }
    }

    void m_prepend(const T *t, int n)
    {
        while (n > 0) {
            int m = qMin(n, (m_i - 1 + m_v.size()) % m_v.size() + 1);
            n -= m;
            prependm(t + n, m);
        }
    }

private:
    QVector<T> m_v;
    T *m_data;
    int m_i;
};

template <typename T>
class RingBufferLimited
{
public:
    inline RingBufferLimited() { m_size = 0; }

    inline explicit RingBufferLimited(int size) : m_v(size)
    { m_data = m_v.data(); m_size = 0; m_i = 0; }

    inline RingBufferLimited(const RingBufferLimited<T> &v) : m_v(v.m_v)
    { m_data = m_v.data(); m_size = 0; m_i = v.m_i; }

    inline RingBufferLimited<T> &operator=(const RingBufferLimited<T> &v)
    { m_v = v.m_v; m_data = m_v.data(); m_size = 0; m_i = v.m_i; return *this; }

    inline T &operator[](int i)
    { return m_data[(i + m_i) % m_v.size()]; }

    inline const T &operator[](int i) const
    { return m_data[(i + m_i) % m_v.size()]; }

    inline int size() const { return m_size; }
    inline int ssize() const { return m_v.size(); }
    inline void clear() { *this = RingBufferLimited(m_v.size()); }

    void append(const T &t)
    { m_append(t); }

    void prepend(const T &t)
    { m_prepend(t); }

    void appends(const T *t, int n)
    {
        for (const T *e = t + n; t != e; t++)
            m_append(*t);
    }

    void prepends(const T *t, int n)
    {
        for (const T *e = t + n; t != e;)
            m_prepend(*(--e));
    }

    inline void appendm(const T *t, int n)
    {
        if (QTypeInfo<T>::isComplex)
            appends(t, n);
        else
            m_appendm(t, n);
    }

    inline void prependm(const T *t, int n)
    {
        if (QTypeInfo<T>::isComplex)
            prepends(t, n);
        else
            m_prependm(t, n);
    }

    inline void append(const T *t, int n)
    {
        if (QTypeInfo<T>::isComplex)
            appends(t, n);
        else
            m_append(t, n);
    }

    inline void prepend(const T *t, int n)
    {
        if (QTypeInfo<T>::isComplex)
            prepends(t, n);
        else
            m_prepend(t, n);
    }

private:
    inline void m_append(const T &t)
    {
        m_data[(m_i + m_size) % m_v.size()] = t;
        int m = m_size / m_v.size();
        m_size += 1 - m;
        m_i = (m_i + m) % m_v.size();
    }

    inline void m_prepend(const T &t)
    {
        m_i = (m_i - 1 + m_v.size()) % m_v.size();
        m_data[m_i] = t;
        int m = m_size / m_v.size();
        m_size += 1 - m;
    }

    inline void m_appendmm(const T *t, int n)
    {
        memcpy(m_data + ((m_i + m_size) % m_v.size()), t, n * sizeof(T));
        m_size += n;
        int mm = m_size / m_v.size();
        int m = (m_size - m_v.size()) * mm;
        m_size -= m;
        m_i = (m_i + m) % m_v.size();
    }

    inline void m_prependmm(const T *t, int n)
    {
        m_i = (m_i - n + m_v.size()) % m_v.size();
        memcpy(m_data + m_i, t, n * sizeof(T));
        m_size += n;
        int mm = m_size / m_v.size();
        int m = (m_size - m_v.size()) * mm;
        m_size -= m;
    }

    void m_appendm(const T *t, int n)
    { m_appendmm(t, n); }

    void m_prependm(const T *t, int n)
    { m_prependmm(t, n); }

    void m_append(const T *t, int n)
    {
        while (n > 0) {
            int m = qMin(n, m_v.size() - ((m_i + m_size) % m_v.size()));
            m_appendmm(t, m);
            n -= m;
            t += m;
        }
    }

    void m_prepend(const T *t, int n)
    {
        while (n > 0) {
            int m = qMin(n, (m_i - 1 + m_v.size()) % m_v.size() + 1);
            n -= m;
            m_prependmm(t + n, m);
        }
    }

private:
    QVector<T> m_v;
    T *m_data;
    int m_size;
    int m_i;
};

template <typename T>
class RingBufferLimitless
{
};

#endif // RINGBUFFER_H
