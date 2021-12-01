ext = '.cpp'
unit_tests = [
  'position',
  'controller',
]
integration_tests = [
  'controller',
  'stress_controller',
]

python_unit_tests = [
  'unit_position.py',
  'unit_controller.py',
]
python_integration_tests = [
  'integration_controller.py',
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

python3 = import('python').find_installation('python3')
if python3.found()
  foreach t : python_unit_tests
    test(t, python3,
      args: [files(t)],
      env: {'PYMOD_PATH': pymod_path},
      suite: ['python', 'unit'],
    )
  endforeach
  foreach t : python_integration_tests
    test(t, python3,
      args: [files(t)],
      env: {'PYMOD_PATH': pymod_path},
      suite: ['python', 'integration'],
    )
  endforeach
endif
