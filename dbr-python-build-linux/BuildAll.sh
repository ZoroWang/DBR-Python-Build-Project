cd src
python2 setup.py build
python35 setup.py build
python36 setup.py build
python37 setup.py build
python38 setup.py build

cp build/lib.linux-x86_64-2.7/dbr.so ../wheel27/dbr

cp build/lib.linux-x86_64-3.5/dbr.cpython-35m-x86_64-linux-gnu.so ../wheel35/dbr

cp build/lib.linux-x86_64-3.6/dbr.cpython-36m-x86_64-linux-gnu.so ../wheel36/dbr

cp build/lib.linux-x86_64-3.7/dbr.cpython-37m-x86_64-linux-gnu.so ../wheel37/dbr

cp build/lib.linux-x86_64-3.8/dbr.cpython-38-x86_64-linux-gnu.so ../wheel38/dbr

cd ../wheel27
python2 setup.py bdist_wheel
cp dist/dbr-7.2.2.3-cp27-cp27mu-linux_x86_64.whl ../AllPackage

cd ../wheel35
python35 setup.py bdist_wheel
cp dist/dbr-7.2.2.3-cp35-cp35m-linux_x86_64.whl ../AllPackage

cd ../wheel36
python36 setup.py bdist_wheel
cp dist/dbr-7.2.2.3-cp36-cp36m-linux_x86_64.whl ../AllPackage

cd ../wheel37
python37 setup.py bdist_wheel
cp dist/dbr-7.2.2.3-cp37-cp37m-linux_x86_64.whl ../AllPackage

cd ../wheel38
python38 setup.py bdist_wheel
cp dist/dbr-7.2.2.3-cp38-cp38-linux_x86_64.whl ../AllPackage
