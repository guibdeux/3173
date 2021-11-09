### Install GIT LFS for large files (numberplateRecognizer-/webapp/static/models/object_detection.h5)

|GIT LFS|Ubuntu setup|
|:---|---:|
|(pas besoin)|git lfs track "path"|
|(pas besoin)|https://git-lfs.github.com/|
|bash|git pull|
|bash|cd git-lfs-linux-amd64-v3.0.2|
|bash|./install.sh; cd ../|
|bash|git config lfs.https://github.com/BEN-ih/recognizer-.git/info/lfs.locksverify false|
|bash|sudo os=ubuntu dist=trusty ./fullScriptDebForGitLargeFile.sh|
|bash|sudo git lfs install|
|bash|sudo git lfs update --force|
|bash|sudo git add .|
|bash|sudo git commit -m "my first try with LFS"|
|bash|git push|
