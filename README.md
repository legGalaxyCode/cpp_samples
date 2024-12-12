# About the project


## Notes
### Clang-tidy
At the beginning I wanted to integrate clang-tidy with a cmake. 
In this case cmake calls clang-tidy before the main compilation.
The disadvantage of this approach is that clang-tidy errors don't terminate compilation in this case.
Since I don't want to compile sources if there're clang-tidy errors I need to write a python script
that is called by build script first.
But the good thing is that I can easily specify what target I want to check with clang-tidy.