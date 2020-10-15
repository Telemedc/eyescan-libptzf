project('ptzf', ['cpp'],
  version: run_command(
    'head', '-n', '1', files('VERSION'),
  ).stdout().strip(),
  meson_version: '>= 0.49.0',
  license: 'LGPL',
  default_options: [
    'cpp_std=c++14',
    'warning_level=2',  # -Wall
  ],
)

# the c++ compiler object
cc = meson.get_compiler('cpp')

# split the project version
proj_ver = meson.project_version().split('.')
# version minus patch revision
proj_ver_short = proj_ver[0] + '.' + proj_ver[1]
proj_ver_major = proj_ver[0]
proj_ver_minor = proj_ver[1]
proj_ver_patch = proj_ver[2]
# all of this is used by configure_file on plugin.vala
project_description = 'Pan Tilt Zoom Focus library'
binary_package = 'source'
origin = 'github'
# change this if you fork this
project_url = 'https://github.com/mdegans/libptzf'


subdir('include')
subdir('src')
subdir('test')
# subdir('docs')
