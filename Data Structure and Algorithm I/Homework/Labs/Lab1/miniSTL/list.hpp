#pragma once

#include <cstddef>
#include <iterator>
#include <memory>
#include <limits>
#include <stdexcept>
#include <utility>
#include <compare>
#include <initializer_list>

template <class T>
struct ListBaseNode
{
    ListBaseNode *m_next;
    ListBaseNode *m_prev;
};

template <class T>
struct ListValueNode : ListBaseNode<T>
{
    T m_value;
    template <typename... Args>
    ListValueNode(Args &&...args) : m_value(std::forward<Args>(args)...) {}
    inline T &value() { return m_value; }
    inline T const &value() const { return m_value; }
};

template <class T, class Alloc = std::allocator<T>>
struct List
{
    using value_type = T;
    using allocator_type = Alloc;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using pointer = T *;
    using const_pointer = T const *;
    using reference = T &;
    using const_reference = T const &;

private:
    using ListNode = ListBaseNode<T>;
    using AllocNode = std::allocator_traits<Alloc>::template rebind_alloc<ListValueNode<T>>;

    ListNode m_dummy;
    size_t m_size;
    [[no_unique_address]] Alloc m_alloc;

public:
    List()
        : m_size(0)
    {
        m_dummy.m_next = &m_dummy;
        m_dummy.m_prev = &m_dummy;
    }

    List(List &&that)
        : m_size(that.m_size),
          m_alloc(std::move(that.m_alloc))
    {
        if (that.m_size > 0)
        {
            m_dummy.m_next = that.m_dummy.m_next;
            m_dummy.m_prev = that.m_dummy.m_prev;

            m_dummy.m_next->m_prev = &m_dummy;
            m_dummy.m_prev->m_next = &m_dummy;

            that.m_dummy.m_next = &that.m_dummy;
            that.m_dummy.m_prev = &that.m_dummy;
            that.m_size = 0;
        }
        else
        {
            m_dummy.m_next = &m_dummy;
            m_dummy.m_prev = &m_dummy;
        }
    }

    List(List const &that)
        : m_size(0),
          m_alloc(that.m_alloc)
    {
        m_dummy.m_next = &m_dummy;
        m_dummy.m_prev = &m_dummy;
        ListNode *it = &that.m_dummy;
        while (it->m_next != &that.m_dummy)
        {
            it = it->m_next;
            push_back(it->value());
        }
    }

    List &operator=(List const &that)
    {
        m_size = 0;
        m_alloc = that.m_alloc;
        m_dummy.m_next = &m_dummy;
        m_dummy.m_prev = &m_dummy;
        ListNode *it = &that.m_dummy;
        while (it->m_next != &that.m_dummy)
        {
            it = it->m_next;
            push_back(it->value());
        }
        return *this;
    }

    bool empty() { return m_size == 0; }

    T &front() { return static_cast<ListValueNode<T> *>(m_dummy.m_next)->value(); }

    T &back() { return static_cast<ListValueNode<T> *>(m_dummy.m_prev)->value(); }

    T const &front() const { return static_cast<ListValueNode<T> *>(m_dummy.m_next)->value(); };

    T const &back() const { return static_cast<ListValueNode<T> *>(m_dummy.m_prev)->value(); };

    explicit List(size_t n, Alloc const &alloc = Alloc())
        : m_size(0),
          m_alloc(alloc)
    {
        m_dummy.m_next = &m_dummy;
        m_dummy.m_prev = &m_dummy;

        if (n == 0)
        {
            return;
        }
        for (size_t i = 0; i < n; ++i)
        {
            ListValueNode<T> *new_node = new ListValueNode<T>();
            ListNode *prev_node = m_dummy.m_prev;
            new_node->m_prev = prev_node;
            new_node->m_next = &m_dummy;
            prev_node->m_next = new_node;
            m_dummy.m_prev = new_node;
            ++m_size;
        }
    }

    List(size_t n, T const &val, Alloc const &alloc = Alloc())
        : m_size(0),
          m_alloc(alloc)
    {
        m_dummy.m_next = &m_dummy;
        m_dummy.m_prev = &m_dummy;
        for (size_t i = 0; i < n; ++i)
        {
            push_back(val);
        }
    }

    template <std::input_iterator InputIt>
    List(InputIt first, InputIt last, Alloc const &alloc = Alloc())
        : m_size(0),
          m_alloc(alloc)
    {
        m_dummy.m_next = &m_dummy;
        m_dummy.m_prev = &m_dummy;
        for (InputIt it = first; it != last; ++it)
        {
            push_back(*it);
        }
    }

    List(std::initializer_list<T> ilist, Alloc const &alloc = Alloc())
        : m_size(0),
          m_alloc(alloc)
    {
        m_dummy.m_next = &m_dummy;
        m_dummy.m_prev = &m_dummy;
        for (auto it : ilist)
        {
            push_back(it);
        }
    }

    List &operator=(std::initializer_list<T> ilist)
    {
        m_size = 0;
        m_dummy.m_next = &m_dummy;
        m_dummy.m_prev = &m_dummy;
        for (auto it : ilist)
        {
            push_back(it);
        }
        return *this;
    }

    size_t size() { return m_size; }

    template <std::input_iterator InputIt>
    void assign(InputIt first, InputIt last)
    {
        clear();
        for (InputIt it = first; it != last; ++it)
        {
            push_back(*it);
        }
    }

    void assign(std::initializer_list<T> ilist)
    {
        clear();
        for (auto it : ilist)
        {
            push_back(it);
        }
    }

    void assign(size_t n, T const &val)
    {
        clear();
        for (size_t i = 0; i < n; ++i)
        {
            push_back(val);
        }
    }

    void push_back(T const &val)
    {
        ListValueNode<T> *new_node = new ListValueNode<T>();
        new_node->m_value = val;
        ListNode *prev_node = m_dummy.m_prev;
        new_node->m_prev = prev_node;
        new_node->m_next = &m_dummy;
        prev_node->m_next = new_node;
        m_dummy.m_prev = new_node;
        ++m_size;
    }

    void push_back(T &&val)
    {
        ListValueNode<T> *new_node = new ListValueNode<T>();
        new_node->m_value = std::move(val);
        ListNode *prev_node = m_dummy.m_prev;
        new_node->m_prev = prev_node;
        new_node->m_next = &m_dummy;
        prev_node->m_next = new_node;
        m_dummy.m_prev = new_node;
        ++m_size;
    }

    void push_front(T const &val)
    {
        ListValueNode<T> *new_node = new ListValueNode<T>();
        new_node->m_value = val;
        ListNode *next_node = m_dummy.m_next;
        new_node->m_prev = &m_dummy;
        new_node->m_next = next_node;
        next_node->m_prev = new_node;
        m_dummy.m_next = new_node;
        ++m_size;
    }

    void push_front(T &&val)
    {
        ListValueNode<T> *new_node = new ListValueNode<T>();
        new_node->m_value = std::move(val);
        ListNode *next_node = m_dummy.m_next;
        new_node->m_prev = &m_dummy;
        new_node->m_next = next_node;
        next_node->m_prev = new_node;
        m_dummy.m_next = new_node;
        ++m_size;
    }

    ~List()
    {
        clear();
    }

    void clear()
    {
        auto it = m_dummy.m_next;
        while (it != &m_dummy)
        {
            auto delete_node = it;
            it = it->m_next;
            delete delete_node;
        }
        m_dummy.m_next = &m_dummy;
        m_dummy.m_prev = &m_dummy;
        m_size = 0;
    }

    struct iterator
    {
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = ptrdiff_t;
        using pointer = T *;
        using reference = T &;

    private:
        ListNode *m_curr;

        friend List;

        explicit iterator(ListNode *curr) : m_curr(curr) {}

    public:
        iterator() = default;

        iterator &operator++() // 前置++
        {
            m_curr = m_curr->m_next;
            return *this;
        }

        iterator operator++(int) // 后置++
        {
            iterator tmp = *this;
            m_curr = m_curr->m_next;
            return tmp;
        }

        iterator &operator--()
        {
            m_curr = m_curr->m_prev;
            return *this;
        }

        iterator operator--(int)
        {
            iterator tmp = *this;
            m_curr = m_curr->m_prev;
            return tmp;
        }

        T &operator*() { return static_cast<ListValueNode<T> *>(m_curr)->value(); } // 解引用

        bool operator!=(iterator const &that) { return m_curr != that.m_curr; }

        bool operator==(iterator const &that) { return m_curr == that.m_curr; }
    };

    struct const_iterator
    {
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = ptrdiff_t;
        using pointer = T const *;
        using reference = T const &;

    private:
        ListNode const *m_curr;

        friend List;

        explicit const_iterator(ListNode const *curr) : m_curr(curr) {}

    public:
        const_iterator() = default;

        const_iterator(iterator that) : m_curr(that.m_curr) {};

        explicit operator iterator()
        {
            return iterator(const_cast<ListNode *>(m_curr));
        }

        const_iterator &operator++()
        {
            m_curr = m_curr->m_next;
            return *this;
        }

        const_iterator operator++(int)
        {
            const_iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        const_iterator &operator--()
        {
            m_curr = m_curr->m_prev;
            return *this;
        }

        const_iterator operator--(int)
        {
            const_iterator tmp = *this;
            --(*this);
            return tmp;
        }

        T const &operator*() { return static_cast<ListValueNode<T> const *>(m_curr)->value(); }

        bool operator!=(const_iterator const &that) { return m_curr != that.m_curr; }

        bool operator==(const_iterator const &that) { return m_curr == that.m_curr; }
    };

    iterator begin() { return iterator(m_dummy.m_next); }

    iterator end() { return iterator(&m_dummy); }

    const_iterator begin() const { return const_iterator(m_dummy.m_next); }

    const_iterator end() const { return const_iterator(&m_dummy); }

    const_iterator cbegin() const { return begin(); }

    const_iterator cend() const { return end(); }

    using reverse_iterator = std::reverse_iterator<iterator>;
    using reverse_const_iterator = std::reverse_iterator<const_iterator>;

    reverse_iterator rbegin() { return reverse_iterator(end()); }

    reverse_iterator rend() { return reverse_iterator(begin()); }

    reverse_const_iterator crbegin() { return const_reverse_iterator(end()); }

    reverse_const_iterator crend() { return const_reverse_iterator(begin()); }

    reverse_const_iterator rbegin() const { return const_reverse_iterator(end()); }

    reverse_const_iterator rend() const { return const_reverse_iterator(begin()); }

    iterator erase(const_iterator pos)
    {
        const_iterator next_pos = pos;
        ++next_pos;
        return erase(pos, next_pos);
    }

    iterator erase(const_iterator first, const_iterator last)
    {
        if (first == last)
        {
            return iterator(const_cast<ListNode *>(last.m_curr));
        }
        ListNode *prev_first = first.m_curr->m_prev;
        ListNode *curr = const_cast<ListNode *>(first.m_curr);
        ListNode *stop = const_cast<ListNode *>(last.m_curr);
        while (curr != stop)
        {
            ListNode *next = curr->m_next;
            curr->m_prev->m_next = curr->m_next;
            curr->m_next->m_prev = curr->m_prev;
            delete static_cast<ListValueNode<T> *>(curr);
            curr = next;
            --m_size;
        }
        return iterator(const_cast<ListNode *>(last.m_curr));
    }

    void pop_front() { erase(begin()); }

    void pop_back() { erase(std::prev(end())); }

    iterator insert(const_iterator pos, const T &val) { return insert(pos, 1, val); }

    iterator insert(const_iterator pos, T &&val)
    {
        T tmp = std::move(val);
        return insert(pos, 1, tmp);
    }

    iterator insert(const_iterator pos, size_t n, T const &val)
    {
        if (n == 0)
        {
            return iterator(const_cast<ListNode *>(pos.m_curr));
        }

        ListNode *next_node = const_cast<ListNode *>(pos.m_curr);
        ListNode *prev_node = next_node->m_prev;
        ListNode *first_new = nullptr;
        ListNode *last_new = nullptr;

        for (size_t i = 0; i < n; ++i)
        {
            ListValueNode<T> *new_node = new ListValueNode<T>(val);
            if (!first_new)
            {
                first_new = new_node;
            }
            else
            {
                last_new->m_next = new_node;
                new_node->m_prev = last_new;
            }
            last_new = new_node;
        }

        first_new->m_prev = prev_node;
        last_new->m_next = next_node;
        prev_node->m_next = first_new;
        next_node->m_prev = last_new;

        m_size += n;

        return iterator(first_new);
    }

    template <std::input_iterator InputIt>
    iterator insert(const_iterator pos, InputIt first, InputIt last)
    {
        if (first == last)
        {
            return iterator(const_cast<ListNode *>(pos.m_curr));
        }
        ListNode *next_node = const_cast<ListNode *>(pos.m_curr);
        ListNode *prev_node = next_node->m_prev;

        ListNode *first_new = nullptr;
        ListNode *last_new = nullptr;
        for (auto it = first; it != last; ++it)
        {
            ListValueNode<T> *new_node = new ListValueNode<T>(*it);

            if (!first_new)
            {
                first_new = new_node;
            }
            else
            {
                last_new->m_next = new_node;
                new_node->m_prev = last_new;
            }
            last_new = new_node;
        }
        first_new->m_prev = prev_node;
        last_new->m_next = next_node;
        prev_node->m_next = first_new;
        next_node->m_prev = last_new;

        size_t n = std::distance(first, last);
        m_size += n;

        return iterator(first_new);
    }

    iterator insert(const_iterator pos, std::initializer_list<T> ilist)
    {
        return insert(pos, ilist.begin(), ilist.end());
    }

    bool operator==(List const &that) const
    {
        if (this->m_size != that.m_size)
        {
            return false;
        }
        auto it1 = this->begin();
        auto it2 = that.begin();
        auto end = this->end();
        while (it1 != end)
        {
            if (*it1 != *it2)
            {
                return false;
            }
            ++it1;
            ++it2;
        }
        return true;
    }
};