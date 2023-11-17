# ReleaseButler
[简体中文](./REDME_ZH_CN.md)

ReleaseButler is a powerful and flexible tool designed to simplify the software where in GitHub installation process on Linux systems🤗. It intelligently detects the underlying Linux distribution, fetches the appropriate software package from the designated GitHub project homepage, and facilitates seamless installations and updates.

---
⚠️: You must have the **sudo** instead of **open-doas** or others, or you can modify the source code yourself so that it no longer depends on sudo😀.
---

## Feature:

 **Automatic Distribution Detection:** ReleaseButler determines the Linux distribution of the host environment, ensuring compatibility with a wide range of systems.

 **Installation and Updates**: Effortlessly install software packages with a single command. AutoInstaller also supports updates, keeping your installed software current. 

## Getting Started

```bash
$ git clone https://github.com/suoyuan666/ReleaseButler.git
$ cd ReleaseButler
$ make install
```

## Usage

### Installation:

```bash
$ relesebutler --install <package> --form <url>
```

### Update

```bash
$ relesebutler --update
```

---

note: ReleaseButler currently does not support the update of the specified software package.🙃

---

## Supported Linux Distributions

- Debian/Ubuntu
- Fedora

---

Some distributions (such as Arch Linux) have their own user software repositories🥲, and I think for the most part they won't need this software. Therefore, ReleaseButler currently only supports Debian and Fedora (I use Debian myself, and Fedora is a support I added conveniently, and I have not tested it myself).

---

## Develop

If you are in Debian:

```bash
$ sudo apt install libcurl4-openssl-dev
$ git clone https://github.com/suoyuan666/ReleaseButler.git
$ cd ReleaseButler
$ make relesebutler
```

If it is compiled under other platforms, forgive me for being lazy, and go find the libcurl4-openssl-dev package and sign up for the software corresponding to other versions.😛