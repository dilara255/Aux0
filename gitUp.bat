echo off

set major=1
set minor=0
set sub=0
set build=0

git status
pause

git add *
git status
pause

echo Enter commit message
set /P "Message="

git commit -m "v%major%.%minor%.%sub% #%build% - %Message%"

git push https://github.com/dilara255/Aux0.git

pause