
import BuildHelpers

import os
import sys
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
	if not BuildHelpers.CheckAbsPath(SolutionDir):
		print("[PostProjectBuild] --- Invalid solution path.")
		return
	if not BuildHelpers.CheckAbsPath(EngineDir):
		print("[PostProjectBuild] --- Invalid engine path.")
		return
	if not Platform in BuildHelpers.SUPPORTED_BUILD_PLATFORMS:
		print("[PostProjectBuild] --- Invalid platform name.")
		return


	pass
#------------------------------------------------------#





if __name__ == "__main__":
	SolutionDir = sys.argv[1]
	EngineDir = sys.argv[2]
	Platform = sys.argv[3]
	
	PostProjectBuild(SolutionDir, EngineDir, Platform)