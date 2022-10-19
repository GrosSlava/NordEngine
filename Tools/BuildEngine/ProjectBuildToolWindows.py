
import os
import sys
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), os.path.pardir)))


import ToolsConfig
import Common.ToolsFunctionLibrary
import Common.Logger
import PreProjectBuild
import PostModuleBuild
import PostProjectBuild

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
	if not Common.ToolsFunctionLibrary.CheckAbsPath(SolutionDir):
		Common.Logger.Log("BuildProjectWindows", "Invalid solution path.")
		return
	if not Common.ToolsFunctionLibrary.CheckAbsPath(EngineDir):
		Common.Logger.Log("BuildProjectWindows", "Invalid engine path.")
		return
	if not Common.ToolsFunctionLibrary.CheckAbsPath(MSVCPath):
		Common.Logger.Log("BuildProjectWindows", "Invalid MSVC path.")
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