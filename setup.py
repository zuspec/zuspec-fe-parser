#*****************************************************************************
#* setup.py
#*
#* zuspec-fe-parser Python extension setup file
#*****************************************************************************
import os
import sys
import platform
from setuptools import Extension, find_namespace_packages


proj_dir = os.path.dirname(os.path.abspath(__file__))

try:
    sys.path.insert(0, os.path.join(proj_dir, "python/zsp_fe_parser"))
    from __version__ import VERSION, BASE
    base = BASE
    version = VERSION
except ImportError as e:
    print("No build_num: %s" % str(e))
    version="0.0.1"

isSrcBuild = False

try:
    from ivpm.setup import setup
    isSrcBuild = os.path.isdir(os.path.join(proj_dir, "src"))
    print("zuspec-fe-parser: isSrcBuild: %s" % str(isSrcBuild), flush=True)
except ImportError as e:
    from setuptools import setup
    print("zuspec-fe-parser: not IVPM build (Falling back): %s" % str(e), flush=True)

zuspec_fe_parser_dir = proj_dir
pythondir = os.path.join(zuspec_fe_parser_dir, "python")

include_dirs=[]

include_dirs.append(os.path.join(zuspec_fe_parser_dir, "src/include"))
# include_dirs.append(os.path.join(packages_dir, "debug-mgr/src/include"))
# include_dirs.append(os.path.join(packages_dir, "vsc-dm/src/include"))
# include_dirs.append(os.path.join(packages_dir, "vsc-dm/python"))
# include_dirs.append(os.path.join(packages_dir, "zuspec-arl-dm/src/include"))
# include_dirs.append(os.path.join(packages_dir, "zuspec-parser/build/include"))
# include_dirs.append(os.path.join(packages_dir, "zuspec-parser/python"))
#build_dir = os.path.join(zuspec_fe_parser_dir, "build")
#
#include_dirs.append(build_dir)
#include_dirs.append(os.path.join(build_dir, "zsp_ast/ext"))
#include_dirs.append(os.path.join(build_dir, "zsp_ast/src/include"))

sources = []
sources.append(os.path.join(pythondir, "core.pyx"))


ext = Extension(
    "zsp_fe_parser.core", 
    sources,
    include_dirs=include_dirs,
    language="c++")

setup_args = dict(
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
    setup_requires=[
        'cython', 'ciostream', 'ivpm', 'zuspec-arl-dm',
        'zuspec-parser',
    ],
    install_requires=[
        'ciostream', 
        'zuspec-arl-dm>=%s' % base, 
        'zuspec-parser>=%s' % base
    ],
    entry_points={
        "ivpm.pkginfo": {
            'zuspec-fe-parser = zsp_fe_parser.pkginfo:PkgInfo'
        }
    },
    ext_modules=[ext],
    )

if isSrcBuild:
    setup_args["ivpm_extdep_pkgs"] = ["zuspec-arl-dm", "zuspec-parser"]
    setup_args["ivpm_extra_data"] = {
        "zsp_fe_parser": [
            ("src/include", "share"),
            ("build/{libdir}/{libpref}zsp-fe-parser{dllext}", "")
        ]
    }

print("--> setup", flush=True)
setup(**setup_args)
print("<-- setup", flush=True)

