def FlagsForFile( filename, **kwargs ):
  return {
    'flags': [ '-x', 'c++', '-Wall', '-Wextra', '-Werror', '-lpthread', '-lgdal', '-lGL','-lGLEW', '-lglfw', '-lpng'],
  }
