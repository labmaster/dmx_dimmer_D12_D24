copy .\*.stw .\UserFiles\
echo "This file has the sole purpose of getting the directory where is file is in handled by git" > .\UserFiles\used_for_git

cd .\c_dmx_d_maincpu_u2\
copy .\*.stp .\UserFiles\
echo "This file has the sole purpose of getting the directory where is file is in handled by git" > .\UserFiles\used_for_git
cd .\Debug\
del /Q /S .\*
echo "This file has the sole purpose of getting the directory where is file is in handled by git" > .\used_for_git
cd ..
cd .\Release\
copy .\*.s19 .\..\Binarys\
del /Q /S .\*
echo "This file has the sole purpose of getting the directory where is file is in handled by git" > .\used_for_git
cd ..

cd ..

cd .\c_dmx_d_slavecpu_u3_u4\
copy .\*.stp .\UserFiles\
echo "This file has the sole purpose of getting the directory where is file is in handled by git" > .\UserFiles\used_for_git
cd .\Debug\
del /Q /S .\*
echo "This file has the sole purpose of getting the directory where is file is in handled by git" > .\used_for_git
cd ..
cd .\Release\
copy .\*.s19 .\..\Binarys\
del /Q /S .\*
echo "This file has the sole purpose of getting the directory where is file is in handled by git" > .\used_for_git
cd ..

cd ..

