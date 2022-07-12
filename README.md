## cpp_script
### Description
cpp_script is a header-only C++ 20 library that tries to bring Python expressions such as list,dict,range,etc. to C++ and aims to make them have the same "feel" as when writing real Python code. In order for class instances to work with container classes of cpp_script such as `List` or `Dict`, they must inherit from `Object`. Behind the scenes, cpp_script uses `dynamic_cast` a lot, causing a `List` which stores only integer values to be much slower than a `std::vector<int>`. Expect code using cpp_script to be slightly faster than equivalent Python code.

### Creating a new class
In order to create a new class that is compatible with `List`, `Dict` or other cpp_script containers, it must inherit from `Object` and follow some other rules, which are listed below:
- Inherit as `public virtual Object`
- Implement the copy constructor.
- Implement the destructor and call `OBJECT_DESCTRUCT`, except if the class is abstract.

Optional:
- Override `is_equal`.
- Override `to_string`.

The implementation of `Int` in `include/primitives.hpp` is a good example.

### Running tests
Test can be run with:
```
cd build && make && ctest -T memcheck
```
