
import BuildHelpers

import os
import sys
import shutil
import pathlib
import glob




'''
	Method to call before project rebuild.
	@param SolutionDir - absolute path to project root.
	@param EngineDir - absolute path to engine root.
	@param Platform - name of platform to compile. e,g ["Windows", "Linux"...]
'''
def PreProjectBuild(SolutionDir: str, EngineDir: str, Platform: str):
	if not BuildHelpers.CheckAbsPath(SolutionDir):
		print("[PreProjectBuild] --- Invalid solution path.")
		return
	if not BuildHelpers.CheckAbsPath(EngineDir):
		print("[PreProjectBuild] --- Invalid engine path.")
		return
	if not Platform in BuildHelpers.SUPPORTED_BUILD_PLATFORMS:
		print("[PreProjectBuild] --- Invalid platform name.")
		return


	if os.path.exists(os.path.join(SolutionDir, "Build")):
		shutil.rmtree(os.path.join(SolutionDir, "Build"))

	BuildHelpers.CreateDirIfNotExist(os.path.join(SolutionDir, "Intermediate"))
	BuildHelpers.CreateDirIfNotExist(os.path.join(SolutionDir, "Build"))
	BuildHelpers.CreateDirIfNotExist(os.path.join(SolutionDir, "Build", "lib"))
	
	if SolutionDir != EngineDir:
		if Platform == "Windows":
			BuildHelpers.CopyAllFilesWithExtinsionFromDirReplacing(os.path.join(EngineDir, "Build"), os.path.join(SolutionDir, "Build"), ".dll")
		elif Platform == "Linux":
			BuildHelpers.CopyAllFilesWithExtinsionFromDirReplacing(os.path.join(EngineDir, "Build"), os.path.join(SolutionDir, "Build"), ".so")

	if os.path.exists(os.path.join(SolutionDir, "Content")):
		shutil.copytree(os.path.join(SolutionDir, "Content"), os.path.join(SolutionDir, "Build", "Content"))
	else:
		BuildHelpers.CreateDirIfNotExist(os.path.join(SolutionDir, "Build", "Content"))

	for LFileName in glob.iglob(os.path.join(SolutionDir, "Source", "ThirdParty", "**"), recursive = False):
		if not os.path.isdir(LFileName):
			continue
		
		BuildHelpers.CopyAllFilesFromDirReplacing(os.path.join(LFileName, "bin", Platform), os.path.join(SolutionDir, "Build"))
		BuildHelpers.CopyAllFilesFromDirReplacing(os.path.join(LFileName, "lib", Platform), os.path.join(SolutionDir, "Build", "lib"))
#------------------------------------------------------#





if __name__ == "__main__":
	SolutionDir = sys.argv[1]
	EngineDir = sys.argv[2]
	Platform = sys.argv[3]
	
	PreProjectBuild(SolutionDir, EngineDir, Platform)