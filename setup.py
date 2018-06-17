from distutils.core import setup, Extension

module1 = Extension('ext',
    include_dirs = ['/usr/local/include'],
    libraries = ['pthread'],
    sources = ['extmodule.c'])

setup (name = 'ext',
    version = '1.0',
    description = 'This is a C extension for Python program',
    author = 'Somdip Dey',
    url = '',
    ext_modules = [module1])
