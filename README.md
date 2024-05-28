# ReleaseButler

[English](./README.md) [简体中文](./README_ZH_CN.md)

~~The current project is still in the development stage, so the software will output a lot of unnecessary information.~~(The option verbose is now added.)

~~ReleaseButler is designed to simplify the software installation process on Github on Linux systems🤗. It can automatically detect the underlying Linux distribution, obtain and install the corresponding software package from the designated GitHub project homepage, and also supports update operations.~~

At present, I want to implement something similar to the BSD-based "ports-like system", like Arch Linux's [ABS](https://wiki.archlinux.org/title/Arch_build_system), or like Gentoo Linux's [ protage](https://wiki.gentoo.org/wiki/Portage). What I want now is not even as complicated as ABS, and I haven't even thought about the USE variable like Gentoo Linux.

TODO:

- [x] ~~Use JSON format to store package information.~~
- [x] ~~Use the `build` field to store some work performed after installation.~~
- [x] ~~Use the `install` field to store some preparation work before installation.~~
- [x] ~~I want to realize that if the name of the software package is not provided, it will clone the repository directly by default and prepare to start compiling it.~~
- [ ] Want to create a repository on GitHub to store the json file for software package installation, so that users can directly import and install it. In other words, it's like nix (but I'm just too lazy to learn nix syntax, so I insisted on writing one myself)
- [ ] Currently, it's using sudo to install the software. It has not checked whether the user rights of the current software are non-root users, and it has  not checked sudo (I'm planning to implement it to detect whether there are doas if there is no sudo).
- [ ] I want to realize that it supports many software (source code) distribution platforms, but it is only processed with GitHub.

---

⚠️: You must first install **sudo** instead of **open-doas** or other similar software. If you don’t want to rely heavily on **sudo**, you can choose to modify the source code😀. Or if I think of it later, I will try to add detection for sudo or doas.

---

## Features:

**Automatic release detection:** ReleaseButler determines the Linux distribution of the running environment to ensure compatibility with various systems.

**INSTALL & UPDATE**: Easily install packages using a single command. ReleaseButler supports updates to keep your installed software up to date.

**Try to adapt to various situations**: Try to introduce various fields to adapt to the installation steps of various repositories.

## Instructions

### install software:

```bash
$ releasebutler --install <name> --pakname <package name> --from <url> [--verbose]
```

The `--install` field accepts the name of the software to be installed, and the `packname` field is the name of the software package, for example `fastfetch-linux-amd64.deb` is `packname`.

Installing from the command line does not yet support all fields (like `install`, `build`, etc.).

### Update (Todo)

```bash
$ releasebutler --update
```

### Parse externally imported json files

```bash
$ releasebutler --parse <file name>
```

note: This file needs to be in the **~/.config/ReleaseButler/** directory

---

note: ReleaseButler does not currently support individual updates of specified packages. 🙃

Software installed using the command line is recorded in **~/.config/ReleaseButler/info.json**, but when updating, all json files in the **~/.config/ReleaseButler/** directory will actually be traversed.

I try to support importing json files from other places in this way. But the file name cannot be called **package.json**. This file is used to record the version of the installed software package. This json file will also be skipped if it is traversed.

---

## Field definitions of software package information files:

```json
{
    "fastfetch": {
       "name": "fastfetch-linux-amd64.deb",
       "url": "https://github.com/fastfetch-cli/fastfetch",
       "version": "2.13.2",
       "build" : [
          "touch something",
          "touch haaa"
       ],
       "install" : [
          "echo something > something"
       ],
       "clone": false,
       "download" : false
    }
}
```

- `name` is the package name of the Release page.
- `build` used to store some operations to be performed before downloading the software.
- `install` used to store some operations to be performed after the software is installed.
- `clone` indicates whether to directly clone the repository.
- `download` indicates whether to just download the package from Release instead of calling the system package manager to install it after downloading.

## Supported Linux distributions

- Debian/Ubuntu/Deepin
- Fedora

---

## How to compile

If you compile this project under Debian OS:

```bash
$ sudo apt install libcurl4-openssl-dev libboost-all-dev clang
$ git clone https://github.com/suoyuan666/ReleaseButler.git
$ cd ReleaseButler
$ git submodule update --init --recursive
$ cmake -B build -DCMAKE_BUILD_TYPE=Release
$ cmake --build build -j `nproc`
```

If it is compiled under other platforms, forgive me for being lazy and look for the package name of the libcurl4-openssl-dev package corresponding to other versions. 😛

I tried to compile this project in OpenSUSE Tumbleweed. I installed the required software using the following code.

```bash
$ sudo zypper install libboost_program_options-devel boost-devel libcurl-devel clang18 llvm18-gold cmake
```