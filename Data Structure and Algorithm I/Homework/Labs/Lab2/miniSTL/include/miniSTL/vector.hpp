#include <cstddef>
#include <iterator>
#include <memory>
#include <limits>
#include <stdexcept>
#include <utility>
#include <compare>
#include <initializer_list>

template <class T, class Alloc = std::allocator<T>>
struct Vector
{
    using value_type = T;
    using allocator_type = Alloc;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using pointer = T *;
    using const_pointer = T const *;
    using reference = T &;
    using const_reference = T const &;
    using iterator = T *;
    using const_iterator = T const *;
    using reverse_iterator = std::reverse_iterator<T *>;
    using const_reverse_iterator = std::reverse_iterator<T const *>;

    T *m_data;
    size_t m_size;
    size_t m_cap;
    [[no_unique_address]] Alloc m_alloc;

    Vector() : m_data(nullptr), m_size(0), m_cap(0) {}

    Vector(std::initializer_list<T> ilist, Alloc const &alloc = Alloc())
        // 使用初始化列表初始化
        : m_alloc(alloc),
          m_size(ilist.size()),
          m_cap(ilist.size())
    {
        if (m_cap > 0)
        {
            m_data = m_alloc.allocate(m_cap); // 分配内存
            size_t i = 0;
            for (auto it : ilist)
            {
                std::construct_at(m_data + i, it); // 构造对象
                ++i;
            }
        }
        else
        {
            m_data = nullptr;
        }
    }

    explicit Vector(size_t n, Alloc const &alloc = Alloc())
        // 初始化能容纳 n 个元素
        : m_alloc(alloc),
          m_size(n),
          m_cap(n)
    {
        if (m_cap > 0)
        {
            m_data = m_alloc.allocate(m_cap); // 分配内存
            for (size_t i = 0; i < m_size; ++i)
            {
                std::construct_at(m_data + i); // 构造对象
            }
        }
        else
        {
            m_data = nullptr;
        }
    }

    Vector(size_t n, T const &val, Alloc const &alloc = Alloc())
        // 初始化 n 个元素，值为 val
        : m_alloc(alloc),
          m_size(n),
          m_cap(n)
    {
        if (m_cap > 0)
        {
            m_data = m_alloc.allocate(m_cap); // 分配内存
            for (size_t i = 0; i < m_size; ++i)
            {
                std::construct_at(m_data + i, val); // 构造对象
            }
        }
        else
        {
            m_data = nullptr;
        }
    }

    template <std::random_access_iterator InputIt>
    Vector(InputIt first, InputIt last, Alloc const &alloc = Alloc())
        // 使用首尾迭代器初始化
        : m_alloc(alloc),
          m_size(last - first),
          m_cap(m_size)
    {
        if (m_cap > 0)
        {
            m_data = m_alloc.allocate(m_cap);
            size_t i = 0;
            for (InputIt it = first; it != last; ++it)
            {
                std::construct_at(m_data + i, *it);
                ++i;
            }
        }
        else
        {
            m_data = nullptr;
        }
    }

    void clear() // 清空元素，cap 不变
    {
        if (m_data != nullptr)
        {
            for (size_t i = 0; i < m_size; ++i)
            {
                std::destroy_at(m_data + i);
            }
            m_size = 0;
        }
    }

    void resize(size_t n)
    {
        if (n < m_size)
        {
            for (size_t i = n; i < m_size; ++i)
            {
                std::destroy_at(m_data + i);
            }
            m_size = n;
        }
        else if (n > m_size)
        {
            T *new_data = m_alloc.allocate(n);
            if (m_data != nullptr)
            {
                for (size_t i = 0; i < m_size; ++i)
                {
                    std::construct_at(new_data + i, std::move(*(m_data + i))); // 移动原有元素
                    std::destroy_at(m_data + i);                               // 析构旧元素
                }
                m_alloc.deallocate(m_data, m_cap); // 释放内存
            }
            for (size_t i = m_size; i < n; ++i)
            {
                std::construct_at(new_data + i);
            }
            m_size = n;
            m_cap = n;
            m_data = new_data;
        }
    }

    void resize(size_t n, T const &val)
    {
        if (n < m_size)
        {
            for (size_t i = n; i < m_size; ++i)
            {
                std::destroy_at(m_data + i);
            }
            m_size = n;
        }
        else if (n > m_size)
        {
            T *new_data = m_alloc.allocate(n);
            if (m_data != nullptr)
            {
                for (size_t i = 0; i < m_size; ++i)
                {
                    std::construct_at(new_data + i, std::move(*(m_data + i))); // 移动原有元素
                    std::destroy_at(m_data + i);                               // 析构旧元素
                }
                m_alloc.deallocate(m_data, m_cap); // 释放内存
            }
            for (size_t i = m_size; i < n; ++i)
            {
                std::construct_at(new_data + i, val);
            }
            m_size = n;
            m_cap = n;
            m_data = new_data;
        }
    }

    void shrink_to_fit()
    {
        if (m_cap > m_size)
        {
            if (m_size > 0)
            {
                T *new_data = m_alloc.allocate(m_size);
                if (m_data != nullptr)
                {
                    for (size_t i = 0; i < m_size; ++i)
                    {
                        std::construct_at(new_data + i, std::move(*(m_data + i)));
                        std::destroy_at(m_data + i);
                    }
                    m_alloc.deallocate(m_data, m_cap);
                }
                m_data = new_data;
                m_cap = m_size;
            }
            else
            {
                if (m_data != nullptr)
                {
                    m_alloc.deallocate(m_data, m_cap);
                }
                m_data = nullptr;
                m_cap = 0;
            }
        }
    }

    void reserve(size_t n) // 确保容量至少为 n
    {
        if (n > m_cap)
        {
            T *new_data = m_alloc.allocate(n);
            if (m_data != nullptr)
            {
                for (size_t i = 0; i < m_size; ++i)
                {
                    std::construct_at(new_data + i, std::move(*(m_data + i)));
                    std::destroy_at(m_data + i);
                }
                m_alloc.deallocate(m_data, m_cap);
            }
            m_data = new_data;
            m_cap = n;
        }
    }

    size_t capacity() const { return m_cap; }

    size_t size() const { return m_size; }

    bool empty() const { return m_size == 0; }

    T const &operator[](size_t i) const { return m_data[i]; }

    T &operator[](size_t i) { return m_data[i]; }

    T const &at(size_t i) const
    {
        if (i >= m_size)
        {
            throw std::out_of_range("Vector::at: index out of range");
        }
        return m_data[i];
    }

    T &at(size_t i)
    {
        if (i >= m_size)
        {
            throw std::out_of_range("Vector::at: index out of range");
        }
        return m_data[i];
    }

    Vector(Vector &&that) : // 移动
                            m_data(that.m_data),
                            m_size(that.m_size),
                            m_cap(that.m_cap),
                            m_alloc(std::move(that.m_alloc))
    {
        that.m_data = nullptr;
        that.m_size = 0;
        that.m_cap = 0;
    }

    Vector(Vector &&that, Alloc const &alloc)
        : m_alloc(alloc)
    {
        if (m_alloc == that.m_alloc)
        {
            // 分配器相同，接管资源
            m_data = that.m_data;
            m_size = that.m_size;
            m_cap = that.m_cap;
        }
        else
        {
            // 分配器不同，分配新内存，移动元素到新内存
            m_size = that.m_size;
            m_cap = that.m_size;
            if (m_cap > 0)
            {
                m_data = m_alloc.allocate(m_cap);
                for (size_t i = 0; i < m_size; ++i)
                {
                    std::construct_at(m_data + i, std::move(*(that.m_data + i)));
                    std::destroy_at(that.m_data + i);
                }
                that.m_alloc.deallocate(that.m_data, that.m_cap);
            }
            else
            {
                m_data = nullptr;
            }
        }
        that.m_data = nullptr;
        that.m_size = 0;
        that.m_cap = 0;
    }

    Vector &operator=(Vector &&that)
    {
        if (this != &that)
        {
            // 销毁和释放当前资源
            clear();
            if (m_data != nullptr)
            {
                m_alloc.deallocate(m_data, m_cap);
            }

            if (m_alloc == that.m_alloc)
            {
                // 分配器相同，接管资源
                m_data = that.m_data;
                m_size = that.m_size;
                m_cap = that.m_cap;
            }
            // 接管数据
            else
            {
                // 分配器不同，分配新内存，移动元素到新内存
                m_size = that.m_size;
                m_cap = that.m_size;
                if (m_cap > 0)
                {
                    m_data = m_alloc.allocate(m_cap);
                    for (size_t i = 0; i < m_size; ++i)
                    {
                        std::construct_at(m_data + i, std::move(*(that.m_data + i)));
                        std::destroy_at(that.m_data + i);
                    }
                    that.m_alloc.deallocate(that.m_data, that.m_cap);
                }
                else
                {
                    m_data = nullptr;
                }
            }
            // 清空源对象
            that.m_data = nullptr;
            that.m_size = 0;
            that.m_cap = 0;
        }
        return *this;
    }

    void swap(Vector &that)
    {
        std::swap(m_data, that.m_data);
        std::swap(m_size, that.m_size);
        std::swap(m_cap, that.m_cap);
        std::swap(m_alloc, that.m_alloc);
    }

    Vector(Vector const &that)
        : m_alloc(that.m_alloc),
          m_size(that.m_size),
          m_cap(that.m_cap)
    {
        if (m_cap > 0)
        {
            m_data = m_alloc.allocate(m_cap);
            {
                for (size_t i = 0; i < m_size; ++i)
                {
                    std::construct_at(m_data + i, *(that.m_data + i));
                }
            }
        }
        else
        {
            m_data = nullptr;
        }
    }

    Vector(Vector const &that, Alloc const &alloc)
        : m_alloc(alloc),
          m_size(that.m_size),
          m_cap(that.m_cap)
    {
        if (m_cap > 0)
        {
            m_data = m_alloc.allocate(m_cap);

            for (size_t i = 0; i < m_size; ++i)
            {
                std::construct_at(m_data + i, *(that.m_data + i));
            }
        }
        else
        {
            m_data = nullptr;
        }
    }

    Vector &operator=(Vector const &that)
    {
        Vector temp(that);
        swap(temp);
        return *this;
    }

    T const &front() const { return m_data[0]; }

    T &front() { return m_data[0]; }

    T const &back() const { return m_data[m_size - 1]; }

    T &back() { return m_data[m_size - 1]; }

    void push_back(T const &val)
    {
        if (m_size == m_cap) // 扩容：容量翻倍，至少加 1
        {
            reserve(m_cap == 0 ? 1 : m_cap * 2);
        }
        std::construct_at(m_data + m_size, val);
        ++m_size;
    }

    void push_back(T &&val)
    {
        if (m_size == m_cap) // 扩容：容量翻倍，至少加 1
        {
            reserve(m_cap == 0 ? 1 : m_cap * 2);
        }
        std::construct_at(m_data + m_size, std::move(val));
        ++m_size;
    }

    T *data() { return m_data; }

    T const *data() const { return m_data; }

    T const *cdata() { return m_data; }

    T *begin() { return m_data; }

    T *end() { return m_data + m_size; }

    T const *begin() const { return m_data; }

    T const *end() const { return m_data + m_size; }

    T const *cbegin() { return m_data; }

    T const *cend() { return m_data + m_size; }

    std::reverse_iterator<T *> rbegin()
    {
        return reverse_iterator(end());
    }

    std::reverse_iterator<T *> rend()
    {
        return reverse_iterator(begin());
    }

    std::reverse_iterator<T const *> crbegin()
    {
        return reverse_iterator(cend());
    }

    std::reverse_iterator<T const *> crend()
    {
        return reverse_iterator(cbegin());
    }

    void pop_back()
    {
        if (m_size > 0)
        {
            std::destroy_at(m_data + m_size - 1);
            --m_size;
        }
    }

    T *erase(T const *it) noexcept(std::is_nothrow_move_assignable_v<T>)
    {
        return erase(it, it + 1);
    }

    T *erase(T const *first, T const *last) noexcept(std::is_nothrow_move_assignable_v<T>)
    {
        size_t idx_first = first - m_data;
        size_t count = last - first;
        if (count == 0)
        {
            return m_data + idx_first;
        }
        T *destination_it = m_data + idx_first;
        T const *source_it = last;
        while (source_it != m_data + m_size)
        {
            *destination_it = std::move(*source_it);
            ++destination_it;
            ++source_it;
        }
        for (size_t i = 0; i < count; ++i)
        {
            std::destroy_at(m_data + m_size - 1 - i);
        }
        m_size -= count;
        return m_data + idx_first;
    }

    void assign(size_t n, T const &val)
    {
        if (n > m_cap)
        {
            Vector temp(n, val, m_alloc);
            swap(temp);
            return;
        }
        else
        {
            if (n < m_size)
            {
                for (size_t i = n; i < m_size; ++i)
                {
                    std::destroy_at(m_data + i);
                }
            }
            for (size_t i = 0; i < n; ++i)
            {
                if (i < m_size) // 对现有元素赋值
                {

                    m_data[i] = val;
                }
                else // 在未构造区域构造
                {
                    std::construct_at(m_data + i, val);
                }
            }
            m_size = n;
        }
    }

    template <std::random_access_iterator InputIt>
    void assign(InputIt first, InputIt last)
    {
        size_t new_size = last - first;
        if (new_size > m_cap)
        {
            Vector temp(first, last, m_alloc);
            swap(temp);
        }
        else
        {
            size_t common_size = std::min(m_size, new_size);
            InputIt it = first;
            for (size_t i = 0; i < common_size; ++i)
            {
                m_data[i] = *it;
                ++it;
            }
            if (new_size > m_size) // 构造新元素
            {
                for (size_t i = m_size; i < new_size; ++i)
                {
                    std::construct_at(m_data + i, *it);
                    ++it;
                }
            }
            else if (new_size < m_size) // 销毁多余元素
            {
                for (size_t i = new_size; i < m_size; ++i)
                {
                    std::destroy_at(m_data + i);
                }
            }
            m_size = new_size;
        }
    }

    void assign(std::initializer_list<T> ilist)
    {
        // 1. 计算新大小
        size_t new_size = ilist.size();
        if (new_size > m_cap)
        {
            Vector temp(ilist, m_alloc);
            swap(temp);
            return;
        }
        size_t common_size = std::min(m_size, new_size);
        auto it = ilist.begin();
        for (size_t i = 0; i < common_size; ++i)
        {
            m_data[i] = *it;
            ++it;
        }
        if (new_size > m_size)
        {
            for (size_t i = m_size; i < new_size; ++i)
            {
                std::construct_at(m_data + i, *it);
                ++it;
            }
        }
        else if (new_size < m_size)
        {
            for (size_t i = new_size; i < m_size; ++i)
            {
                std::destroy_at(m_data + i);
            }
        }
        m_size = new_size;
    }

    Vector &operator=(std::initializer_list<T> ilist)
    {
        assign(ilist);
        return *this;
    }

    T *insert(T const *it, T &&val)
    {
        return insert(it, 1, std::move(val));
    }

    T *insert(T const *it, T const &val)
    {
        return insert(it, 1, val);
    }

    T *insert(T const *it, size_t n, T const &val)
    {
        size_t idx = it - m_data;
        size_t new_size = m_size + n;
        if (n == 0)
        {
            return m_data + idx;
        }
        if (new_size > m_cap)
        {
            size_t new_cap = m_cap;
            while (new_size > new_cap)
            {
                new_cap = (new_cap == 0) ? 1 : new_cap * 2;
            }
            T *new_data = m_alloc.allocate(new_cap);
            size_t i = 0;
            for (; i < idx; ++i)
            {
                std::construct_at(new_data + i, std::move(m_data[i]));
            }
            for (; i < idx + n; ++i)
            {
                std::construct_at(new_data + i, val);
            }
            for (size_t j = idx; j < m_size; ++j)
            {
                std::construct_at(new_data + i, std::move(m_data[j]));
                ++i;
            }
            if (m_data != nullptr)
            {
                for (size_t k = 0; k < m_size; ++k)
                {
                    std::destroy_at(m_data + k);
                }
                m_alloc.deallocate(m_data, m_cap);
            }
            m_data = new_data;
            m_cap = new_cap;
            m_size = new_size;
        }
        else
        {
            for (size_t i = m_size; i > idx; --i)
            {
                std::construct_at(m_data + i + n - 1, std::move(m_data[i - 1]));
                std::destroy_at(m_data + i - 1);
            }
            for (size_t i = 0; i < n; ++i)
            {
                std::construct_at(m_data + idx + i, val);
            }
            m_size = new_size;
        }
        return m_data + idx;
    }

    template <std::random_access_iterator InputIt>
    T *insert(T const *it, InputIt first, InputIt last)
    {
        size_t idx = it - m_data;
        size_t n = last - first;
        size_t new_size = m_size + n;
        if (n == 0)
        {
            return m_data + idx;
        }
        if (new_size > m_cap)
        {
            size_t new_cap = m_cap;
            while (new_size > new_cap)
            {
                new_cap = (new_cap == 0) ? 1 : new_cap * 2;
            }
            T *new_data = m_alloc.allocate(new_cap);
            size_t i = 0;
            for (; i < idx; ++i)
            {
                std::construct_at(new_data + i, std::move(m_data[i]));
            }
            for (; i < idx + n; ++i)
            {
                std::construct_at(new_data + i, *(first + (i - idx)));
            }
            for (size_t j = idx; j < m_size; ++j)
            {
                std::construct_at(new_data + i, std::move(m_data[j]));
                ++i;
            }
            if (m_data != nullptr)
            {
                for (size_t k = 0; k < m_size; ++k)
                {
                    std::destroy_at(m_data + k);
                }
                m_alloc.deallocate(m_data, m_cap);
            }
            m_data = new_data;
            m_cap = new_cap;
            m_size = new_size;
        }
        else
        {
            for (size_t i = m_size; i > idx; --i)
            {
                std::construct_at(m_data + i + n - 1, std::move(m_data[i - 1]));
                std::destroy_at(m_data + i - 1);
            }
            for (size_t i = 0; i < n; ++i)
            {
                std::construct_at(m_data + idx + i, *(first + i));
            }
            m_size = new_size;
        }
        return m_data + idx;
    }

    T *insert(T const *it, std::initializer_list<T> ilist)
    {
        size_t idx = it - m_data;
        size_t n = ilist.size();
        size_t new_size = m_size + n;
        if (n == 0)
        {
            return m_data + idx;
        }
        if (new_size > m_cap)
        {
            size_t new_cap = m_cap;
            while (new_size > new_cap)
            {
                new_cap = (new_cap == 0) ? 1 : new_cap * 2;
            }
            T *new_data = m_alloc.allocate(new_cap);
            size_t i = 0;
            for (; i < idx; ++i)
            {
                std::construct_at(new_data + i, std::move(m_data[i]));
            }
            for (auto it : ilist)
            {
                std::construct_at(new_data + i, it);
                ++i;
            }
            for (size_t j = idx; j < m_size; ++j)
            {
                std::construct_at(new_data + i, std::move(m_data[j]));
                ++i;
            }
            if (m_data != nullptr)
            {
                for (size_t k = 0; k < m_size; ++k)
                {
                    std::destroy_at(m_data + k);
                }
                m_alloc.deallocate(m_data, m_cap);
            }
            m_data = new_data;
            m_cap = new_cap;
            m_size = new_size;
        }
        else
        {
            for (size_t i = m_size; i > idx; --i)
            {
                std::construct_at(m_data + i + n - 1, std::move(m_data[i - 1]));
                std::destroy_at(m_data + i - 1);
            }
            size_t i = 0;
            for (auto it : ilist)
            {
                std::construct_at(m_data + idx + i, it);
                ++i;
            }
            m_size = new_size;
        }
        return m_data + idx;
    }

    ~Vector()
    {
        if (m_data != nullptr)
        {
            clear();
            m_alloc.deallocate(m_data, m_cap);
        }
        m_data = nullptr;
        m_size = 0;
        m_cap = 0;
    }

    bool operator==(Vector const &that) const
    {
        if (m_size != that.m_size)
        {
            return false;
        }
        for (size_t i = 0; i < m_size; ++i)
        {
            if (!(m_data[i] == that.m_data[i]))
            {
                return false;
            }
        }
        return true;
    }
};