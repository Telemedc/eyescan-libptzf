ext = '.cpp'
basenames = [
  'controller'
]

# googletest subproject stuff
gtest_proj = subproject('gtest')

test_deps = [
  gtest_proj.get_variable('gtest_dep'),
  dependency('boost', modules: ['system']),
]

foreach basename : basenames
  test_file = files('test_' + basename + ext)
  exe = executable('test_' + basename, test_file,
    dependencies: [ptzf_deps, test_deps],
    include_directories: ptzf_incdir,
    link_with: libptzf,
  )
  test('test ' + basename + ext, exe)
endforeach