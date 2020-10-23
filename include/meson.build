ptzf_config_h = configure_file(
  input: 'ptzf_config.h.in',
  output: 'ptzf_config.h',
  configuration: {
    'version': meson.project_version(),
  },
)

ptzf_incdir = include_directories('.')

install_headers(
  'controller.h',
  'position.h',
  ptzf_config_h,
  subdir: meson.project_name(),
)