#include <memory>
#include <atomic>
#include <utility>
#include <type_traits>
#include <stdint.h>

#include <iostream>

// TODO: rename this general namespace
namespace my_std {
namespace internal {

template <typename T>
struct ControlBlockBase {
    using counter_t = std::atomic<uint32_t>;

    alignas(64) counter_t shared_cnt = 0;
    alignas(64) counter_t weak_cnt   = 0;
    
    virtual const T* getPtr() const = 0;
    virtual T* getPtr() = 0;
    virtual void destroy() = 0;
    // TODO: rename this
    virtual void detach() = 0;
};

template <typename T>
struct ControlBlock : ControlBlockBase<T> {
    T* data_ = nullptr;

    explicit ControlBlock(T* ptr): data_{ptr} {}

    const T* getPtr() const final { return data_; }
    T* getPtr() final { return data_; }
    void destroy() final { delete data_; }
    void detach() final { delete this; }
};

struct cblock_tag_alloc_hint {};

template <typename T>
struct ControlBlockAllocHint : ControlBlockBase<T> {
    T data_;

    template <typename... Args>
    explicit ControlBlockAllocHint(cblock_tag_alloc_hint, Args&&... args): data_(std::forward<Args>(args)...) {}

    const T* getPtr() const final { return &data_; }
    T* getPtr() final { return &data_; }
    void destroy() final { /* data_ will be destroed automatically */ }
    void detach() final { delete this; }
};

}

template <typename T>
class WeakPtr;

template <typename T>
class SharedPtr {
public:
    template <typename U, typename... Args>
    friend SharedPtr<U> make_shared(Args&&...);

    template <typename U>
    friend class WeakPtr;

    constexpr SharedPtr() noexcept {
        std::cout << "init\n";
    }
    SharedPtr(T* raw_pointer): data_{raw_pointer}, cblock_{new internal::ControlBlock{raw_pointer}} {
        // To deallocate memory once if weak_ptr is still exists when shared is dead
        //cblock_->data_ = data_;
        cblock_->shared_cnt++;
    }

    template <typename Y>
    explicit SharedPtr(Y* ptr) noexcept: data_{ptr}, cblock_{new internal::ControlBlock<T>()} {
        static_assert(std::is_convertible_v<Y*, T*>, "Y should be convertible to T!");
        cblock_->data_ = data_;
        cblock_->shared_cnt++;
    }

    ~SharedPtr() noexcept {
        // Already deleted by another shared ptr or created from nullptr
        destroy();
    }

    SharedPtr(const SharedPtr& ptr) noexcept: data_{ptr.data_}, cblock_{ptr.cblock_} {
        cblock_->shared_cnt++;
    }

    template <typename Y>
    SharedPtr(const SharedPtr<Y>& ptr) noexcept {
        static_assert(std::is_convertible_v<Y*, T*>, "Y should be convertible to T!");

        data_ = ptr.data_;
        cblock_ = ptr.cblock_;
        cblock_->shared_cnt++;
    }

    SharedPtr(SharedPtr&& ptr) noexcept: data_{std::exchange(ptr.data_, nullptr)}, cblock_{std::exchange(ptr.cblock_, nullptr)} {}

    template <typename Y>
    SharedPtr(SharedPtr<Y>&& ptr) noexcept {
        static_assert(std::is_convertible_v<Y*, T*>, "Y should be convertible to T!");

        data_ = std::exchange(ptr.data_, nullptr);
        cblock_ = std::exchange(ptr.cblock_, nullptr);
    }

    T* operator->() noexcept { return data_; }
    const T* operator->() const noexcept { return data_; }

    T& operator*() noexcept { return *data_; }
    const T& operator*() const noexcept { return *data_; }

    template <typename T1 = T, std::enable_if_t<std::is_array_v<T1>, bool> = true>
    T1& operator[](std::ptrdiff_t idx) const {
        return data_[idx];
    }
    operator bool() const noexcept { return !empty(); }

    T*   get() noexcept { return data_; }
    const T* get() const noexcept { return data_; }

    bool empty() const noexcept { return data_ == nullptr; }
    uint32_t getUseCount() const noexcept {
        auto val = empty() ? 0 : cblock_->shared_cnt.load();
        return val;
    }

private:
    void destroy() {
        if (!cblock_) {
            return;
        }
        auto prev_value = std::atomic_fetch_sub(&cblock_->shared_cnt, 1);
        std::cout << prev_value << '\n';
        // Case 1. Another shared_ptr should delete data_
        if (prev_value != 1) {
            return;
        }
        // Case 2.
        if (prev_value == 1) {
            // We shouldn't delete anything if weak_ptr is still alive
            if (cblock_->weak_cnt.load() > 0) {
                return;
            }
            // Dstr() of ControlBlock should free data_ memory
            cblock_->destroy();
            cblock_->detach();
            data_ = nullptr;
        }
    }
    

    // TODO: add tag here
    template <typename... Args>
    SharedPtr(Args&&... args): data_{}, cblock_(new internal::ControlBlockAllocHint<T>(internal::cblock_tag_alloc_hint{}, std::forward<Args>(args)...)) {
        data_ = cblock_->getPtr();
        cblock_->shared_cnt++;
    }

    T* data_ = nullptr;
    internal::ControlBlockBase<T>* cblock_ = nullptr;
};

template <typename T, typename... Args>
SharedPtr<T> make_shared(Args&&... args) {
    return SharedPtr<T>(std::forward<Args>(args)...);
}

template <typename T>
class WeakPtr {
public:

private:
    internal::ControlBlockBase* cblock_ = nullptr;
};

}