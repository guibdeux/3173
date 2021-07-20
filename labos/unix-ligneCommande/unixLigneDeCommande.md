#!/bin/bash

#Question 1
```
ls -l | grep .txt | wc -l > f1
```
### Les extensions ne sont pas un moyen fiable pour identifier des fichiers, préférer la commande file
### par exemple (peut être amélioré!)
```
file -i * | grep 'text/plain' | wc -l > f1
```

#Question 2
```
head -5 /etc/hosts | tail -1
```
# ou
```
tail -n +5 /etc/hosts | head -n 1
```

#Question 3
```
ln -s /bin/sh toto
```

#Question 4
###chmod 777 --> utilisateur: rwx, groupe: rwx, autres: rwx
###chmod 640 --> utilisateur: rw-, groupe: r--, autres: ---
###chmod 005 --> utilisateur: ---, groupe: ---, autres: r-x

#Question 5
###1
````
ls -1 /etc/c*
````
###2
````
find /etc -maxdepth 2 -name "c*" 2> /dev/null
````
###3
````
grep "^c" /usr/share/dict/words
````