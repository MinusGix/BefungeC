# BefungeC
Simple implementation of a Befunge interpretator in C, so I could get used to the language.
Probably shouldn't be used for anything. Also, certainly not to spec.

Notes:
 - Board size can be resized near the top of main.c (and recompile)
 - It doesn't do any math to display correctly on various terminal sizes, so if it looks odd then you probably need to increase your terminal size
 - Doesn't support:
	? (Random Direction)
	"" (Strings, adds every char inside to the stack)
	& (Get int from user and push to stack)
	~ (Get char from user and push to stack)
 - It's recursive, and as I write this I don't know a lot of the details of C so it might break after recursing too much.