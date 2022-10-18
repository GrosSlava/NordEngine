
import os
import shutil
import pathlib




'''
	Names of available for build platforms.
'''
SUPPORTED_BUILD_PLATFORMS = [ \
	"Windows", \
	"Linux" \
]



'''
	Move file from Src to Dest with replacing.
'''
def MoveFileReplacing(Src: str, Dest: str, File: str):
	LSrcFile = os.path.join(Src, File)
	LDestFile = os.path.join(Dest, File)
	if not os.path.exists(LSrcFile) or not os.path.isfile(LSrcFile) or LSrcFile == LDestFile:
		return
	if os.path.exists(LDestFile):
		os.remove(LDestFile)
	shutil.move(LSrcFile, Dest)
#------------------------------------------------------#
'''
	Copy file from Src to Dest with replacing.
'''
def CopyFileReplacing(Src: str, Dest: str, File: str):
	LSrcFile = os.path.join(Src, File)
	LDestFile = os.path.join(Dest, File)
	if not os.path.exists(LSrcFile) or not os.path.isfile(LSrcFile) or LSrcFile == LDestFile:
		return
	if os.path.exists(LDestFile):
		os.remove(LDestFile)
	shutil.copy(LSrcFile, Dest)
#------------------------------------------------------#


'''
	Copy all files from Src dir to Dest dir with replacing.
	No recursive copying. Only files from first src dir level.
'''
def CopyAllFilesFromDirReplacing(Src: str, Dest: str):
	if not os.path.exists(Src) or Src == Dest:
		return
	for LFileName in os.listdir(Src):
		CopyFileReplacing(Src, Dest, LFileName)
#------------------------------------------------------#
'''
	Move all files from Src dir to Dest dir with replacing.
	No recursive moving. Only files from first src dir level.
'''
def MoveAllFilesFromDirReplacing(Src: str, Dest: str):
	if not os.path.exists(Src) or Src == Dest:
		return
	for LFileName in os.listdir(Src):
		MoveFileReplacing(Src, Dest, LFileName)
#------------------------------------------------------#


'''
	Move all files with Extension from Src dir to Dest dir with replacing.
	No recursive moving. Only files from first src dir level.
'''
def MoveAllFilesWithExtinsionFromDirReplacing(Src: str, Dest: str, Extension: str):
	if not os.path.exists(Src) or Src == Dest:
		return
	for LFileName in os.listdir(Src):
		if pathlib.Path(LFileName).suffix != Extension:
			continue
		MoveFileReplacing(Src, Dest, LFileName)
#------------------------------------------------------#
'''
	Copy all files with Extension from Src dir to Dest dir with replacing.
	No recursive moving. Only files from first src dir level.
'''
def CopyAllFilesWithExtinsionFromDirReplacing(Src: str, Dest: str, Extension: str):
	if not os.path.exists(Src) or Src == Dest:
		return
	for LFileName in os.listdir(Src):
		if pathlib.Path(LFileName).suffix != Extension:
			continue
		CopyFileReplacing(Src, Dest, LFileName)
#------------------------------------------------------#


'''
	Create a new directory only if it does not exist.
'''
def CreateDirIfNotExist(Dir: str):
	if not os.path.exists(Dir):
		os.mkdir(Dir)
#------------------------------------------------------#


'''
	Check that Path is absolute and exists.
'''
def CheckAbsPath(Path: str):
	return os.path.isabs(Path) and os.path.exists(Path)
#------------------------------------------------------#