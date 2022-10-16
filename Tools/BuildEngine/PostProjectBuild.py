
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
	pass





if __name__ == "__main__":
	SolutionDir = sys.argv[1]
	EngineDir = sys.argv[2]
	Platform = sys.argv[3]
	
	PostProjectBuild(SolutionDir, EngineDir, Platform)