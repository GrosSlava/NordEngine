
import BuildHelpers
import PreProjectBuild
import PostModuleBuild
import PostProjectBuild

import os
import sys
import glob
import subprocess




'''
	Function for easy building project for Windows.
	@param SolutionDir - absolute path to project root.
	@param EngineDir - absolute path to engine root.
	@param MSVCPath - absolute path to MSVC.
	@Modules - list of modules names. For plugins it is Plugins\ModuleName.
'''
def BuildProjectWindows(SolutionDir: str, EngineDir: str, MSVCPath: str, Modules: list):
	if not BuildHelpers.CheckAbsPath(SolutionDir):
		print("[BuildProjectWindows] --- Invalid solution path.")
		return
	if not BuildHelpers.CheckAbsPath(EngineDir):
		print("[BuildProjectWindows] --- Invalid engine path.")
		return
	if not BuildHelpers.CheckAbsPath(MSVCPath):
		print("[BuildProjectWindows] --- Invalid MSVC path.")
		return


	PreProjectBuild.PreProjectBuild(SolutionDir, EngineDir, "Windows")
	
	for LModule in Modules:
		if LModule.strip() == "":
			continue

		LCommand = "{MSVC} /verbosity:minimal /nologo {ModuleVCX} /property:SolutionDir={SolutionDir} /property:Configuration=Release /property:Platform=x64".format \
		(\
			MSVC = MSVCPath, \
			ModuleVCX = os.path.join(SolutionDir, "Source", LModule), \
			SolutionDir = SolutionDir \
		)
		subprocess.run(LCommand)

	PostModuleBuild.PostModuleBuild(SolutionDir)
	PostProjectBuild.PostProjectBuild(SolutionDir, EngineDir, "Windows")
#------------------------------------------------------#





if __name__ == "__main__":
	SolutionDir = sys.argv[1]
	EngineDir = sys.argv[2]
	MSVCPath = sys.argv[3]
	Modules = list(sys.argv[4].split(';'))
	
	BuildProjectWindows(SolutionDir, EngineDir, MSVCPath, Modules)