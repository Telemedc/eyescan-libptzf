ext = '.cpp'
unit_tests = []
integration_tests = [
  'controller'
]

test_cflags = []
build_integration_tests = false
if get_option('PRINTER') != ''
  printer_device = get_option('PRINTER')
  message('PRINTER=' + printer_device)
  test_cflags += '-DPRINTER="' + printer_device + '"'
  message('PRINTER defined. Building integration tests.')
  build_integration_tests = true
endif

# googletest subproject stuff
gtest_proj = subproject('gtest')

test_deps = [
  gtest_proj.get_variable('gtest_dep'),
  dependency('boost', modules: ['system', 'log']),
]

# run unit tests with default timeout
foreach basename : unit_tests
  test_file = files('test_' + basename + ext)
  exe = executable('test_' + basename, test_file,
    cpp_args: test_cflags,
    dependencies: [ptzf_deps, test_deps],
    include_directories: ptzf_incdir,
    link_with: libptzf,
  )
  test('test ' + basename + ext, exe,
    suite: 'unit',
    protocol: 'gtest',
    env: [],
  )
endforeach

if build_integration_tests
  # build integration tests but do not run them
  foreach basename : integration_tests
    test_file = files('integration_' + basename + ext)
    exe = executable('integration_' + basename, test_file,
      cpp_args: test_cflags,
      dependencies: [ptzf_deps, test_deps],
      include_directories: ptzf_incdir,
      link_with: libptzf,
    )
  endforeach
endif