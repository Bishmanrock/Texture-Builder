@echo off

:: Prompt for a commit message
set /p COMMIT_MSG="Enter your commit message: "

:: Pull the latest changes from the remote repository
git pull origin main

:: Push changes to GitHub with the provided commit message
echo Pushing changes to GitHub...
git remote add origin https://github.com/Bishmanrock/Texture-Builder.git
git add .
git commit -m "%COMMIT_MSG%"

:: Push change from the local branch (HEAD) to the "main" branch on GitHub
git push origin HEAD:main

echo Push completed.

pause