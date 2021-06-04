# libptzf

Pan Tilt Zoom Focus library for custom Marlin 3d printer firmware.

# Requirements:

to build the library
```
sudo apt install \
  libboost-log-dev \
  libboost-system-dev \
  libserial-dev \
  ninja-build
pip3 install --upgrade meson
```

to build coverage reports
```
sudo apt install \
  libxml2-dev \
  libxslt1-dev
pip3 install gcovr
```

# Installation:

```
mkdir builddir (vscode should make this for you if you have the meson extension)
cd buildir
meson ..
ninja test
(optional for coverage reports) ninja coverage
(sudo) ninja install
```

**NOTE**: `ninja install` may also require Meson to be available to the root user if
the installation path is not writable to the running user (usually the case).
In this case, do the ugly `sudo pip3 install --upgrade meson`. You will probably
wish to `sudo apt uninstall meson` if meson is installed with apt since it may
conflict.

# Testing:

## Valgrind

Other than the above tests and coverage reports, Valgrind (`sudo apt install valgrind`) can also be used to
wrap the test to test for memory issues like this:

```
meson test --wrap='valgrind --leak-check=full --show-leak-kinds=all --error-exitcode=1'
```
[source: pmod on stackoverflow, MIT license](https://stackoverflow.com/questions/57734973/how-to-use-valgrind-with-meson)

## Sanitizers

[Sanitizers can be configured](https://mesonbuild.com/howtox.html)
when running meson the first time like:
```
meson -Db_sanitize=address (for ASan)
```
[Values can be](https://mesonbuild.com/Builtin-options.html#base-options):
`none, address, thread, undefined, memory, address, undefined`