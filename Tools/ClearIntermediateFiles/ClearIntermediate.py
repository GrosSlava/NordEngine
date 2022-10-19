
import os
import sys
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), os.path.pardir)))


import Common.ToolsFunctionLibrary
import Common.Logger

import shutil
import pathlib





'''
	Folders to remove from project.
'''
INTERMEDIATE_FOLDERS = [ \
"Intermediate", \
".vs", \
".vscode", \
"__pycache__" \
]

'''
	File extensions to remove from project.
'''
INTERMEDIATE_EXTENSIONS = [ \
".user", \
".generated", \
".gen", \
".obj", \
".o", \
".sln", \
".vcxproj", \
".code-workspace" \
]

ProgressBarSize = 60




'''
	Clear project intermediate dirs/files.
	@param SolutionDir - absolute path to project root.
'''
def ClearIntermediate(SolutionDir: str):
	if not Common.ToolsFunctionLibrary.CheckAbsPath(SolutionDir):
		Common.Logger.Log("ClearIntermediate", "Invalid solution path.")
		return


	LCount = len(list(os.walk(SolutionDir)))
	LIndex = 0
	LastProgressBarPoint = 0

	print("[", end = '')
	for SubDirs, Dirs, Files in os.walk(SolutionDir):
		for LDir in Dirs:
			if pathlib.Path(LDir).parts[-1] in INTERMEDIATE_FOLDERS:
				shutil.rmtree(os.path.join(SubDirs, LDir))

		for LFile in Files:
			if pathlib.Path(LFile).suffix in INTERMEDIATE_EXTENSIONS:
				os.remove(os.path.join(SubDirs, LFile))

		LIndex += 1

		LProgressBarPoint = int(((LIndex / LCount) * ProgressBarSize))
		if(LProgressBarPoint > LastProgressBarPoint):
			LProgressFill = "." * (LProgressBarPoint - LastProgressBarPoint)
			LastProgressBarPoint = LProgressBarPoint
			print(LProgressFill, end = '')
	print("]")
#------------------------------------------------------#





if __name__ == "__main__":
	SolutionDir = sys.argv[1]
	
	ClearIntermediate(SolutionDir)