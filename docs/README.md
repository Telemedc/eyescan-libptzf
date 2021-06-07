# Meson Doxygen Template

This folder can be reused to provide drop-in Doxygen support to Meson projects.

## Use

Copy this folder ino the same folder that contains your `include` folder, add `subdir('docs')` to that folder's `meson.build` , and do `ninja docs` from your build directory to build the docs. That's it.

## It didn't work

If your includes are not in `include` under the source root, you'll have to modify the `input` key value in `configure_file` in `meson.build` appropriately to point to your headers.

The `meson.build` also assumes the `project_description` and `project_url` variables are set an upper level `meson.build` , but these can be replaced with string literals in `meson.build` -> `configure_file` if you prefer to set them manually.

## Customizing

If you want to modify the Doxyfile template, edit Doxyfile.in, however it's probably more useful to add additional @template_variables@ and `configure_file` them.
