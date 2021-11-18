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

python3 = import('python').find_installation('python3')
if python3.found()
  message('Building `pyptzf` python3 module.')
  pymod = python3.extension_module('pyptzf',
    sources: ['pymod.cpp'],
    dependencies: [
      dependency('python3'),
      dependency('pybind11'),
    ],
    link_with: libptzf,
    include_directories: ptzf_incdir,
    install: true,
    install_dir: '/usr/local/lib/python3.6/dist-packages',
  )
else
  message('Python3 not found. Not building python module.')
endif
