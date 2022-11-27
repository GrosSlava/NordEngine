
import os
import sys
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), os.path.pardir)))


import ToolsConfig
import Common.ToolsFunctionLibrary
import Common.Logger
import ProjectDefaultFiles

import shutil
import glob
import pathlib
import subprocess





'''
	First time new project initialization.
	@param ProjectPath - absolute path to project root.
	@param EnginePath - absolute path to engne root.
'''
def InitNewProject(ProjectPath: str, EnginePath: str, ProjectName: str):
	if not Common.ToolsFunctionLibrary.CheckAbsPath(ProjectPath):
		Common.Logger.Log("InitNewProject", "Invalid project path.")
		return
	if not Common.ToolsFunctionLibrary.CheckAbsPath(EnginePath):
		Common.Logger.Log("InitNewProject", "Invalid engine path.")
		return


	LFilePath = os.path.join(ProjectPath, "ProjectConfig.txt")
	if not os.path.exists(LFilePath):
		with open(LFilePath, 'w') as f:
			f.write(ProjectDefaultFiles.GetDefault_ProjectConfig_FileText(EnginePath, ProjectName))

	LFilePath = os.path.join(ProjectPath, "GenerateWindowsProject.bat")
	if not os.path.exists(LFilePath):
		with open(LFilePath, 'w') as f:
			f.write(ProjectDefaultFiles.GetDefault_GenerateWindowsProject_BatFileText(EnginePath))

	LFilePath = os.path.join(ProjectPath, "ClearIntermediate.bat")
	if not os.path.exists(LFilePath):
		with open(LFilePath, 'w') as f:
			f.write(ProjectDefaultFiles.GetDefault_ClearIntermediate_BatFileText(EnginePath))

	LFilePath = os.path.join(ProjectPath, "BuildProjectWindows.bat")
	if not os.path.exists(LFilePath):
		with open(LFilePath, 'w') as f:
			f.write(ProjectDefaultFiles.GetDefault_BuildProjectWindows_BatFileText(EnginePath))

	LFilePath = os.path.join(ProjectPath, "GenerateDocs.bat")
	if not os.path.exists(LFilePath):
		with open(LFilePath, 'w') as f:
			f.write(ProjectDefaultFiles.GetDefault_GenerateDocs_BatFileText())
			
	LFilePath = os.path.join(ProjectPath, "Doxyfile")
	if not os.path.exists(LFilePath):
		subprocess.run(["doxygen", "-g"])
		ProjectDefaultFiles.Fixup_Doxyfile(LFilePath, ProjectName)
#------------------------------------------------------#





if __name__ == "__main__":
	SolutionDir = sys.argv[1]
	EngineDir = sys.argv[2]
	ProjectName = sys.argv[3]

	InitNewProject(SolutionDir, EngineDir, ProjectName)