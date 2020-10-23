ptzf_source = [
  'position.cpp',
  'controller.cpp',
  'utils.cpp',
]

ptzf_deps = [
  dependency('threads'),
  dependency('libserial'),
  dependency('boost', modules: ['log']),
]

libptzf = library('ptzf-' + proj_ver_short, ptzf_source,
  dependencies: ptzf_deps,
  include_directories: ptzf_incdir,
  install: true,
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
)