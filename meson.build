project(
  'sway-theme-customizer',
  'cpp',
  default_options: ['cpp_std=c++17'],
)

gtkmm_dep = dependency('gtkmm-4.0')

executable(
  'sway-theme-customizer',
  'main.cc',
  'main_paned.cc',
  'settings.cc',
  'wallpapers.cc',
  'window.cc',
  dependencies: gtkmm_dep,
)

configure_file(input: 'gear-solid.png', output: 'gear-solid.png', copy: true)
configure_file(input: 'style.css', output: 'style.css', copy: true)