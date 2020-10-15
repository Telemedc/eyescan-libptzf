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
