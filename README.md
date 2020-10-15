# libptzf

Pan Tilt Zoom Focus library for custom Marlin 3d printer firmware.

# Requirements:

```
sudo apt install \
  libboost-log-dev \
  libboost-system-dev \
  libserial-dev \
  ninja-build
pip3 install --upgrade meson
```

# Installation:

```
mkdir builddir (vscode should make this for you if you have the meson extension)
cd buildir
meson ..
ninja test
ninja install
```
