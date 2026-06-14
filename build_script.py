Import("env")
import os
import shutil
import tempfile

# print(env.Dump())

print ("###########################################################")
env.Replace(PROGNAME="%s" % env['PIOENV'] + "_sV" + env.GetProjectOption("custom_prog_version"))
print (env['PROGNAME'])

def copy_firmware(source, target, env):
	src = os.path.join(env['PROJECT_BUILD_DIR'], env['PIOENV'], "firmware.bin")
	trg = os.path.join(tempfile.gettempdir(), env['PROGNAME'] + ".bin")
	print (src)
	print (trg)
	shutil.copy2(src,trg)

env.AddPostAction("buildprog", copy_firmware)

# print (env.Dump())
