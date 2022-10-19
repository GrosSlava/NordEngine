
import os
import sys





'''
	Easy to use platform identifiers.
'''
WINDOWS_PLATFORM = "Windows"
LINUX_PLATFORM = "Linux"


'''
	Names of available for build platforms.
'''
SUPPORTED_BUILD_PLATFORMS = [ \
	WINDOWS_PLATFORM, \
	LINUX_PLATFORM \
]




'''
	Check that Name of platform is available.
	@return true if PlatformName is supported.
'''
def CheckPlatform(PlatformName: str):
	return PlatformName in SUPPORTED_BUILD_PLATFORMS
#------------------------------------------------------#