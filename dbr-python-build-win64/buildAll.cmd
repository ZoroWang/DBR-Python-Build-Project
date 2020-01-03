cd src
SET VS90COMNTOOLS=%VS140COMNTOOLS%
python27 setup.py build
python35 setup.py build
python36 setup.py build
python37 setup.py build
python38 setup.py build

copy ..\bin\*.* ..\wheel\dbr\
cd build\
copy lib.win-amd64-2.7\*.* ..\..\wheel27\dbr\
copy lib.win-amd64-3.5\*.* ..\..\wheel35\dbr\
copy lib.win-amd64-3.6\*.* ..\..\wheel36\dbr\
copy lib.win-amd64-3.7\*.* ..\..\wheel37\dbr\
copy lib.win-amd64-3.8\*.* ..\..\wheel38\dbr\


cd ..\..\wheel27\
python27 setup.py bdist_wheel
copy dist\*.whl ..\AllPackage\
cd ..\wheel35\
python35 setup.py bdist_wheel
copy dist\*.whl ..\AllPackage\
cd ..\wheel36\
python36 setup.py bdist_wheel
copy dist\*.whl ..\AllPackage\
cd ..\wheel37\
python37 setup.py bdist_wheel
copy dist\*.whl ..\AllPackage\
cd ..\wheel38\
python38 setup.py bdist_wheel
copy dist\*.whl ..\AllPackage\

pause
