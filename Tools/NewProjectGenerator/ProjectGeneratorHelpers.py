
import os
import sys
import shutil
import glob
import pathlib




'''
	Comvert str to bool.
	@return Bool by str.
'''
def StrToBool(S: str):
	return S == "1" or S == "Yes" or S == "YES" or S == "On" or S == "true" or S == "True" or S == "TRUE"
#------------------------------------------------------#


'''
	Create a new directory only if it does not exist.
'''
def CreateDirIfNotExist(Dir: str):
	if not os.path.exists(Dir):
		os.mkdir(Dir)
#------------------------------------------------------#


'''
	@return absolute path for given relative to script path.
'''
def GetRelativePath(ReativePath: str):
	SourceDir = os.path.dirname(os.path.abspath(__file__))
	return os.path.join(SourceDir, ReativePath)
#------------------------------------------------------#

'''
	Check that Path is absolute and exists.
'''
def CheckAbsPath(Path: str):
	return os.path.isabs(Path) and os.path.exists(Path)
#------------------------------------------------------#