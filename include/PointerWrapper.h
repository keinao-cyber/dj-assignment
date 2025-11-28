#ifndef POINTERWRAPPER_H
#define POINTERWRAPPER_H

#include <utility>
#include <iostream>

/**
 * PointerWrapper - A template class that wraps a raw pointer
 * 
 * This is Phase 3 of the assignment. Students must analyze and implement
 * a complete pointer wrapper class that manages dynamic memory safely.
 * 
 * Refer to the assignment instructions (Phase 3) for detailed guiding questions
 * about resource management, ownership semantics, copy vs move, and interface design.
 */
template<typename T>
class PointerWrapper {
private:
    T* ptr;  // Raw pointer to the managed object

public:
    // ========== CONSTRUCTION AND DESTRUCTION ==========

    /**
     * Default constructor - creates empty wrapper
     */
    PointerWrapper() : ptr(nullptr) {}

    /**
     * Constructor from raw pointer - wraps the pointer
     */
    explicit PointerWrapper(T* p) : ptr(p) {}

    /**
     * TODO: Implement destructor
     * HINT: What should happen to the wrapped pointer when the wrapper is destroyed?
     * Think about ownership and resource management.
     * Is the default destructor sufficient here?
     */

     /*
     - The Default Destructor only cleans up the member variables of the class itself. 
     - Since ptr is just a simple memory address (an integer, essentially), the default destructor simply removes that integer from memory.
     - It does not follow that address to the Heap to actually free the allocated memory.
    */


    ~PointerWrapper() {
        delete ptr; 
    }

    // ========== COPY OPERATIONS (DELETED) ==========

    /**
     * Copy constructor is DELETED
     * Think about why this might be necessary for a pointer wrapper
     */
    PointerWrapper(const PointerWrapper& other) = delete;

    /**
     * Copy assignment is DELETED
     * Consider what problems could arise if copying was allowed
     */
    PointerWrapper& operator=(const PointerWrapper& other) = delete;

    // ========== MOVE OPERATIONS (STUDENTS IMPLEMENT) ==========

    /**
     * TODO: Implement move constructor
     * HINT: How should ownership transfer from one wrapper to another?
     * What should happen to the source wrapper after the move?
     */

    /*
     Move constructor - transfers ownership from 'other' to this
     */
    PointerWrapper(PointerWrapper&& other) noexcept : ptr(other.ptr) {
        other.ptr = nullptr;
    }

    /**
     * TODO: Implement move assignment operator
     * HINT: Handle cleanup of current resource and ownership transfer
     * Don't forget about self-assignment!
     */
    PointerWrapper& operator=(PointerWrapper&& other) noexcept {

        // 1. Check for self-assignment
        if (this == &other) {
            return *this;
        }

        // 2. Clean up the current resource
        delete ptr;

        // 3. Transfer ownership
        ptr = other.ptr;
        other.ptr = nullptr;

        // 4. Return reference to self
        return *this;
    }

    // ========== ACCESS OPERATIONS ==========

    /**
     * TODO: Implement dereference operator
     * HINT: How do you access the object that the wrapper points to? Is this operation implementation complete?
     * @throws std::runtime_error if ptr is null
     */

    T& operator*() const {
        if (ptr == nullptr) {
            throw std::runtime_error("Null Pointer Exception: Attempted to dereference null pointer");
        }

        return *ptr;
    };

    /**
     * TODO: Implement arrow operator
     * HINT: How do you access members of the wrapped object?
     * What safety checks should you perform?
     */
    T* operator->() const {
        if (ptr == nullptr) {
            throw std::runtime_error("Null Pointer Exception: Attempted to access member of null pointer");
        }
        return ptr;
    }

    /**
     * TODO: Implement get() function
     * HINT: Sometimes you need access to the raw pointer without changing ownership
     * What should this function return?
     * @throws std::runtime_error if ptr is null
     */
    T* get() const {
        if (ptr == nullptr) {
            throw std::runtime_error("Null Pointer Exception: Attempted to get value of null pointer");
        }

        return ptr; 
    }

    // ========== OWNERSHIP MANAGEMENT ==========

    /**
     * TODO: Implement release() function
     * HINT: What does "release" mean in terms of ownership?
     * Should the wrapper still own the pointer after calling release()?
     */


    /**
     * release() - relinquishes ownership of the pointer
     * Returns the raw pointer and sets internal pointer to null.
     * The wrapper NO LONGER owns the object (it won't delete it).
     */

    T* release() {
        T* temp = ptr;
        ptr = nullptr;
        return temp;
    }

    /**
     * TODO: Implement reset() function
     * HINT: How do you replace the currently wrapped pointer?
     * What should happen to the old pointer?
     */
    void reset(T* new_ptr = nullptr) {
        // Prevent self-reset (e.g., reset(get()))
        if (ptr != new_ptr) {
            delete ptr;    // Destroy the old object
            ptr = new_ptr; // Take ownership of the new one
        }
    }

    // ========== UTILITY FUNCTIONS ==========

    /**
     * TODO: Implement boolean conversion operator
     * HINT: When should a wrapper be considered "true" or "false"?
     * Why might the explicit keyword be important here?
     */
    explicit operator bool() const {
        return ptr != nullptr;
    }

    /**
     * Swap two PointerWrapper objects
     * This is implemented for you as a reference
     */
    void swap(PointerWrapper& other) noexcept {
        std::swap(ptr, other.ptr);
    }
};

// ========== NON-MEMBER FUNCTIONS ==========

/**
 * Helper function to create PointerWrapper
 * This is implemented for you as an example
 * Can you figure out when this would be useful in phase 4?
 */
template<typename T, typename... Args>
PointerWrapper<T> make_pointer_wrapper(Args&&... args) {
    return PointerWrapper<T>(new T(std::forward<Args>(args)...));
}

/**
 * TODO: Implement swap for PointerWrapper
 * HINT: How can you swap two wrapper objects?
 * Why might this be useful?
 */
template<typename T>
void swap(PointerWrapper<T>& lhs, PointerWrapper<T>& rhs) noexcept {
    // TODO: Implement global swap function
    // HINT: You can use the member swap function
    //your code here...
    lhs.swap(rhs);
}

#endif // POINTERWRAPPER_H