ptzf_source = [
  'position.cpp',
  'controller.cpp',
  'utils.cpp',
]

ptzf_deps = [
  dependency('libserial'),
]

cpp_args = []
if get_option('PRINTER') != ''
  printer_device = get_option('PRINTER')
  message('PRINTER=' + printer_device)
  cpp_args += '-DPRINTER="' + printer_device + '"'
else
  warning('PRINTER not defined with `-DPRINTER=...` PRINTER will be simulated.')
endif

libptzf = library(meson.project_name(), ptzf_source,
  dependencies: [
    ptzf_deps,
    dependency('boost', modules: ['log', 'system', 'thread']),
  ],
  cpp_args: cpp_args,
  override_options : ['cxx_std=c++14'],  # because subprjects can override this
  version: meson.project_version(),
  include_directories: ptzf_incdir,
  install: true,
)

ptzf_dep = declare_dependency(
  link_with: libptzf,
  include_directories: ptzf_incdir,
  version: meson.project_version(),
)

pkg = import('pkgconfig')
ptzf_pc = pkg.generate(libptzf,
  description: project_description,
  requires_private: ptzf_deps,
  url: project_url,
  install_dir: get_option('datadir') / 'pkgconfig',
  subdirs: meson.project_name(),
)