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
#include "string.hpp"

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

class Dict : public virtual Object {
private:

class PlaceHolder : public virtual Object {
public:

/* Constructors */
PlaceHolder(const string& key_string) : key_string_(key_string) {
#ifdef DEBUG_DICT
    std::cout << "PlaceHolder() : this = " << (void*) this << std::endl;
#endif
}

explicit PlaceHolder(const PlaceHolder& p) :key_string_(p.key_string_) {
#ifdef DEBUG_DICT
    std::cout << "PlaceHolder(const PlaceHolder& p) : this = " << (void*) this << std::endl;
#endif
}

/* Destructor */
~PlaceHolder() override {
#ifdef DEBUG_DICT
    std::cout << "~PlaceHolder() : this = " << (void*) this << std::endl;
#endif
    OBJECT_DESCTRUCT(this);
}

/* Miscellanous */
string to_string() const override {
    throw KeyError("KeyError: '"+key_string_+"'");
}

private:

const string key_string_;

};

/* Private constructor */
Dict(const size_t max_size) : 
    max_size_(max_size),
    num_elems_(0),
    data_(new unordered_map<tuple<size_t,Element,Element>>())
{}

public:

class KeyError : public exception {
public:

explicit KeyError(const string& s) : s_(s) {}

const char* what() const noexcept override {
    return s_.c_str();
}

private:

const string s_;

};

/* Public constructors */
Dict() : Dict(MIN_DICT_SIZE) {}

Dict(const Dict& d) : Dict(MIN_DICT_SIZE) {
#ifdef DEBUG_DICT
    std::cout << "Dict(const Dict& d) : this = " << (void*) this << std::endl;
#endif
    for (const auto& e : *d.data_) {
        const auto& v = std::get<1>(e);
        (*this)[std::get<1>(v)] = std::get<2>(v);
    }
}

/* Destructor */
~Dict() override {
#ifdef DEBUG_DICT
    std::cout << "~Dict() : this = " << (void*) this << std::endl;
#endif
    OBJECT_DESCTRUCT(this);
}

/* Accessor methods */
template <typename T>
requires std::is_arithmetic_v<std::remove_cvref_t<T>>
Element& operator[](T&& v) {
    if constexpr (std::is_integral_v<std::remove_cvref_t<T>>) {
        return (*this)[Int(std::forward<T>(v))];
    } else {
        return (*this)[Float(std::forward<T>(v))];
    }
}

Element& operator[](const char* s) {
    return (*this)[String(s)];
}

Element& operator[](string&& s) {
    return (*this)[String(std::move(s))];
}

Element& operator[](const string& s) {
    return (*this)[String(s)];
}

const Element& operator[](const char* s) const {
    return (*this)[String(s)];
}

private:

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

private:

template <typename T>
requires (std::is_base_of_v<Object,std::remove_cvref_t<T>> || std::is_same_v<std::remove_cvref_t<T>,Element>)
Element& index_get(T&& k, size_t& idx) {
    // Get the hash of the key
    const size_t h = k.hash();

    // Create a search index generator
    SearchIndexGenerator g(h,max_size_-1u);
    idx = g();

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
                data_->emplace(idx,std::forward_as_tuple(h,k,PlaceHolder(k.to_string())));
            } else {
                data_->emplace(idx,std::forward_as_tuple(h,Element(std::forward<T>(k)),
                               PlaceHolder(k.to_string())));
            }

            // Increase the element count
            ++num_elems_;

            // Check the load factor
            if (static_cast<double>(num_elems_) / static_cast<double>(max_size_) >= 0.666) {
                // Increase the maximum size
                max_size_ *= 2;

                // Create a new larger dictionary
                Dict d(max_size_);
                const size_t insert_idx = idx;
                for (const auto& e : *data_) {
                    const auto& v = std::get<1>(e);
                    if (std::get<0>(e) == insert_idx) {
                        d.index_get(std::get<1>(v),idx) = std::get<2>(v);
                    } else {
                        d[std::get<1>(v)] = std::get<2>(v);
                    }
                }

                // Move the data to the current dictionary
                data_ = std::move(d.data_);
            }

            break;
        }
    }

    // Return a reference to the entry
    return std::get<2>(data_->at(idx));
}

public:

template <typename T>
requires (std::is_base_of_v<Object,std::remove_cvref_t<T>> || std::is_same_v<std::remove_cvref_t<T>,Element>)
Element& operator[](T&& k) {
    size_t idx;
    return index_get(std::forward<T>(k),idx);
}

template <typename T>
requires (std::is_base_of_v<Object,std::remove_cvref_t<T>> || std::is_same_v<std::remove_cvref_t<T>,Element>)
const Element& operator[](T&& k) const {
    // Get the hash of the key
    const size_t h = k.hash();

    // Create a search index generator
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
            throw KeyError("KeyError: '"+k.to_string()+"'");
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
