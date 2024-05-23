# ReleaseButler

[English](./README.md) [简体中文](./README_ZH_CN.md)

**The current project is still in the development stage, so the software will output a lot of unnecessary information.**

~~ReleaseButler is designed to simplify the software installation process on Github on Linux systems🤗. It can automatically detect the underlying Linux distribution, obtain and install the corresponding software package from the designated GitHub project homepage, and also supports update operations. ~~

At present, I want to implement something similar to the BSD-based "ports-like system", like Arch Linux's [ABS](https://wiki.archlinux.org/title/Arch_build_system), or like Gentoo Linux's [ protage](https://wiki.gentoo.org/wiki/Portage). What I want now is not even as complicated as ABS, and I haven't even thought about the USE variable like Gentoo Linux.

TODO:

- [ ] Use JSON format to store package information, (still considering what format to use to store information)
- [ ] Use the `build` field to store some work performed after installation.
- [ ] Use the `ready` field to store some preparation work before installation.
- [ ] I want to realize that if the name of the software package is not provided, it will clone the repository directly by default and prepare to start compiling it, but I have not yet done any detection in this regard.
- [ ] Want to create a repository on GitHub to store the json file for software package installation, so that users can directly import and install it. In other words, it's like nix (but I'm just too lazy to learn nix syntax, so I insisted on writing one myself)
- [ ] Currently, it's using sudo to install the software. It has not checked whether the user rights of the current software are non-root users, and it has  not checked sudo (I'm planning to implement it to detect whether there are doas if there is no sudo).
- [ ] I want to realize that it supports many software (source code) distribution platforms, but it is only processed with GitHub.

---

⚠️: You must first install **sudo** instead of **open-doas** or other similar software. If you don’t want to rely heavily on **sudo**, you can choose to modify the source code😀. Or if I think of it later, I will try to add detection for sudo or doas.

---

## Features:

**Automatic release detection:** ReleaseButler determines the Linux distribution of the running environment to ensure compatibility with various systems.

**INSTALL & UPDATE**: Easily install packages using a single command. ReleaseButler supports updates to keep your installed software up to date. (TODO)

**Try to adapt to various situations**: Try to introduce various fields to adapt to the installation steps of various repositories. (TODO)

## Instructions

### install software:

```bash
$ releasebutler --install <homepage> --packname <name>
```

### Update (Todo)

```bash
$ releasebutler --update
```

---

note: ReleaseButler does not currently support individual updates of specified packages. 🙃

---

## Supported Linux distributions

- Debian/Ubuntu
-Fedora

---

## How to compile

If you compile this project under Debian OS:

```bash
$ sudo apt install libcurl4-openssl-dev clang
$ git clone https://github.com/suoyuan666/ReleaseButler.git
$ cd ReleaseButler
$ cmake -B build -DCMAKE_BUILD_TYPE=Release
$ cmake --build build -j `nproc`
```

If it is compiled under other platforms, forgive me for being lazy and look for the package name of the libcurl4-openssl-dev package corresponding to other versions. 😛

I wrote in **CMakeLists.txt** that I use `clang` to compile, because I use `clang` for some of the compilation options. If you want to use `gcc` to compile, you may also need to modify the compilation options. This is it:

```CMakeLists
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Configuring Release build")
    # something come form https://airbus-seclab.github.io/c-compiler-security/clang_compilation.html
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -pipe -fPIE -Wall -Wextra -Wpedantic -Werror -Wthread-safety")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fstack-clash-protection -fstack-protector-all -fcf-protection=full")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -flto -fvisibility=hidden -fsanitize=cfi")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fsanitize=integer -fsanitize-minimal-runtime -fno-sanitize-recover")
endif()
```