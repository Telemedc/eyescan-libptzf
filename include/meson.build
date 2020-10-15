config_h = configure_file(
  input: 'config.h.in',
  output: 'config.h',
  configuration: {
    'version': meson.project_version(),
  },
)

ptzf_incdir = include_directories('.')

install_headers(
  'controller.h',
  'position.h',
  'utils.h',
  config_h,
  subdir: meson.project_name(),
)