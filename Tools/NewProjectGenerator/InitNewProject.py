
import ProjectGeneratorHelpers
import ProjectDefaultFiles

import os
import sys
import shutil
import glob
import pathlib




'''
	First time new project initialization.
	@param ProjectPath - absolute path to project root.
	@param EnginePath - absolute path to engne root.
'''
def InitNewProject(ProjectPath: str, EnginePath: str, ProjectName: str):
	if not ProjectGeneratorHelpers.CheckAbsPath(ProjectPath):
		print("[InitNewProject] --- Invalid project path.")
		return
	if not ProjectGeneratorHelpers.CheckAbsPath(EnginePath):
		print("[InitNewProject] --- Invalid engine path.")
		return
	if ProjectPath == EnginePath:
		print("[InitNewProject] --- Can't reinit engine.")
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
#------------------------------------------------------#





if __name__ == "__main__":
	SolutionDir = sys.argv[1]
	EngineDir = sys.argv[2]
	ProjectName = sys.argv[3]
	
	InitNewProject(SolutionDir, EngineDir, ProjectName)