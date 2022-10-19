
import os
import sys
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), os.path.pardir)))


import ToolsConfig
import Common.ProjectConfig
import Common.ToolsFunctionLibrary
import Common.Logger

import shutil
import pathlib





'''
	Method to call after each module build.
	@param ProjectConfig - scanned project config.
'''
def PostModuleBuild(ProjectConfig: Common.ProjectConfig.FProjectConfig):
	LBuildDir = os.path.join(ProjectConfig.ProjectPath, "Build")
	LBuildLibDir = os.path.join(LBuildDir, "lib")
	
	Common.ToolsFunctionLibrary.CreateDirIfNotExist(LBuildDir)
	Common.ToolsFunctionLibrary.CreateDirIfNotExist(LBuildLibDir)

	Common.ToolsFunctionLibrary.MoveAllFilesWithExtinsionFromDirReplacing(LBuildDir, LBuildLibDir, ".lib")
	Common.ToolsFunctionLibrary.MoveAllFilesWithExtinsionFromDirReplacing(LBuildDir, LBuildLibDir, ".exp")
	Common.ToolsFunctionLibrary.MoveAllFilesWithExtinsionFromDirReplacing(LBuildDir, LBuildLibDir, ".pdb")
	Common.ToolsFunctionLibrary.MoveAllFilesWithExtinsionFromDirReplacing(LBuildDir, LBuildLibDir, ".a")
#------------------------------------------------------#





if __name__ == "__main__":
	SolutionDir = sys.argv[1]
	
	LProjectConfig = Common.ProjectConfig.ScanProjectConfig(SolutionDir)
	
	PostModuleBuild(LProjectConfig)