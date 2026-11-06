project(
  'theme-customizer',
  'cpp',
  default_options: ['cpp_std=c++17'],
)

gtkmm_dep = dependency('gtkmm-4.0')

executable(
  'theme-customizer',
  'src/main.cc',
  'src/main_paned.cc',
  'src/settings.cc',
  'src/wallpapers.cc',
  'src/window.cc',
  dependencies: gtkmm_dep,
)