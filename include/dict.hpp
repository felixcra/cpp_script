/* 
 * Author: Felix Crazzolara
 */ 
#pragma once

#include <functional>
#include <tuple>
#include <unordered_map>
#include <exception>
#include <string>
#include <type_traits>
#include <memory>

#ifdef DEBUG_DICT
#include <iostream>
#endif

#include "object.hpp"
#include "element.hpp"

namespace cs {

/* Configuration */
// See:
// https://stackoverflow.com/questions/56097997/how-does-python-implement-dictionaries
inline constexpr size_t MIN_DICT_SIZE = 8;
inline constexpr size_t PERTURB_SHIFT = 5;

/* Type definitions */
template <typename T>
using unordered_map = std::unordered_map<size_t,T>;
template <typename... Types>
using tuple         = std::tuple<Types...>;
using exception     = std::exception;
using string        = std::string;
template <typename T>
using unique_ptr    = std::unique_ptr<T>;

class Dict : public Object {

class PlaceHolder : public Object {
public:

/* Constructors */
PlaceHolder() {
#ifdef DEBUG_DICT
    std::cout << "PlaceHolder() : this = " << (void*) this << std::endl;
#endif
}

explicit PlaceHolder(const PlaceHolder& p) {
#ifdef DEBUG_DICT
    std::cout << "PlaceHolder(const PlaceHolder& p) : this = " << (void*) this << std::endl;
#endif
}

/* Destructor */
~PlaceHolder() override {
#ifdef DEBUG_DICT
    std::cout << "~PlaceHolder(const PlaceHolder& p) : this = " << (void*) this << std::endl;
#endif
    OBJECT_DESCTRUCT(this);
}

};

class KeyError : public exception {
public:

explicit KeyError(const string& s) : s_(s) {}

const char* what() const noexcept override {
    return s_.c_str();
}

private:

const string s_;

};

public:

/* Constructors */
Dict(const size_t max_size=MIN_DICT_SIZE) : 
    max_size_(max_size),
    num_elems_(0),
    data_(new unordered_map<tuple<size_t,Element,Element>>())
{}

Dict(const Dict& d) {
    // TODO: Implement this
    throw exception();
}

/* Destructor */
~Dict() override {
    OBJECT_DESCTRUCT(this);
}

/* Accessor methods */
template <typename T>
requires std::is_arithmetic_v<std::remove_cvref_t<T>>
Element& operator[](T&& v) {
    return (*this)[Primitive(v)];
}

template <typename T>
requires (std::is_base_of_v<Object,std::remove_cvref_t<T>> || std::is_same_v<std::remove_cvref_t<T>,Element>)
Element& operator[](T&& k) {
    // Get the hash of the key
    const size_t h = k.hash();

    // Create a search index generator
    class SearchIndexGenerator {
        public:
            SearchIndexGenerator(const size_t hash, const size_t mask) :
                j(hash & mask),
                perturb(hash),
                mask(mask) {}

            size_t operator()() {
                const size_t ret = j;

                j = (j << 2) + j + 1 + perturb; // (j << 2) + j == 5*j

                perturb >>= PERTURB_SHIFT;

                return ret & mask;
            }

        private:
            size_t j;
            size_t perturb;
            size_t mask;
    };
    SearchIndexGenerator g(h,max_size_-1u);
    size_t idx = g();

    // Try to get the element
    while (true) {
        if (data_->contains(idx)) {
            // Check if the hashes match
            if (std::get<1>(data_->at(idx)) == k) {
                // Check if the keys match
                if (std::get<1>(data_->at(idx)) == k) {
                    // Found the correct element
                    break;
                } else {
                    // Probe a new index
                    idx = g();
                }
            } else {
                // Probe a new index
                idx = g();
            }
        } else {
            // Add a new element
            if constexpr (std::is_same_v<std::remove_cvref_t<T>,Element>) {
                data_->emplace(idx,std::forward_as_tuple(h,k,PlaceHolder()));
            } else {
                data_->emplace(idx,std::forward_as_tuple(h,Element(std::forward<T>(k)),PlaceHolder()));
            }

            // Increase the element count
            ++num_elems_;

            // Check the load factor
            if (static_cast<double>(num_elems_) / static_cast<double>(max_size_) >= 0.666) {
                // Increase the maximum size
                max_size_ *= 2;

                Dict d(max_size_);
                for (const auto& e : *data_) {
                    const auto& v = std::get<1>(e);
                    d[std::get<1>(v)] = std::get<2>(v);
                }

                data_ = std::move(d.data_);
            }

            break;
        }
    }

    // Return a reference to the entry
    return std::get<2>(data_->at(idx));
}

private:

size_t max_size_;
std::hash<Element> hash_;
size_t num_elems_;

static_assert(std::is_copy_assignable_v<tuple<size_t,Element,Element>>);
static_assert(std::is_copy_constructible_v<tuple<size_t,Element,Element>>);
unique_ptr<unordered_map<tuple<size_t,Element,Element>>> data_;

};

};
