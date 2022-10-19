
import os
import sys
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), os.path.pardir)))


import ToolsConfig
import Common.ProjectConfig
import Common.ToolsFunctionLibrary
import Common.Logger
import PreProjectBuild
import PostModuleBuild
import PostProjectBuild

import glob
import subprocess





'''
	Function for easy building project for Windows.
	@param ProjectConfig - scanned project config.
	@param MSVCPath - absolute path to MSVC.
'''
def BuildProjectWindows(ProjectConfig: Common.ProjectConfig.FProjectConfig, MSVCPath: str):
	if not Common.ToolsFunctionLibrary.CheckAbsPath(MSVCPath):
		Common.Logger.Log("BuildProjectWindows", "Invalid MSVC path.")
		return


	PreProjectBuild.PreProjectBuild(ProjectConfig, "Windows")

	for LModule in ProjectConfig.ModulesNames:
		if LModule.strip() == "":
			continue

		LCommand = "{MSVC} /verbosity:minimal /nologo {ModuleVCX} /property:SolutionDir={SolutionDir} /property:Configuration=Release /property:Platform=x64".format \
		(\
			MSVC = MSVCPath, \
			ModuleVCX = os.path.join(ProjectConfig.ProjectPath, "Source", LModule), \
			SolutionDir = ProjectConfig.ProjectPath \
		)
		subprocess.run(LCommand)

	PostModuleBuild.PostModuleBuild(ProjectConfig)
	PostProjectBuild.PostProjectBuild(ProjectConfig, "Windows")
#------------------------------------------------------#





if __name__ == "__main__":
	SolutionDir = sys.argv[1]
	MSVCPath = sys.argv[2]

	LProjectConfig = Common.ProjectConfig.ScanProjectConfig(SolutionDir)
	
	BuildProjectWindows(LProjectConfig, MSVCPath)