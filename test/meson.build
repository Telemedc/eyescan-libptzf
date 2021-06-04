ext = '.cpp'
unit_tests = [
  'position',
]
integration_tests = [
  'controller',
  'stress_controller',
]

# googletest subproject stuff
gtest_proj = subproject('gtest')

test_deps = [
  gtest_proj.get_variable('gtest_dep'),
]

# run unit tests with default timeout
foreach basename : unit_tests
  test_file = files('unit_' + basename + ext)
  exe = executable('unit_' + basename, test_file,
    cpp_args: cpp_args,
    dependencies: [ptzf_dep, test_deps],
    include_directories: ptzf_incdir,
  )
  test('test ' + basename + ext, exe,
    suite: 'unit',
    protocol: 'gtest',
    env: ['PTZF_DEBUG=true'],
  )
endforeach

# build integration tests but do not run them
foreach basename : integration_tests
  test_file = files('integration_' + basename + ext)
  exe = executable('integration_' + basename, test_file,
    cpp_args: cpp_args,
    dependencies: [ptzf_dep, test_deps],
    include_directories: ptzf_incdir,
  )
  test('test ' + basename + ext, exe,
    suite: 'integration',
    protocol: 'gtest',
    is_parallel: false,
    timeout: 300,
    env: ['PTZF_DEBUG=true'],
  )
endforeach
