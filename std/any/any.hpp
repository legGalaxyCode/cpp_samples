#include <typeinfo>
#include <type_traits>
#include <cstdint>

namespace internal {

// template <typename T>
// const T& any_cast();

class any {
public:
    template <typename T>
    friend const T& any_cast(const any&);

    any() = default;
    ~any() {
        if (val_) {
            delete val_;
        }
    }

    template <typename T>
    any& operator=(const T& val) {
        // only if copy constuctible
        if (val_) {
            delete val_;
        }
        val_ = reinterpret_cast<uint8_t*>(new T(val));
        //info_ = typeid(T);

        return *this;
    }

    //const std::type_info& get_type() const { return info_; }

private:
    template <typename T>
    const T& get_unbound() const {
        // no check here
        return *reinterpret_cast<T*>(val_);
    }

    uint8_t* val_ = nullptr;
    //mutable const std::type_info& info_{typeid(void)};
};

template <typename T>
const T& any_cast(const any& a) {
    // const auto& info = typeid(T);
    // if (a.get_type() != info) {
    //     throw std::bad_cast();
    // }

    return a.get_unbound<T>();
}

}
