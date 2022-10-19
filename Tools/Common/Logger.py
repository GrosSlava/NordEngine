
import os
import sys





'''
	Log message in universal format.
'''
def Log(FunctionName: str, Message: str):
	print("[{FunctionName}] --- {Message}".format(FunctionName = FunctionName, Message = Message))
#------------------------------------------------------#