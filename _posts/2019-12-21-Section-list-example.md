---
layout: post
title: Section List Example
date: 2019-12-21 00:00:00
---

# Contents
1. [Introduction]({{page.url}}#introduction)



# Introduction

The aim of this article is to provide a series of good general advice and considerations on how to design and write libraries, particularly if portability, ease of use and performance are of concern.

The word "game" is in parentheses since most of the advice also applies to non-game libraries.

Bear in mind that there is no "true way" to write a library and different people have different opinions, however, there do exist general advice and considerations that can be examined based on existing work.

The advice and considerations presented in this article are not meant for all kinds of libraries, for example, for libraries such as language-specific containers or wrappers around os functionality, some of the advice may not apply. Remember that when designing a library you should understand your requirements and do what you find to be the best approach, this article acts simply as a non-exhaustive list of good considerations and advice.

This article aims to present advice based on existing libraries that are considered of quality in the community so that developers can better understand some of the considerations involved in designing a library. The list does not aim to be exhaustive, but hopefully, it can be updated to include more advice and considerations over time and serves as a starting point and educational resource to anyone interested in library design.

The core principles of this advice are:

Maximize portability.

Be easy to build.

Be easy to integrate.

Be usable in as many scenarios as possible.

# Consider writing the library in C and add wrappers to other languages later.
## C is the lingua franca of programming. There are many advantages to writing your library in C from the start:

Every language out there has a way to call into C, so if you write your library in C everyone will be able to use it, in any language, and people can write wrappers for it easily. If you write a library in Python, chances are most people won’t be able to use it if they don’t also use Python. If you write a library in C however, someone who really likes it can make bindings for it in Python. C truly brings all of us together.

C code is usually fast. There is a saying that if your code is slower than C, someone will rewrite it in C. Performance is especially important for game developers and people who use low-level languages in general.

C is the most portable language in the world, if your library is written in C it means it can be used on any OS, console or mobile device and even on the web.

## You might also consider writing the library in C++, be mindful however of the following drawbacks:

It is easier in general for a C++ user to use a C library than it is for a C user to use a C++ library.

C++ is not as easy to write wrappers for in other languages.

If you use C++, unless you limit which C++ features you use (to the point where you are pretty much left with C) a lot of people won’t be able to use your library. Not everyone uses RAII, some people disable exceptions and RTTI, and not everyone is willing to use a library with smart pointers, template metaprogramming, STL allocators, virtual dispatch, etc.

## Take into account the following aspects when writing your C library:

Compiler extensions make your code less portable, try using the subset of C99 which compiles both in C and C++ mode on GCC, Clang, and MSVC. This is important since some compilers (such as MSVC) have problems with supporting C.

Try not to use the hosted (OS-dependent) parts of the C standard library, unless you really have to, since they might work differently on other platforms (or might not work at all). If you need to use OS-dependent functions but want to maximize portability, request function pointers from the client.

Prefix your names to avoid name collisions (more on this later).

For maximum portability consider using the built-in C types (char, short, int, long). This is because not all compilers and platforms have support for stdint.h (eg: old versions of MSVC). Try checking for the availability of stdint.h and use that if available. As an example consider looking at how the library stb_image handles the use of stdint.h.

Consider using header guards instead of #pragma once. Header guards are standard and also allow the user of the library to check if a library was included.

Undef macros that should not be exposed to the user at the end (eg: my_min/max macros), do this even in C files.

Make sure your library can compile as one compilation unit since a lot of developers choose to do single-compilation-unit builds (also known as unity builds).

## If you want to use another language for the implementation, consider keeping the interface in C.

If you want to write a library using another language, such as Odin, Rust, Zig, C++, etc, consider keeping the interface in C so that people can easily use it and wrap it for other languages.

Bear in mind that this has several disadvantages:

People might have a harder time integrating your library into their projects as source.

You might be adding the standard library of your language of choice as an extra dependency that the user of your library now needs to consider. This can also have other implications, for example, some standard libraries use the general heap allocator, which violates the principle of giving the user of the library full control over memory allocations (more on this later). Also interfacing with certain aspects of a standard library from another language via a C API could be awkward.

You might need to provide precompiled binaries or ways for your users to build your library for their target platform. This might not be an issue with C++ but it can be with other languages (eg: Odin, Zig, Rust).

If people don't use the same language for their projects their debugging experience might suffer when using your library.

# Consider providing an optional C++ wrapper.

Besides C users, C++ users are the ones most likely to use your C library. To that extent, you might want to consider making their experience better by providing a C++ wrapper.

Take into consideration that this is an extra piece of code you would have to maintain but that can help the adoption of your library by C++ developers.

## Basic things you can do when writing your C++ wrapper:

Use .hpp and .cpp for your C++ wrapper to distinguish between the C and C++ code. Consider putting the C++ wrapper in another folder or repo and mention it in the readme.

Try not to include the header from the C version in the hpp file. Instead, rewrite the declarations in the hpp in a more C++ style and in the cpp file include the C header and provide the definitions for all the wrapper functions. This is to prevent the names from C and macros to spill into C++ code.

Expose constants to the user using constexpr variables. This is especially easy since C structs are constexpr by default. Make sure constants from C that use macros are not present in the C++ wrapper.

Use namespaces and wrap all the functions like so namespace mylib { foo bar() { return mylib_bar(); }.

Try not to use exceptions and RTTI. Especially in game development, not all people use them and some just disable them.

Consider using default parameter values over function wrappers. C lacks default values for parameters so usually, people write a function with a lot of parameters and then create several wrappers for it that calls the original function with different default parameter values. C++ has default parameter values so consider removing the extra wrapper functions if you have any.

Try not to use STL containers or smart pointers since your wrapper should just simply wrap the functions from C and also adding those that bring extra problems that you need to consider. (More on this later)

If you decide to offer RAII wrappers for parts of your library, still provide wrappers for the non-RAII structs and functions. This is important because not all developers use RAII and if you don’t expose the non-RAII versions of structs and functions in your C++ layer they won’t be able to use it, at least not fully.

# Try not to make the use of a build system mandatory.

There are many build systems out there and chances are people won’t use the same one you do. If your library simply presents the source files, the header files and a list of the dependencies (if you have any), this will help developers integrate your library into their projects.

If there are compiler flags needed to compile your library then you can mention them in the readme, though preferably there won’t be any.

It is good to include optional build files, but mention in the readme that they are optional and try to keep the simple structure that lets people integrate the library easily with their preferred build system.

If you do use a build system such as CMake or Make, try to use them in a standard way so that developers who choose to use them can get the most out of them.

# Ensure that people can easily compile the library from source.

Distribute your libraries such that people can simply include the source files in their project and be done with it.
## This has many advantages such as:

Letting people easily reason about your library.

Easy to include in cross-platform projects, no craziness regarding libc versions and platform stuff.

It allows people to step through the code in a debugger.