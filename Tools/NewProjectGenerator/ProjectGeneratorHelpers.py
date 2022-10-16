
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


'''
	@return absolute path for given relative to script path.
'''
def GetRelativePath(ReativePath: str):
	SourceDir = os.path.dirname(os.path.abspath(__file__))
	return os.path.join(SourceDir, ReativePath)


'''
	Construct new dir only if it's not exist.
'''
def CreateDirIfNotExist(Dir: str):
	if not os.path.exists(Dir):
		os.mkdir(Dir)