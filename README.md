# INF3173 -- ATELIER GUIB
|Deroulement|Laboratoire 3|
|:---|---:|
|Communication et **capsules**|Mattermost|
|Github|https://github.com/guibdeux/guib-3173|
|Déroulement|Essaies et questions|
|Extra|https://overthewire.org/wargames/bandit/|
|Extra|cat /usr/share/vim/vim81/tutor/tutor.fr.utf-8|
|Extra|https://gcc.gnu.org/onlinedocs/gcc/Option-Summary.html|
|Extra|man glob.7|

|Logiciels suggérés| Suggestions|
|:---|---:|
|Jetbrains CLION (ordinateurs perso)|creation de compte, plugin vim| 
|Visual Studio (ordinateurs UQAM)| |

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