bin2array : binary to C array converter

Utilité : ouvre n'importe quel fichier et le transforme en tableau d'octets qui est
ensuite placé dans un fichier header C/C++ avec protection du header.
C'est un moyen de mettre une ressource binaire (image, dll) directement dans l'exécutable final, de cette façon on ne
distribue au client qu'un unique fichier au lieu d'utiliser un installateur, etc.

1er parametre : nom du fichier à transformer en tableau d'octets
2eme parametre : nom du header dans lequel placer le tableau d'octets
3eme parametre : nom du tableau C/C++ afin de donner une signification à la ressource
