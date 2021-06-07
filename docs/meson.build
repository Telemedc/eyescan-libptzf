# https://stackoverflow.com/questions/52520146/cannot-run-doxygen-from-meson-on-a-c-project
doxygen = find_program('doxygen', required: false)
if doxygen.found()
  doxyfile = configure_file(
    input: 'Doxyfile.in',
    output: 'Doxyfile',
    configuration: {
      'input': meson.current_source_dir() / '..' / 'include',
      'project_name': meson.project_name(),
      'project_number': meson.project_version(),
      'project_brief': project_description,
      'output_directory': meson.current_build_dir(),
      'url': project_url,
    },
  )
  run_target('docs', command: [doxygen, doxyfile])
else
  warning('doxygen not found... not building documentation')
endif
