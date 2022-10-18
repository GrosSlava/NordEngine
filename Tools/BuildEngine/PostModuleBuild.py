
import BuildHelpers

import os
import sys
import shutil
import pathlib




'''
	Method to call after each module build.
	@param SolutionDir - absolute path to project root.
'''
def PostModuleBuild(SolutionDir: str):
	if not BuildHelpers.CheckAbsPath(SolutionDir):
		print("[PostModuleBuild] --- Invalid solution path.")
		return


	BuildHelpers.CreateDirIfNotExist(os.path.join(SolutionDir, "Build"))
	BuildHelpers.CreateDirIfNotExist(os.path.join(SolutionDir, "Build", "lib"))

	BuildHelpers.MoveAllFilesWithExtinsionFromDirReplacing(os.path.join(SolutionDir, "Build"), os.path.join(SolutionDir, "Build", "lib"), ".lib")
	BuildHelpers.MoveAllFilesWithExtinsionFromDirReplacing(os.path.join(SolutionDir, "Build"), os.path.join(SolutionDir, "Build", "lib"), ".exp")
	BuildHelpers.MoveAllFilesWithExtinsionFromDirReplacing(os.path.join(SolutionDir, "Build"), os.path.join(SolutionDir, "Build", "lib"), ".pdb")
	BuildHelpers.MoveAllFilesWithExtinsionFromDirReplacing(os.path.join(SolutionDir, "Build"), os.path.join(SolutionDir, "Build", "lib"), ".a")
#------------------------------------------------------#





if __name__ == "__main__":
	SolutionDir = sys.argv[1]
	
	PostModuleBuild(SolutionDir)