
import os
import sys
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), os.path.pardir)))


import Common.ToolsFunctionLibrary
import Common.Logger

import shutil
import pathlib





'''
	Method to call after each module build.
	@param SolutionDir - absolute path to project root.
'''
def PostModuleBuild(SolutionDir: str):
	if not Common.ToolsFunctionLibrary.CheckAbsPath(SolutionDir):
		Common.Logger.Log("PostModuleBuild", "Invalid solution path.")
		return


	LBuildDir = os.path.join(SolutionDir, "Build")
	
	Common.ToolsFunctionLibrary.CreateDirIfNotExist(LBuildDir)
	Common.ToolsFunctionLibrary.CreateDirIfNotExist(os.path.join(LBuildDir, "lib"))

	Common.ToolsFunctionLibrary.MoveAllFilesWithExtinsionFromDirReplacing(LBuildDir, os.path.join(LBuildDir, "lib"), ".lib")
	Common.ToolsFunctionLibrary.MoveAllFilesWithExtinsionFromDirReplacing(LBuildDir, os.path.join(LBuildDir, "lib"), ".exp")
	Common.ToolsFunctionLibrary.MoveAllFilesWithExtinsionFromDirReplacing(LBuildDir, os.path.join(LBuildDir, "lib"), ".pdb")
	Common.ToolsFunctionLibrary.MoveAllFilesWithExtinsionFromDirReplacing(LBuildDir, os.path.join(LBuildDir, "lib"), ".a")
#------------------------------------------------------#





if __name__ == "__main__":
	SolutionDir = sys.argv[1]
	
	PostModuleBuild(SolutionDir)