ptzf_config_h = configure_file(
  input: 'ptzf_config.h.in',
  output: 'ptzf_config.h',
  configuration: {
    'version': meson.project_version(),
    'MAX_POS_X': get_option('MAX_POS_X'),
    'MAX_POS_Y': get_option('MAX_POS_Y'),
    'MAX_POS_Z': get_option('MAX_POS_Z'),
    'MAX_POS_F': get_option('MAX_POS_F'),
  },
)

ptzf_incdir = include_directories('.')

install_headers(
  'controller.h',
  'position.h',
  subdir: meson.project_name(),
)