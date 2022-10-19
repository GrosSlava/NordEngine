
import os
import sys

import Common.ToolsFunctionLibrary
import Common.Logger

import pathlib





'''
	Base information about project.
'''
class FProjectConfig:
	def __init__(self, ProjectPath: str):
		self.ProjectPath = ProjectPath		# absolute path to project root
		self.PathToEngine = ""				# absolute or relative path to engine root folder.
		self.IsEnginePathRelative = False	# indicate that PathToEngine set as relative path
		self.IsEngine = False				# mark that we are building engine project
		self.ProjectName = ""				# name of project	
		self.ShowConsole = False			# project will have platform console

	def GetAbsolutePathToEngine(self):
		if self.IsEnginePathRelative:
			return os.path.join(self.ProjectPath, self.PathToEngine)
		else:
			return self.PathToEngine
	#------------------------------------------------------#



'''
	Read project generation config form "ProjectConfig.txt" file.
	@param ProjectPath - absolute path to project root.
	@return FProjectConfig.
'''
def ScanProjectConfig(ProjectPath: str):
	if not Common.ToolsFunctionLibrary.CheckAbsPath(ProjectPath):
		Common.Logger.Log("ScanProjectConfig", "Invalid project path.")
		sys.exit(0)	


	LProjectConfigFilePath = os.path.join(ProjectPath, "ProjectConfig.txt")
	if not os.path.exists(LProjectConfigFilePath):
		print("Can't find 'ProjectConfig.txt' file!")
		sys.exit(0)

	LProjectConfig = FProjectConfig(ProjectPath)
	with open(LProjectConfigFilePath, "r", encoding = 'utf-8') as ProjectConfigFile:
		for LLine in ProjectConfigFile:
			S = LLine.split("=")
			if len(S) != 2:
				continue

			if S[0].strip() == "PathToEngine":
				LProjectConfig.PathToEngine = S[1].strip()
				if LProjectConfig.PathToEngine == ".":
					LProjectConfig.IsEngine = True
					LProjectConfig.IsEnginePathRelative = True
				elif LProjectConfig.PathToEngine[0] == ".":
					LProjectConfig.IsEnginePathRelative = True
			elif S[0].strip() == "ProjectName":
				LProjectConfig.ProjectName = S[1].strip()
			elif S[0].strip() == "ShowConsole":
				LProjectConfig.ShowConsole = Common.ToolsFunctionLibrary.StrToBool(S[1].strip())

	if LProjectConfig.PathToEngine.strip() == "":
		print("PathToEngine not set!")
		sys.exit(0)				
	if LProjectConfig.ProjectName.strip() == "":
		print("ProjectName not set!")
		sys.exit(0)	

	return LProjectConfig
#------------------------------------------------------#