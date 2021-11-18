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
meson -Db_coverage=true ..
ninja test
(optional for coverage reports) ninja coverage
(sudo) ninja install
```

**NOTE**: `ninja install` may also require Meson to be available to the root user if
the installation path is not writable to the running user (usually the case).
In this case, do the ugly `sudo pip3 install --upgrade meson`. You will probably
wish to `sudo apt uninstall meson` if meson is installed with apt since it may
conflict.

## Python bindings

A `pyptzf` module will be built and installed in `/usr/local/lib/python3.6/dist-packages`
if `python3-dev` (python headers) are installed on the system. The interface is
mostly the same as C++. Simple usage is:

```python
import pyptzf

c = pyptzf.Controller('/dev/MCB')

p = pyptzf.Position(0,1,2,3)

if not c.go(p):
  print(f"controller could not go to position: {}")
```

Would go to xyzf position 0,1,2,3 unless it wasn't possible in which case 
`controller could not go to position: G0 X0.000 Y1.000 Z2.000 E3.000` would
be printed.

TODO: `Controller` as a context manager.

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