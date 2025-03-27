#ifndef DYNAMIC_ARRAY_HPP
#define DYNAMIC_ARRAY_HPP

#include <stdexcept>  // For out_of_range exception
#include <limits>     // For numeric limits

template <typename T>
class DynamicArray {
private:
    T* data;          // Pointer to the dynamically allocated array
    int capacity;     
    int currentSize;  

    // Optional: Maximum capacity to prevent runaway memory allocation.
    const int MAX_CAPACITY = std::numeric_limits<int>::max() / sizeof(T);

    void resize() {
        // Check if doubling the capacity would exceed the maximum capacity.
        if (capacity > MAX_CAPACITY / 2) {
            throw std::runtime_error("Maximum capacity exceeded");
        }

        int newCapacity = capacity * 2;

        T* newData = new T[newCapacity];  // Allocate new array
        for (int i = 0; i < currentSize; i++) {
            newData[i] = data[i];         // Copy existing elements
        }
        delete[] data;                    // Free old array
        data = newData;
        capacity = newCapacity;
    }

public:
    // Constructor with an initial capacity (default 10).
    DynamicArray(int initialCapacity = 10)
        : capacity(initialCapacity), currentSize(0) {
        if (capacity <= 0)
            throw std::invalid_argument("Capacity must be greater than 0");
        data = new T[capacity];
    }

    // Destructor cleans up allocated memory.
    ~DynamicArray() {
        delete[] data;
        data = nullptr;
    }

    // Add an element at the end. Resize if necessary.
    void push_back(const T& element) {
        if (currentSize >= capacity) {
            resize();
        }
        data[currentSize++] = element;
    }

    // Operator[] for read-only access (with bounds checking).
    const T& operator[](int index) const {
        if (index < 0 || index >= currentSize) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    // Operator[] for write-access (with bounds checking).
    T& operator[](int index) {
        if (index < 0 || index >= currentSize) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    // Returns the current number of elements.
    int size() const {
        return currentSize;
    }

    // Checks if the array is empty.
    bool empty() const {
        return currentSize == 0;
    }

    // Resets the array (logical clear, memory remains allocated).
    void clear() {
        currentSize = 0;
    }

    // Iterator for read-write access.
    class Iterator {
    private:
        T* ptr;
    public:
        Iterator(T* p) : ptr(p) {}
        T& operator*() { return *ptr; }
        Iterator& operator++() { ++ptr; return *this; }
        bool operator!=(const Iterator& other) const { return ptr != other.ptr; }
    };

    // Const iterator for read-only access.
    class ConstIterator {
    private:
        const T* ptr;
    public:
        ConstIterator(const T* p) : ptr(p) {}
        const T& operator*() const { return *ptr; }
        ConstIterator& operator++() { ++ptr; return *this; }
        bool operator!=(const ConstIterator& other) const { return ptr != other.ptr; }
    };

    // Returns an iterator pointing to the first element.
    Iterator begin() {
        return Iterator(data);
    }

    // Returns an iterator pointing past the last element.
    Iterator end() {
        return Iterator(data + currentSize);
    }

    // Const iterator versions.
    ConstIterator begin() const {
        return ConstIterator(data);
    }

    ConstIterator end() const {
        return ConstIterator(data + currentSize);
    }
};

#endif // DYNAMIC_ARRAY_HPP
