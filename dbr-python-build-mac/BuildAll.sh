cd src
source ~/.bash_profile
# export PYENV_ROOT=~/.pyenv
# export PATH=$PYENV_ROOT/shims:$PATH

pyenv global 2.7.17
python setup.py build
pyenv global 3.5.4
python setup.py build
pyenv global 3.6.6
python setup.py build
pyenv global 3.7.5
python setup.py build
pyenv global 3.8.0
python setup.py build

cp build/lib.macosx-10.15-x86_64-2.7/dbr.so ../wheel27/dbr

cp build/lib.macosx-10.15-x86_64-3.5/dbr.cpython-35m-darwin.so ../wheel35/dbr

cp build/lib.macosx-10.15-x86_64-3.6/dbr.cpython-36m-darwin.so ../wheel36/dbr

cp build/lib.macosx-10.15-x86_64-3.7/dbr.cpython-37m-darwin.so ../wheel37/dbr

cp build/lib.macosx-10.15-x86_64-3.8/dbr.cpython-38-darwin.so ../wheel38/dbr

cd ../wheel27
pyenv global 2.7.17
python setup.py bdist_wheel
cp dist/* ../AllPackage

cd ../wheel35
pyenv global 3.5.4
python setup.py bdist_wheel
cp dist/* ../AllPackage

cd ../wheel36
pyenv global 3.6.6
python setup.py bdist_wheel
cp dist/* ../AllPackage

cd ../wheel37
pyenv global 3.7.5
python setup.py bdist_wheel
cp dist/* ../AllPackage

cd ../wheel38
pyenv global 3.8.0
python setup.py bdist_wheel
cp dist/* ../AllPackage
