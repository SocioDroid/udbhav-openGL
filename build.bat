@echo off
cls

@REM Create bin folder if it does not exists
if not exist "bin" (
  mkdir "bin"
  echo Folder created successfully.
) else (
  echo Folder already exists.
)
echo ==================================================================Cleaning up
@REM Cleanup
del OGLMain.exe
del OGLMain.res
del ".\\bin\\*.obj"

echo ==================================================================Compilation started
@REM Compiles all the available cpp files in the subdirectories
for /r %%i in (*.cpp) do (
	echo "%%i"
    cl.exe /Fo".\\bin\\" /c /EHsc /I C:\glew\include /I "C:\Program Files (x86)\OpenAL 1.1 SDK\include" /I "C:\assimp\include" /I "C:/GLM" /I "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v12.1\include" /I "C:\opencv\build\include" "%%i"
)
echo ==================================================================Compilation completed

rc.exe /Fo".\\bin\\OGLMain.res" ./src/OGLMain.rc


echo ==================================================================Compiling cuda
@REM nvcc -c -o bin\VertexUpdate.cu.obj src\utils\model_using_assim\VertexUpdate.cu

echo ==================================================================Compilation completed


echo ==================================================================Linking started
@REM Link
link.exe /out:OGLMain.exe ./bin/*.obj ./bin/OGLMain.res /LIBPATH:C:\glew\lib\Release\x64 /LIBPATH:"C:\Program Files (x86)\OpenAL 1.1 SDK\libs\Win64" /LIBPATH:"C:\assimp\lib" /LIBPATH:"C:\opencv\build\x64\vc16\lib" /LIBPATH:"C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v12.1\lib\x64" assimp-vc143-mtd.lib user32.lib gdi32.lib /SUBSYSTEM:WINDOWS

echo ==================================================================Linking completed

@REM Execute
OGLMain.exe
