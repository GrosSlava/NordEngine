
import os
import sys
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), os.path.pardir)))


import ToolsConfig
import Common.ProjectConfig
import Common.ToolsFunctionLibrary
import Common.Logger
import Common.BuildInfo

import shutil
import pathlib
import glob





'''
	Method to call after project build.
	@param ProjectConfig - scanned project config.
	@param Platform - name of platform to compile. e,g ["Windows", "Linux"...]
'''
def PostProjectBuild(ProjectConfig: Common.ProjectConfig.FProjectConfig, Platform: str):
	if not Common.BuildInfo.CheckPlatform(Platform):
		Common.Logger.Log("PostProjectBuild", "Invalid platform name.")
		return


	pass
#------------------------------------------------------#





if __name__ == "__main__":
	SolutionDir = sys.argv[1]
	Platform = sys.argv[2]
	
	LProjectConfig = Common.ProjectConfig.ScanProjectConfig(SolutionDir)
	
	PostProjectBuild(LProjectConfig, Platform)