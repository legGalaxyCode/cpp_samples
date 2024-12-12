#!/bin/sh

projectDir=~/projects/cpp/cpp_samples/
buildDir=$(realpath ~/projects/cpp/cpp_samples/build/)

useAsan=false
useUbsan=false
useThreadsan=false
useClangTidy=false

cleanBuild=false
# TODO: add support of clang-format

i=0;
for arg in "$@"; do
	case $arg in
		--asan)
			echo "Build with address sanitizer!"
			useAsan=true
			;;
		--ubsan)
			echo "Build with ub sanitizer!"
			useUbsan=true
			;;
		--thread)
			echo "Build with thread sanitizer!"
			useThreadsan=true
			;;
		--clang-tidy)
			echo "Build with clang-tidy!"
			useClangTidy=true
			;;
		--clean-build)
			echo "Clean build!"
			cleanBuild=true
			;;
		*)
			if [ "$i" = 0 ]; then
				continue
			fi

			echo "Unsupported option!"
			exit 1
			;;
	esac
	i=$((i + 1));
done

if [ "${cleanBuild}" = true ]; then
	rm -rf "$buildDir"
fi

echo "Use asan: $useAsan";
echo "Use ubsan: $useUbsan";
echo "Use thread san: $useThreadsan";
echo "Use clang-tidy: $useClangTidy";

if [ "${useAsan}" = true ] && [ "${useThreadsan}" = true ]; then
	echo "Error! Asan can't be built with thread sanitizer!"
	exit 1
fi

CMAKE_FLAGS=""
if [ "$useAsan" = true ]; then
	CMAKE_FLAGS="${CMAKE_FLAGS} -DASAN=ON"
fi

if [ "$useUbsan" = true ]; then
	CMAKE_FLAGS="${CMAKE_FLAGS} -DUBSAN=ON"
fi

if [ "$useThreadsan" = true ]; then
	CMAKE_FLAGS="${CMAKE_FLAGS} -DTHREADSAN=ON"
fi

if [ "$useClangTidy" = true ]; then
	CMAKE_FLAGS="${CMAKE_FLAGS} -DUSE_CLANG_TIDY=ON"
fi

echo ${CMAKE_FLAGS}
echo ${buildDir}

cmake ${CMAKE_FLAGS} -B ${buildDir} -S ${projectDir}
cmake --build ${buildDir}
