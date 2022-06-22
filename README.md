Inheriting from Object
1.) Inherit as public virtual Object
2.) Implement the copy constructor.
3.) Implement the destructor and call OBJECT_DESCTRUCT, except if the class is abstract.
4.) Override is_equal if necessary.
5.) Override to_string.

Run test with
cd build && make && ctest -T memcheck
