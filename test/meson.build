ext = '.cpp'
unit_tests = []
integration_tests = [
  'controller'
]


# googletest subproject stuff
gtest_proj = subproject('gtest')

test_deps = [
  gtest_proj.get_variable('gtest_dep'),
  dependency('boost', modules: ['system']),
]

# run unit tests with default timeout
foreach basename : unit_tests
  test_file = files('test_' + basename + ext)
  exe = executable('test_' + basename, test_file,
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

# run integration tests with large timout
foreach basename : integration_tests
  test_file = files('integration_' + basename + ext)
  exe = executable('integration_' + basename, test_file,
    dependencies: [ptzf_deps, test_deps],
    include_directories: ptzf_incdir,
    link_with: libptzf,
  )
  test('integration ' + basename + ext, exe, 
    suite: 'integration',
    is_parallel: false, # the tests will break on parallel run
    protocol: 'gtest',
    timeout: 240,  # seconds
    env: [],
  )
endforeach