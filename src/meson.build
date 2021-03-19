ptzf_source = [
  'position.cpp',
  'controller.cpp',
  'utils.cpp',
]

ptzf_deps = [
  dependency('threads'),
  dependency('libserial'),
]

libptzf = library(meson.project_name(), ptzf_source,
  dependencies: [
    ptzf_deps,
    dependency('boost', modules: ['log']),
  ],
  include_directories: ptzf_incdir,
  install: true,
  version: meson.project_version(),
)

ptzf_dep = declare_dependency(
  link_with: libptzf,
  include_directories: ptzf_incdir,
  dependencies: ptzf_deps,
)

pkg = import('pkgconfig')
ptzf_pc = pkg.generate(libptzf,
  description: project_description,
  libraries: ptzf_deps,
  url: project_url,
  install_dir: get_option('datadir') / 'pkgconfig',
  subdirs: meson.project_name(),
)