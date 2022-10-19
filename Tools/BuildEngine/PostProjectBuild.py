
import os
import sys
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), os.path.pardir)))


import ToolsConfig
import Common.ToolsFunctionLibrary
import Common.Logger
import Common.BuildInfo

import shutil
import pathlib
import glob





'''
	Method to call after project build.
	@param SolutionDir - absolute path to project root.
	@param EngineDir - absolute path to engine root.
	@param Platform - name of platform to compile. e,g ["Windows", "Linux"...]
'''
def PostProjectBuild(SolutionDir: str, EngineDir: str, Platform: str):
	if not Common.ToolsFunctionLibrary.CheckAbsPath(SolutionDir):
		Common.Logger.Log("PostProjectBuild", "Invalid solution path.")
		return
	if not Common.ToolsFunctionLibrary.CheckAbsPath(EngineDir):
		Common.Logger.Log("PostProjectBuild", "Invalid engine path.")
		return
	if not Common.BuildInfo.CheckPlatform(Platform):
		Common.Logger.Log("PostProjectBuild", "Invalid platform name.")
		return


	pass
#------------------------------------------------------#





if __name__ == "__main__":
	SolutionDir = sys.argv[1]
	EngineDir = sys.argv[2]
	Platform = sys.argv[3]
	
	PostProjectBuild(SolutionDir, EngineDir, Platform)