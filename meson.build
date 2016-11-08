project('luksmeta', 'c', version: '3')

add_global_arguments(
  '-Wall',
  '-Wextra',
  '-Werror',
  '-Wstrict-aliasing',
  '-Wchar-subscripts',
  '-Wformat-security',
  '-Wmissing-declarations',
  '-Wmissing-prototypes',
  '-Wnested-externs',
  '-Wpointer-arith',
  '-Wshadow',
  '-Wsign-compare',
  '-Wstrict-prototypes',
  '-Wtype-limits',
  '-Wunused-function',
  '-Wno-missing-field-initializers',
  '-Wno-unused-parameter',
  language: 'c'
)

libcryptsetup = dependency('libcryptsetup', version: '>=1.5.1')

libcheck = static_library(
  'check',
  'check.c'
)

libcrc32c = static_library(
  'crc32c',
  'crc32c.c',
  c_args: '-fPIC'
)

libluksmeta = shared_library(
  'luksmeta',
  'libluksmeta.c',
  soversion: '0',
  version: '0.0.0',
  link_with: libcrc32c,
  link_args: '-Wl,--version-script,@0@/libluksmeta.map'.format(meson.current_source_dir()),
  dependencies: libcryptsetup,
  install: true
)

install_headers('luksmeta.h')

pkgcfg = import('pkgconfig')
pkgcfg.generate(
  libraries: libluksmeta,
  requires: 'libcryptsetup',
  version: meson.project_version(),
  name: 'LUKSMeta',
  filebase: 'luksmeta',
  description: 'Library for storing metadata in the LUKS header'
)

executable(
  'luksmeta',
  'luksmeta.c',
  link_with: libluksmeta,
  dependencies: libcryptsetup,
  install: true
)

test(
  'test-crc32c',
  executable(
    'test-crc32c',
    'test-crc32c.c',
    link_with: libcrc32c
  )
)

test(
  'test-lm-assumptions',
  executable(
    'test-lm-assumptions',
    'test-lm-assumptions.c',
    link_with: [libcheck, libluksmeta],
    dependencies: libcryptsetup
  )
)

test(
  'test-lm-init',
  executable(
    'test-lm-init',
    'test-lm-init.c',
    link_with: [libcheck, libluksmeta],
    dependencies: libcryptsetup
  )
)

test(
  'test-lm-one',
  executable(
    'test-lm-one',
    'test-lm-one.c',
    link_with: [libcheck, libluksmeta],
    dependencies: libcryptsetup
  )
)

test(
  'test-lm-two',
  executable(
    'test-lm-two',
    'test-lm-two.c',
    link_with: [libcheck, libluksmeta],
    dependencies: libcryptsetup
  )
)

test(
  'test-lm-big',
  executable(
    'test-lm-big',
    'test-lm-big.c',
    link_with: [libcheck, libluksmeta],
    dependencies: libcryptsetup
  )
)

test(
  'test-luksmeta',
  find_program('test-luksmeta')
)
