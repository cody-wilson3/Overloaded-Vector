#pragma once

// #include <cstddef>
#include <functional>
#include <initializer_list>
#include <memory>
#include <stdexcept>

namespace usu
{

    template <typename T>
    class vector
    {
      public:
        using size_type = std::size_t;
        using reference = T&;
        using pointer = std::shared_ptr<T[]>;
        using value_type = T;
        using resize_type = std::function<size_type(size_type)>;

        // Declarations
        vector();
        vector(size_type size);
        vector(resize_type resize);
        vector(size_type size, resize_type resize);
        vector(std::initializer_list<T> list);
        vector(std::initializer_list<T> list, resize_type resize);
        reference operator[](size_type index);
        void add(T value);
        void insert(size_type index, T value);
        void remove(size_type index);
        void clear();
        void resize();
        size_type size() const { return sizeVal; }
        size_type capacity() const { return capacityVal; }

        // Iterator -----------------------------------------------------------------------------------
        class iterator
        {
          public:
            using iterator_category = std::forward_iterator_tag;

            // Declarations
            iterator();
            iterator(pointer ptr);
            iterator(size_type pos, pointer ptr);

            // SLIDE 21
            iterator(const iterator& obj);
            iterator(iterator&& obj) noexcept;

            // SLIDE 23
            iterator operator++();
            iterator operator--();
            iterator operator++(int);
            iterator operator--(int);

            // SLIDE 25
            iterator& operator=(const iterator& rhs);
            iterator& operator=(iterator&& rhs);

            reference operator*();
            T* operator->() const;
            bool operator==(const iterator& other) const;
            bool operator!=(const iterator& other) const;

          private:
            size_type pos;
            pointer data;
        }; // iterator class

        iterator begin();
        iterator end();

      private:
        size_type capacityVal;
        size_type sizeVal;
        pointer data;
        resize_type resize_func_;
    }; // vector class

    // []
    template <typename T>
    typename vector<T>::reference vector<T>::operator[](size_type index)
    {
        if (index >= sizeVal)
        {
            throw std::range_error("Index out of bounds");
        }
        return data[index];
    }

    // begin()
    template <typename T>
    vector<T>::iterator vector<T>::begin()
    {
        return iterator(data);
    }

    // end()
    template <typename T>
    vector<T>::iterator vector<T>::end()
    {
        return iterator(sizeVal, data);
    }

    // ---------------------------------------------------------- Vector Constructors --------------------------------
    template <typename T>
    vector<T>::vector()
    {
        capacityVal = 10;
        sizeVal = 0;
        data = std::make_shared<T[]>(capacityVal);
        resize_func_ = [](size_type currentCapacity)
        {
            return currentCapacity * 2;
        };
    }

    template <typename T>
    vector<T>::vector(size_type incomingSize) :
        vector()
    {
        sizeVal = incomingSize;
        if (incomingSize > 10)
        {
            capacityVal = incomingSize * 2;
        }
    }

    template <typename T>
    vector<T>::vector(resize_type resize) :
        vector()
    {
        resize_func_ = resize;
    }

    template <typename T>
    vector<T>::vector(size_type initialSize, resize_type resize) :
        vector()
    {
        if (initialSize > 10)
        {
            capacityVal = initialSize * 2;
        }
        sizeVal = initialSize;
        resize_func_ = resize;
    }

    template <typename T>
    vector<T>::vector(std::initializer_list<T> list) :
        vector()
    {
        for (const auto& item : list)
        {
            add(item);
        }
    }

    template <typename T>
    vector<T>::vector(std::initializer_list<T> list, resize_type resize) :
        vector()
    {
        resize_func_ = resize;
        for (const auto& item : list)
        {
            add(item);
        }
    }

    template <typename T>
    void vector<T>::resize()
    {
        capacityVal = resize_func_(sizeVal);
        auto new_data = std::make_shared<T[]>(capacityVal);
        for (size_type i = 0; i < sizeVal; ++i)
        {
            new_data[i] = data[i];
        }
        data = (new_data);
    }

    template <typename T>
    void vector<T>::add(T value)
    {
        if (sizeVal == capacityVal)
        {
            resize();
        }
        data[sizeVal] = value;
        sizeVal++;
    }

    template <typename T>
    void vector<T>::insert(size_type index, T value)
    {
        if (index > sizeVal)
        {
            throw std::range_error("Index out of bounds");
        }
        if (sizeVal == capacityVal)
        {
            resize();
        }
        for (size_type i = sizeVal; i > index; --i)
        {
            data[i] = data[i - 1];
        }
        data[index] = value;
        ++sizeVal;
    }

    template <typename T>
    void vector<T>::remove(size_type index)
    {
        if (index >= sizeVal)
        {
            throw std::range_error("Index out of bounds");
        }
        for (size_type i = index; i < sizeVal - 1; ++i)
        {
            data[i] = data[i + 1];
        }
        --sizeVal;
    }

    template <typename T>
    void vector<T>::clear()
    {
        sizeVal = 0;
    }

    template <typename T>
    vector<T>::iterator::iterator() :
        iterator(nullptr)
    {
    }

    template <typename T>
    vector<T>::iterator::iterator(pointer ptr)
    {
        data = ptr;
        pos = 0;
    }

    template <typename T>
    vector<T>::iterator::iterator(size_type newPos, pointer ptr)
    {
        data = ptr;
        pos = newPos;
    }

    // SLIDE 24
    // ++
    template <typename T>
    typename vector<T>::iterator vector<T>::iterator::operator++()
    {
        pos++;
        return *this;
    }

    // --
    template <typename T>
    typename vector<T>::iterator vector<T>::iterator::operator--()
    {
        pos--;
        return *this;
    }

    // SLIDE 24
    // --
    template <typename T>
    typename vector<T>::iterator vector<T>::iterator::operator++(int)
    {
        iterator i = *this;
        pos++;
        return i;
    }

    // --
    template <typename T>
    typename vector<T>::iterator vector<T>::iterator::operator--(int)
    {
        iterator i = *this;
        pos--;
        return i;
    }

    // Copy Assignable FROM SLIDE 26
    // =
    template <typename T>
    typename vector<T>::iterator& vector<T>::iterator::operator=(const iterator& rhs)
    {
        this->pos = rhs.pos;
        this->data = rhs.data;
        return *this;
    }

    // Copy Constructor SLIDE 22
    template <typename T>
    vector<T>::iterator::iterator(const iterator& obj)
    {
        this->pos = obj.pos;
        this->data = obj.data;
    }

    // Move Constructor SLIDE 22
    template <typename T>
    vector<T>::iterator::iterator(iterator&& obj) noexcept
    {
        this->pos = obj.pos;
        this->data = obj.data;
        obj.pos = 0;
        obj.data = nullptr;
    }

    // Move Assignable FROM SLIDE 26
    // =
    template <typename T>
    typename vector<T>::iterator& vector<T>::iterator::operator=(iterator&& rhs)
    {
        if (this != &rhs)
        {
            std::swap(this->pos, rhs.pos);
            std::swap(this->data, rhs.data);
        }
    }

    // Dereferenceable FROM SLIDE 27
    template <typename T>
    vector<T>::reference vector<T>::iterator::operator*()
    {
        return data[pos];
    }

    // ->
    template <typename T>
    T* vector<T>::iterator::operator->() const
    {
        return &(data[pos]);
    }

    // FROM SLIDE 28
    // ==
    template <typename T>
    bool vector<T>::iterator::operator==(const iterator& other) const
    {
        return pos == other.pos;
    }

    // FROM SLIDE 28
    // !=
    template <typename T>
    bool vector<T>::iterator::operator!=(const iterator& other) const
    {
        return pos != other.pos;
    }
} // namespace usu