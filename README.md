# libptzf

Pan Tilt Zoom Focus library for custom Marlin 3d printer firmware.

# Requirements:

```
sudo apt install \
  libboost-log-dev \
  libboost-system-dev \
  libserial-dev \
  meson \
  ninja-build
```

# Installation:

```
mkdir builddir (vscode should make this for you if you have the meson extension)
cd buildir
meson ..
ninja test
ninja install
```
