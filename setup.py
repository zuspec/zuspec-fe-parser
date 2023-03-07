#*****************************************************************************
#* setup.py
#*
#* zuspec-parser Python extension setup file
#*****************************************************************************

import os
import sys
import subprocess
import sysconfig
import shutil

from setuptools import Extension, setup, find_namespace_packages
from setuptools.command.build_ext import build_ext as _build_ext
from distutils.file_util import copy_file
from Cython.Build import cythonize

version="0.0.1"

if "-DDEBUG" in sys.argv:
    sys.argv.remove("-DDEBUG")
    CMAKE_BUILD_TYPE="Debug"
    _DEBUG = True
else:
    CMAKE_BUILD_TYPE="Release"
    _DEBUG = False

zuspec_fe_parser_dir = os.path.dirname(os.path.abspath(__file__))
pythondir = os.path.join(zuspec_fe_parser_dir, "python")

if os.path.isdir(os.path.join(zuspec_fe_parser_dir, "packages")):
    print("Pacakges are inside this directory")
    packages_dir = os.path.join(zuspec_fe_parser_dir, "packages")
else:
    parent = os.path.dirname(zuspec_fe_parser_dir)

    if os.path.isdir(os.path.join(parent, "zuspec-parser")):
        print("zuspec-parser is a peer")
        packages_dir = parent
    else:
        raise Exception("Unexpected source layout")

cwd = os.getcwd()
if not os.path.isdir(os.path.join(cwd, "build")):
    os.makedirs(os.path.join(cwd, "build"))

env = os.environ.copy()
python_bindir = os.path.dirname(sys.executable)
print("python_bindir: %s" % str(python_bindir))

if "PATH" in env.keys():
    env["PATH"] = python_bindir + os.pathsep + env["PATH"]
else:
    env["PATH"] = python_bindir

# Run configure...
result = subprocess.run(
    ["cmake", 
     zuspec_fe_parser_dir,
     "-GNinja",
     "-DCMAKE_BUILD_TYPE=%s" % CMAKE_BUILD_TYPE,
     "-DPACKAGES_DIR=%s" % packages_dir,
     "-DCMAKE_INSTALL_PREFIX=%s" % os.path.join(cwd, "build")
     ],
    cwd=os.path.join(cwd, "build"),
    env=env)

if result.returncode != 0:
    raise Exception("cmake configure failed")

result = subprocess.run(
    ["ninja",
     "-j",
     "%d" % os.cpu_count()
     ],
    cwd=os.path.join(cwd, "build"),
    env=env)
if result.returncode != 0:
    raise Exception("build failed")

result = subprocess.run(
    ["ninja",
     "-j",
     "%d" % os.cpu_count(),
     "install"
     ],
    cwd=os.path.join(cwd, "build"),
    env=env)
if result.returncode != 0:
    raise Exception("build failed")

# Add in paths for required projects
for d in {"ciostream", "debug-mgr", "zsp-parser"}:
    if os.path.isdir(os.path.join(packages_dir, d, "python")):
        sys.path.insert(0, os.path.join(packages_dir, d, "python"))
    elif os.path.isdir(os.path.join(packages_dir, d, "src", d)):
        sys.path.insert(0, os.path.join(packages_dir, d, "src"))

#********************************************************************
#* Copy over required files
#********************************************************************
builddir = os.path.join(cwd, "build")

extra_compile_args = sysconfig.get_config_var('CFLAGS').split()
extra_compile_args = []
#extra_compile_args += ["-std=c++11", "-Wall", "-Wextra"]
if _DEBUG:
#    extra_compile_args += ["-g", "-O0", "-DDEBUG=%s" % _DEBUG_LEVEL, "-UNDEBUG"]
    extra_compile_args += ["-g", "-O0", "-UNDEBUG"]
else:
    extra_compile_args += ["-DNDEBUG", "-O3"]

class build_ext(_build_ext):
    def run(self):
        super().run()

    # Needed for Windows to not assume python module (generate interface in def file)
    def get_export_symbols(self, ext):
        return None

    def copy_extensions_to_source(self):
        """ Like the base class method, but copy libs into proper directory in develop. """
        print("copy_extensions_to_source", flush=True)
        super().copy_extensions_to_source()

        build_py = self.get_finalized_command("build_py")
        
        ext = self.extensions[0]
        fullname = self.get_ext_fullname(ext.name)
        filename = self.get_ext_filename(fullname)
        modpath = fullname.split(".")
        package = ".".join(modpath[:-1])
        package_dir = build_py.get_package_dir(package)

        copy_file(
            os.path.join(cwd, "build", "lib", "libzsp-fe-parser.so"),
            os.path.join(package_dir, "libzsp-fe-parser.so"))

        dest_filename = os.path.join(package_dir, filename)
        
        print("package_dir: %s dest_filename: %s" % (package_dir, dest_filename))
        
        return

include_dirs=[]

include_dirs.append(os.path.join(zuspec_fe_parser_dir, "src/include"))
include_dirs.append(os.path.join(packages_dir, "debug-mgr/src/include"))

build_dir = os.path.join(zuspec_fe_parser_dir, "build")

include_dirs.append(build_dir)
include_dirs.append(os.path.join(build_dir, "zsp_ast/ext"))
include_dirs.append(os.path.join(build_dir, "zsp_ast/src/include"))

if "CMAKE_BINARY_DIR" in os.environ.keys():
    cmake_binary_dir=os.environ["CMAKE_BINARY_DIR"]
    include_dirs.append(os.path.join(cmake_binary_dir, "zsp_ast/ext"))
    include_dirs.append(os.path.join(cmake_binary_dir, "zsp_ast/ext/"))
    include_dirs.append(os.path.join(cmake_binary_dir, "zsp_ast/src/include"))
    include_dirs.append(os.path.join(pythondir))

library_dirs = [] 

libraries = []

sources = []
sources.append(os.path.join(pythondir, "core.pyx"))

print("sources=" + str(sources))

# TODO: depending on the platform, perform the link differently
extra_link_args=[]
#extra_link_args.append(os.path.join(os.getcwd(), "../antlr4/lib/libantlr4-runtime.a"))

ext = Extension(
    "zsp_fe_parser.core", 
    sources,
    include_dirs=include_dirs,
    library_dirs=library_dirs,
    libraries=libraries,
    extra_link_args=extra_link_args,
    language="c++")

extensions=[ext]

setup(
    name="zuspec-fe-parser",
    packages=find_namespace_packages(where='python'),
    package_dir={'' : 'python' },
    version=version,
    author="Matthew Ballance",
    author_email="matt.ballance@gmail.com",
    description="Provides a PSS parser and related tools",
    long_description="""
    Zuspec front-end to bring in parsed PSS
    """,
    ext_modules=cythonize(extensions),
    cmdclass={'build_ext': build_ext}
    )
