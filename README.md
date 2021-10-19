# INF3173 -- ATELIER GUIB

https://docs.google.com/presentation/d/1T1JKK-E8m9iu_rUnuRkhM4OPkPbZ8lSM-vbLIyOgSRM/edit#slide=id.gf230178a39_0_149


|Deroulement|Laboratoire 4|
|:---|---:|
|Communication et **capsules**|Mattermost|
|Github|https://github.com/guibdeux/guib-3173|
|Extra|https://overthewire.org/wargames/bandit/|
|Extra|cat /usr/share/vim/vim81/tutor/tutor.fr.utf-8|
|Extra|https://gcc.gnu.org/onlinedocs/gcc/Option-Summary.html|
|Extra|https://waytolearnx.com/2018/08/qcm-linux-gestion-de-processus.html|
|Extra|man glob.7|
|Extra|man regex.7|
|Extra|man man|

|Logiciels suggérés| | Suggestions|
|:---|---|---:|
|Jetbrains CLION|creation de compte education, plugin vim|https://www.jetbrains.com/shop/eform/students|
|Virtual Box|Ubuntu 21.04|https://releases.ubuntu.com/21.04/|

### Update MAN
````
sudo apt install man-db manpages-posix
````
````
sudo apt install manpages-dev manpages-posix-dev
````

### Verifier la version de la libc
```
dpkg -l libc6
```
```
apt-cache policy libc6
```
