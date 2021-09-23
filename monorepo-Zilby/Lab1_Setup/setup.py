# 1. About this file
# The purpose of this file is to setup the correct git hooks
# for testing for the current project.
# 
# Hooks have to be copied from the 'tests' directory into
# the local .git/hooks directory which lives locally on your machine.
# This is relatively easy to do since I know the directory structure of
# your project.
# 
# 2. Purpose of hooks
# - Hooks may enforce things such as making sure your code compiles.
# - 
import os
import shutil

cwd = os.getcwd()
directoryName = os.path.basename(cwd)

# Print out a message to whoever is running the script.
# This in practice could be done silently, but is instead printed
# out for transparency to the user.
print('(setup.py) Setting up: '+directoryName)

# Now we copy in the appropriate hooks.
# In practice, we only need to do this once.
# However, the cost is relatively cheap, and if I make changes
# to the test I can ensure those will get copied over.
shutil.copy('../tests/'+directoryName+'/pre-commit','../.git/hooks/pre-commit')

# Make sure to make the script executable
os.system('chmod +x ../.git/hooks/pre-commit')

