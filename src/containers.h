#pragma once
#include <stdlib.h>
#include <stdio.h>

int min(int a, int b)
{
    return (a > b) ? b : a;
}

template<typename T>
struct Array
{
    T *data = 0;
    int size = 0;
    int capacity = 0;

    inline void resize(int new_capacity)
    {
        T *new_data = (T *)malloc(new_capacity*sizeof(*new_data));

        if(data)
        {
            for(int i = 0; i < min(size, new_capacity); i++)
            {
                new_data[i] = data[i];
            }
            free(data);
        }

        data = new_data;
        capacity = new_capacity;
        size = min(new_capacity, size);
    }

    inline void append(T x)
    {
        if(capacity == 0)
        {
            resize(16);
        }
        else if(size > capacity)
        {
            resize(capacity*2);
        }
        data[size++] = x;
    }

    inline void remove(int i)
    {
        for(int j = i; j < size-1; j++)
        {
            data[j] = data[j+1];
        }
        size--;
    }

    inline void pop()
    {
        remove(size-1);
    }

    inline void clear()
    {
        size = 0;
    }

    inline void destroy()
    {
        clear();
        free(*data);
    }

    T &operator[](int i)
    {
        return data[i];
    }

    T operator[](int i) const
    {
        return data[i];
    }
};