#ifndef SPARSE_MATRIX_H
#define SPARSE_MATRIX_H

#include <cstddef>
#include <utility>

class SparseMatrix {
private:
    struct Element {
        int row;
        int col;
        double value;
        
        Element(int r, int c, double v) : row(r), col(c), value(v) {}
        
        bool operator==(const Element& other) const {
            return row == other.row && col == other.col;
        }
    };

    Element** elements;     // Array of element pointers
    size_t capacity;       // Current capacity of elements array
    size_t count;         // Number of non-zero elements
    
    static const size_t INITIAL_CAPACITY = 16;
    
    class Iterator {
    private:
        const SparseMatrix* matrix;
        size_t current;
        
    public:
        Iterator(const SparseMatrix* m, bool end = false) 
            : matrix(m), current(end ? matrix->count : 0) {}
        
        Iterator& operator++() {
            if (current < matrix->count) current++;
            return *this;
        }
        
        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }
        
        std::pair<std::pair<int, int>, double> operator*() const {
            const Element* elem = matrix->elements[current];
            return {{elem->row, elem->col}, elem->value};
        }
    };

public:
    SparseMatrix() : elements(new Element*[INITIAL_CAPACITY]), 
                     capacity(INITIAL_CAPACITY), 
                     count(0) {}
    
    ~SparseMatrix() {
        clear();
        delete[] elements;
    }
    
    // Copy constructor
    SparseMatrix(const SparseMatrix& other) 
        : elements(new Element*[other.capacity]),
          capacity(other.capacity),
          count(0) {
        for (size_t i = 0; i < other.count; ++i) {
            set(other.elements[i]->row, 
                other.elements[i]->col, 
                other.elements[i]->value);
        }
    }
    
    // Move constructor
    SparseMatrix(SparseMatrix&& other) noexcept
        : elements(other.elements),
          capacity(other.capacity),
          count(other.count) {
        other.elements = nullptr;
        other.capacity = 0;
        other.count = 0;
    }
    
    // Assignment operator
    SparseMatrix& operator=(SparseMatrix other) {
        swap(*this, other);
        return *this;
    }
    
    friend void swap(SparseMatrix& first, SparseMatrix& second) noexcept {
        using std::swap;
        swap(first.elements, second.elements);
        swap(first.capacity, second.capacity);
        swap(first.count, second.count);
    }

    void set(int row, int col, double value) {
        // Search for existing element
        for (size_t i = 0; i < count; ++i) {
            if (elements[i]->row == row && elements[i]->col == col) {
                if (value == 0.0) {
                    // Remove element
                    delete elements[i];
                    for (size_t j = i; j < count - 1; ++j) {
                        elements[j] = elements[j + 1];
                    }
                    count--;
                } else {
                    elements[i]->value = value;
                }
                return;
            }
        }
        
        if (value != 0.0) {
            if (count == capacity) {
                resize(capacity * 2);
            }
            elements[count++] = new Element(row, col, value);
        }
    }

    double get(int row, int col) const {
        for (size_t i = 0; i < count; ++i) {
            if (elements[i]->row == row && elements[i]->col == col) {
                return elements[i]->value;
            }
        }
        return 0.0;
    }

    size_t nonZeroCount() const {
        return count;
    }
    
    // Iterator support
    Iterator begin() const { return Iterator(this); }
    Iterator end() const { return Iterator(this, true); }
    
    // Transpose the matrix
    SparseMatrix transpose() const {
        SparseMatrix result;
        for (size_t i = 0; i < count; ++i) {
            result.set(elements[i]->col, 
                      elements[i]->row, 
                      elements[i]->value);
        }
        return result;
    }

private:
    void clear() {
        for (size_t i = 0; i < count; ++i) {
            delete elements[i];
        }
        count = 0;
    }
    
    void resize(size_t newCapacity) {
        Element** newElements = new Element*[newCapacity];
        for (size_t i = 0; i < count; ++i) {
            newElements[i] = elements[i];
        }
        delete[] elements;
        elements = newElements;
        capacity = newCapacity;
    }
};

#endif // SPARSE_MATRIX_H