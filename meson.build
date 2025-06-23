project('asciivid', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++14'])

opencv_dep = dependency('opencv4', required: true)

executable('asciivid',
  sources: ['main.cpp'], # Укажите ваши исходные файлы
  dependencies: [opencv_dep])
