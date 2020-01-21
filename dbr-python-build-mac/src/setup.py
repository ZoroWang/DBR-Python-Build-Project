from distutils.core import setup, Extension
import sys
import os
import numpy
from distutils.command.install import install
from distutils.sysconfig import get_python_lib

# NumPy header file path.
numpy_include = os.path.join(os.path.dirname(numpy.__file__), "core", "include", "numpy")
print(numpy_include)

setup_path = os.path.dirname(os.path.realpath(__file__)) 

# Customization for different platforms
dbr_lib_dir = ''
dbr_dll = ''
dbr_include = ''
dbr_lib_name = 'DynamsoftBarcodeReader'

if sys.platform == "linux" or sys.platform == "linux2":
    # linux
    dbr_lib_dir = os.path.dirname(os.path.realpath(__file__))
elif sys.platform == "darwin":
    # OS X
    dbr_lib_dir = os.path.dirname(os.path.realpath(__file__))
    pass
elif sys.platform == "win32":
    # Windows
    dbr_lib_name = 'DBRx64'
    dbr_lib_dir = r'..\lib'
    dbr_dll = r'..\bin'

if sys.platform == "linux" or sys.platform == "linux2":
    ext_args = dict(
        include_dirs = [numpy_include],
        library_dirs = [dbr_lib_dir],
        extra_compile_args = ['-std=c99'],
        extra_link_args = ["-Wl,-rpath=$ORIGIN"],
        libraries = [dbr_lib_name]
    )
elif sys.platform == "darwin":
    ext_args = dict(
        include_dirs = [numpy_include],
        library_dirs = [dbr_lib_dir],
        extra_compile_args = ['-std=c99'],
        libraries = [dbr_lib_name]
    )



if sys.platform == "linux" or sys.platform == "linux2" or sys.platform == "darwin":
	module_dbr = Extension('dbr', ['dbr.c'], **ext_args)
else:
	module_dbr = Extension('dbr', sources=['dbr.c'], include_dirs=[
                       numpy_include], library_dirs=[dbr_lib_dir], libraries=[dbr_lib_name])


class CustomInstall(install):
    def run(self):
        install.run(self)
        if sys.platform == "linux" or sys.platform == "linux2":
            dst = get_python_lib()
            lib = setup_path + "/libDynamsoftBarcodeReader.so"
            os.system("cp {} {}".format(lib, dst))
        elif sys.platform == "darwin":
            dst = get_python_lib()
            lib = setup_path + "/libDynamsoftBarcodeReader.dylib"
            os.system("cp {} {}".format(lib, dst))


setup(name='dbr',
      version='7.2.2',
      description='Python barcode extension',
      author='Dynamsoft',
      author_email='support@dynamsoft.com',
      url='https://www.dynamsoft.com/Products/Dynamic-Barcode-Reader.aspx',
      license='https://www.dynamsoft.com/Products/barcode-reader-license-agreement.aspx',
      ext_modules=[module_dbr],
      long_description='Dynamsoft Barcode Reader is a software development toolkit which enables barcode recognition of Code 39, Code 129, QR Code, DataMatrix, PDF417 and Aztec.',
      platforms=['Windows', 'Linux', 'macOS'],
      cmdclass={'install': CustomInstall}
      )
