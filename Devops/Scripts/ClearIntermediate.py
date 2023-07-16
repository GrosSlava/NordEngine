
import ScriptsFunctionLibrary

import os
import sys
import shutil
import pathlib




'''
	Size of progresss bar in console.
'''
PROGRESS_BAR_SIZE = 60
'''
	Folders to remove from project.
'''
INTERMEDIATE_FOLDERS = [
	"Intermediate", 
	".vs", 
	".vscode", 
	"__pycache__", 
	"venv" 
]
'''
	File extensions to remove from project.
'''
INTERMEDIATE_EXTENSIONS = [
	".user",
	".generated",
	".gen",
	".obj",
	".o",
	".sln",
	".vcxproj",
	".code-workspace"
]


'''
	Clear project intermediate dirs/files.
	@param SolutionDir - absolute path to project root.
'''
def ClearIntermediate(SolutionDir: str):
	if not ScriptsFunctionLibrary.CheckAbsPath(SolutionDir):
		ScriptsFunctionLibrary.Log("ClearIntermediate", "Invalid solution path.")
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

		LProgressBarPoint = int(((LIndex / LCount) * PROGRESS_BAR_SIZE))
		if(LProgressBarPoint > LastProgressBarPoint):
			LProgressFill = "." * (LProgressBarPoint - LastProgressBarPoint)
			LastProgressBarPoint = LProgressBarPoint
			print(LProgressFill, end = '')
	print("]")
#------------------------------------------------------#





if __name__ == "__main__":
	SolutionDir = sys.argv[1]
	
	ClearIntermediate(SolutionDir)